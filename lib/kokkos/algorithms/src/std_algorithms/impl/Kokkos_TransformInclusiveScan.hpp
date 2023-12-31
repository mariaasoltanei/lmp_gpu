//@HEADER
// ************************************************************************
//
//                        Kokkos v. 4.0
//       Copyright (2022) National Technology & Engineering
//               Solutions of Sandia, LLC (NTESS).
//
// Under the terms of Contract DE-NA0003525 with NTESS,
// the U.S. Government retains certain rights in this software.
//
// Part of Kokkos, under the Apache License v2.0 with LLVM Exceptions.
// See https://kokkos.org/LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//@HEADER

#ifndef KOKKOS_STD_ALGORITHMS_TRANSFORM_INCLUSIVE_SCAN_IMPL_HPP
#define KOKKOS_STD_ALGORITHMS_TRANSFORM_INCLUSIVE_SCAN_IMPL_HPP

#include <Kokkos_Core.hpp>
#include "Kokkos_Constraints.hpp"
#include "Kokkos_HelperPredicates.hpp"
#include "Kokkos_ValueWrapperForNoNeutralElement.hpp"
#include "Kokkos_IdentityReferenceUnaryFunctor.hpp"
#include <std_algorithms/Kokkos_Distance.hpp>
#include <string>

namespace Kokkos {
namespace Experimental {
namespace Impl {

template <class ExeSpace, class IndexType, class ValueType, class FirstFrom,
          class FirstDest, class BinaryOpType, class UnaryOpType>
struct ExeSpaceTransformInclusiveScanNoInitValueFunctor {
  using execution_space = ExeSpace;
  using value_type      = ValueWrapperForNoNeutralElement<ValueType>;

  FirstFrom m_first_from;
  FirstDest m_first_dest;
  BinaryOpType m_binary_op;
  UnaryOpType m_unary_op;

  KOKKOS_FUNCTION
  ExeSpaceTransformInclusiveScanNoInitValueFunctor(FirstFrom first_from,
                                                   FirstDest first_dest,
                                                   BinaryOpType bop,
                                                   UnaryOpType uop)
      : m_first_from(std::move(first_from)),
        m_first_dest(std::move(first_dest)),
        m_binary_op(std::move(bop)),
        m_unary_op(std::move(uop)) {}

  KOKKOS_FUNCTION
  void operator()(const IndexType i, value_type& update,
                  const bool final_pass) const {
    const auto tmp = value_type{m_unary_op(m_first_from[i]), false};
    this->join(update, tmp);
    if (final_pass) {
      m_first_dest[i] = update.val;
    }
  }

  KOKKOS_FUNCTION
  void init(value_type& update) const {
    update.val        = {};
    update.is_initial = true;
  }

  KOKKOS_FUNCTION
  void join(value_type& update, const value_type& input) const {
    if (input.is_initial) return;

    if (update.is_initial) {
      update.val = input.val;
    } else {
      update.val = m_binary_op(update.val, input.val);
    }
    update.is_initial = false;
  }
};

template <class ExeSpace, class IndexType, class ValueType, class FirstFrom,
          class FirstDest, class BinaryOpType, class UnaryOpType>
struct ExeSpaceTransformInclusiveScanWithInitValueFunctor {
  using execution_space = ExeSpace;
  using value_type      = ValueWrapperForNoNeutralElement<ValueType>;

  FirstFrom m_first_from;
  FirstDest m_first_dest;
  BinaryOpType m_binary_op;
  UnaryOpType m_unary_op;
  ValueType m_init;

  KOKKOS_FUNCTION
  ExeSpaceTransformInclusiveScanWithInitValueFunctor(FirstFrom first_from,
                                                     FirstDest first_dest,
                                                     BinaryOpType bop,
                                                     UnaryOpType uop,
                                                     ValueType init)
      : m_first_from(std::move(first_from)),
        m_first_dest(std::move(first_dest)),
        m_binary_op(std::move(bop)),
        m_unary_op(std::move(uop)),
        m_init(std::move(init)) {}

  KOKKOS_FUNCTION
  void operator()(const IndexType i, value_type& update,
                  const bool final_pass) const {
    const auto tmp = value_type{m_unary_op(m_first_from[i]), false};
    this->join(update, tmp);

    if (final_pass) {
      m_first_dest[i] = m_binary_op(update.val, m_init);
    }
  }

  KOKKOS_FUNCTION
  void init(value_type& update) const {
    update.val        = {};
    update.is_initial = true;
  }

  KOKKOS_FUNCTION
  void join(value_type& update, const value_type& input) const {
    if (input.is_initial) return;

    if (update.is_initial) {
      update.val = input.val;
    } else {
      update.val = m_binary_op(update.val, input.val);
    }
    update.is_initial = false;
  }
};

//
// exespace impl
//

// -------------------------------------------------------------
// transform_inclusive_scan_exespace_impl without init_value
// -------------------------------------------------------------
template <class ExecutionSpace, class InputIteratorType,
          class OutputIteratorType, class BinaryOpType, class UnaryOpType>
OutputIteratorType transform_inclusive_scan_exespace_impl(
    const std::string& label, const ExecutionSpace& ex,
    InputIteratorType first_from, InputIteratorType last_from,
    OutputIteratorType first_dest, BinaryOpType binary_op,
    UnaryOpType unary_op) {
  // checks
  Impl::static_assert_random_access_and_accessible(ex, first_from, first_dest);
  Impl::static_assert_iterators_have_matching_difference_type(first_from,
                                                              first_dest);
  Impl::expect_valid_range(first_from, last_from);

  // aliases
  using index_type = typename InputIteratorType::difference_type;
  using value_type =
      std::remove_const_t<typename InputIteratorType::value_type>;
  using func_type = ExeSpaceTransformInclusiveScanNoInitValueFunctor<
      ExecutionSpace, index_type, value_type, InputIteratorType,
      OutputIteratorType, BinaryOpType, UnaryOpType>;

  // run
  const auto num_elements =
      Kokkos::Experimental::distance(first_from, last_from);
  ::Kokkos::parallel_scan(
      label, RangePolicy<ExecutionSpace>(ex, 0, num_elements),
      func_type(first_from, first_dest, binary_op, unary_op));
  ex.fence("Kokkos::transform_inclusive_scan: fence after operation");

  // return
  return first_dest + num_elements;
}

// -------------------------------------------------------------
// transform_inclusive_scan_exespace_impl with init_value
// -------------------------------------------------------------
template <class ExecutionSpace, class InputIteratorType,
          class OutputIteratorType, class BinaryOpType, class UnaryOpType,
          class ValueType>
OutputIteratorType transform_inclusive_scan_exespace_impl(
    const std::string& label, const ExecutionSpace& ex,
    InputIteratorType first_from, InputIteratorType last_from,
    OutputIteratorType first_dest, BinaryOpType binary_op, UnaryOpType unary_op,
    ValueType init_value) {
  // checks
  Impl::static_assert_random_access_and_accessible(ex, first_from, first_dest);
  Impl::static_assert_iterators_have_matching_difference_type(first_from,
                                                              first_dest);
  Impl::expect_valid_range(first_from, last_from);

  // aliases
  using index_type = typename InputIteratorType::difference_type;
  using func_type  = ExeSpaceTransformInclusiveScanWithInitValueFunctor<
      ExecutionSpace, index_type, ValueType, InputIteratorType,
      OutputIteratorType, BinaryOpType, UnaryOpType>;

  // run
  const auto num_elements =
      Kokkos::Experimental::distance(first_from, last_from);
  ::Kokkos::parallel_scan(label,
                          RangePolicy<ExecutionSpace>(ex, 0, num_elements),
                          func_type(first_from, first_dest, binary_op, unary_op,
                                    std::move(init_value)));
  ex.fence("Kokkos::transform_inclusive_scan: fence after operation");

  // return
  return first_dest + num_elements;
}

//
// team impl
//

template <class ExeSpace, class ValueType, class FirstFrom, class FirstDest,
          class BinaryOpType, class UnaryOpType>
struct TeamTransformInclusiveScanNoInitValueFunctor {
  using execution_space = ExeSpace;
  using index_type      = typename FirstFrom::difference_type;

  FirstFrom m_first_from;
  FirstDest m_first_dest;
  BinaryOpType m_binary_op;
  UnaryOpType m_unary_op;

  KOKKOS_FUNCTION
  TeamTransformInclusiveScanNoInitValueFunctor(FirstFrom first_from,
                                               FirstDest first_dest,
                                               BinaryOpType bop,
                                               UnaryOpType uop)
      : m_first_from(std::move(first_from)),
        m_first_dest(std::move(first_dest)),
        m_binary_op(std::move(bop)),
        m_unary_op(std::move(uop)) {}

  KOKKOS_FUNCTION
  void operator()(const index_type i, ValueType& update,
                  const bool final_pass) const {
    const auto tmp = ValueType{m_unary_op(m_first_from[i])};
    this->join(update, tmp);
    if (final_pass) {
      m_first_dest[i] = update;
    }
  }

  KOKKOS_FUNCTION
  void init(ValueType& update) const { update = {}; }

  KOKKOS_FUNCTION
  void join(ValueType& update, const ValueType& input) const {
    update = m_binary_op(update, input);
  }
};

template <class ExeSpace, class ValueType, class FirstFrom, class FirstDest,
          class BinaryOpType, class UnaryOpType>
struct TeamTransformInclusiveScanWithInitValueFunctor {
  using execution_space = ExeSpace;
  using index_type      = typename FirstFrom::difference_type;

  FirstFrom m_first_from;
  FirstDest m_first_dest;
  BinaryOpType m_binary_op;
  UnaryOpType m_unary_op;
  ValueType m_init;

  KOKKOS_FUNCTION
  TeamTransformInclusiveScanWithInitValueFunctor(FirstFrom first_from,
                                                 FirstDest first_dest,
                                                 BinaryOpType bop,
                                                 UnaryOpType uop,
                                                 ValueType init)
      : m_first_from(std::move(first_from)),
        m_first_dest(std::move(first_dest)),
        m_binary_op(std::move(bop)),
        m_unary_op(std::move(uop)),
        m_init(std::move(init)) {}

  KOKKOS_FUNCTION
  void operator()(const index_type i, ValueType& update,
                  const bool final_pass) const {
    const auto tmp = ValueType{m_unary_op(m_first_from[i])};
    this->join(update, tmp);

    if (final_pass) {
      m_first_dest[i] = m_binary_op(update, m_init);
    }
  }

  KOKKOS_FUNCTION
  void init(ValueType& update) const { update = {}; }

  KOKKOS_FUNCTION
  void join(ValueType& update, const ValueType& input) const {
    update = m_binary_op(update, input);
  }
};

// -------------------------------------------------------------
// transform_inclusive_scan_team_impl without init_value
// -------------------------------------------------------------
template <class TeamHandleType, class InputIteratorType,
          class OutputIteratorType, class BinaryOpType, class UnaryOpType>
KOKKOS_FUNCTION OutputIteratorType transform_inclusive_scan_team_impl(
    const TeamHandleType& teamHandle, InputIteratorType first_from,
    InputIteratorType last_from, OutputIteratorType first_dest,
    BinaryOpType binary_op, UnaryOpType unary_op) {
  // checks
  Impl::static_assert_random_access_and_accessible(teamHandle, first_from,
                                                   first_dest);
  Impl::static_assert_iterators_have_matching_difference_type(first_from,
                                                              first_dest);
  Impl::expect_valid_range(first_from, last_from);

  // aliases
  using exe_space = typename TeamHandleType::execution_space;
  using value_type =
      std::remove_const_t<typename InputIteratorType::value_type>;
  using func_type = TeamTransformInclusiveScanNoInitValueFunctor<
      exe_space, value_type, InputIteratorType, OutputIteratorType,
      BinaryOpType, UnaryOpType>;

  // run
  const auto num_elements =
      Kokkos::Experimental::distance(first_from, last_from);
  ::Kokkos::parallel_scan(
      TeamThreadRange(teamHandle, 0, num_elements),
      func_type(first_from, first_dest, binary_op, unary_op));
  teamHandle.team_barrier();

  // return
  return first_dest + num_elements;
}

// -------------------------------------------------------------
// transform_inclusive_scan_team_impl with init_value
// -------------------------------------------------------------
template <class TeamHandleType, class InputIteratorType,
          class OutputIteratorType, class BinaryOpType, class UnaryOpType,
          class ValueType>
KOKKOS_FUNCTION OutputIteratorType transform_inclusive_scan_team_impl(
    const TeamHandleType& teamHandle, InputIteratorType first_from,
    InputIteratorType last_from, OutputIteratorType first_dest,
    BinaryOpType binary_op, UnaryOpType unary_op, ValueType init_value) {
  // checks
  Impl::static_assert_random_access_and_accessible(teamHandle, first_from,
                                                   first_dest);
  Impl::static_assert_iterators_have_matching_difference_type(first_from,
                                                              first_dest);
  Impl::expect_valid_range(first_from, last_from);

  // aliases
  using exe_space = typename TeamHandleType::execution_space;
  using func_type = TeamTransformInclusiveScanWithInitValueFunctor<
      exe_space, ValueType, InputIteratorType, OutputIteratorType, BinaryOpType,
      UnaryOpType>;

  // run
  const auto num_elements =
      Kokkos::Experimental::distance(first_from, last_from);
  ::Kokkos::parallel_scan(TeamThreadRange(teamHandle, 0, num_elements),
                          func_type(first_from, first_dest, binary_op, unary_op,
                                    std::move(init_value)));
  teamHandle.team_barrier();

  // return
  return first_dest + num_elements;
}

}  // namespace Impl
}  // namespace Experimental
}  // namespace Kokkos

#endif
