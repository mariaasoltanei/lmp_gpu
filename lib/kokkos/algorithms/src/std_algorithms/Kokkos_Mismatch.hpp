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

#ifndef KOKKOS_STD_ALGORITHMS_MISMATCH_HPP
#define KOKKOS_STD_ALGORITHMS_MISMATCH_HPP

#include "impl/Kokkos_Mismatch.hpp"
#include "Kokkos_BeginEnd.hpp"

namespace Kokkos {
namespace Experimental {

// FIXME: add mismatch overloads accepting 3 iterators.
// An overload consistent with other algorithms:
//
// auto mismatch(const ExecSpace& ex, It1 first1, It1 last1, It2 first2) {...}
//
// makes API ambiguous (with the overload accepting views).

//
// overload set accepting execution space
//
template <
    class ExecutionSpace, class IteratorType1, class IteratorType2,
    std::enable_if_t<Kokkos::is_execution_space_v<ExecutionSpace>, int> = 0>
::Kokkos::pair<IteratorType1, IteratorType2> mismatch(const ExecutionSpace& ex,
                                                      IteratorType1 first1,
                                                      IteratorType1 last1,
                                                      IteratorType2 first2,
                                                      IteratorType2 last2) {
  return Impl::mismatch_exespace_impl("Kokkos::mismatch_iterator_api_default",
                                      ex, first1, last1, first2, last2);
}

template <
    class ExecutionSpace, class IteratorType1, class IteratorType2,
    class BinaryPredicateType,
    std::enable_if_t<Kokkos::is_execution_space_v<ExecutionSpace>, int> = 0>
::Kokkos::pair<IteratorType1, IteratorType2> mismatch(
    const ExecutionSpace& ex, IteratorType1 first1, IteratorType1 last1,
    IteratorType2 first2, IteratorType2 last2,
    BinaryPredicateType&& predicate) {
  return Impl::mismatch_exespace_impl(
      "Kokkos::mismatch_iterator_api_default", ex, first1, last1, first2, last2,
      std::forward<BinaryPredicateType>(predicate));
}

template <
    class ExecutionSpace, class IteratorType1, class IteratorType2,
    std::enable_if_t<Kokkos::is_execution_space_v<ExecutionSpace>, int> = 0>
::Kokkos::pair<IteratorType1, IteratorType2> mismatch(
    const std::string& label, const ExecutionSpace& ex, IteratorType1 first1,
    IteratorType1 last1, IteratorType2 first2, IteratorType2 last2) {
  return Impl::mismatch_exespace_impl(label, ex, first1, last1, first2, last2);
}

template <
    class ExecutionSpace, class IteratorType1, class IteratorType2,
    class BinaryPredicateType,
    std::enable_if_t<Kokkos::is_execution_space_v<ExecutionSpace>, int> = 0>
::Kokkos::pair<IteratorType1, IteratorType2> mismatch(
    const std::string& label, const ExecutionSpace& ex, IteratorType1 first1,
    IteratorType1 last1, IteratorType2 first2, IteratorType2 last2,
    BinaryPredicateType&& predicate) {
  return Impl::mismatch_exespace_impl(
      label, ex, first1, last1, first2, last2,
      std::forward<BinaryPredicateType>(predicate));
}

template <
    class ExecutionSpace, class DataType1, class... Properties1,
    class DataType2, class... Properties2,
    std::enable_if_t<Kokkos::is_execution_space_v<ExecutionSpace>, int> = 0>
auto mismatch(const ExecutionSpace& ex,
              const ::Kokkos::View<DataType1, Properties1...>& view1,
              const ::Kokkos::View<DataType2, Properties2...>& view2) {
  Impl::static_assert_is_admissible_to_kokkos_std_algorithms(view1);
  Impl::static_assert_is_admissible_to_kokkos_std_algorithms(view2);

  namespace KE = ::Kokkos::Experimental;
  return Impl::mismatch_exespace_impl("Kokkos::mismatch_view_api_default", ex,
                                      KE::begin(view1), KE::end(view1),
                                      KE::begin(view2), KE::end(view2));
}

template <
    class ExecutionSpace, class DataType1, class... Properties1,
    class DataType2, class... Properties2, class BinaryPredicateType,
    std::enable_if_t<Kokkos::is_execution_space_v<ExecutionSpace>, int> = 0>
auto mismatch(const ExecutionSpace& ex,
              const ::Kokkos::View<DataType1, Properties1...>& view1,
              const ::Kokkos::View<DataType2, Properties2...>& view2,
              BinaryPredicateType&& predicate) {
  Impl::static_assert_is_admissible_to_kokkos_std_algorithms(view1);
  Impl::static_assert_is_admissible_to_kokkos_std_algorithms(view2);

  namespace KE = ::Kokkos::Experimental;
  return Impl::mismatch_exespace_impl(
      "Kokkos::mismatch_view_api_default", ex, KE::begin(view1), KE::end(view1),
      KE::begin(view2), KE::end(view2),
      std::forward<BinaryPredicateType>(predicate));
}

template <
    class ExecutionSpace, class DataType1, class... Properties1,
    class DataType2, class... Properties2,
    std::enable_if_t<Kokkos::is_execution_space_v<ExecutionSpace>, int> = 0>
auto mismatch(const std::string& label, const ExecutionSpace& ex,
              const ::Kokkos::View<DataType1, Properties1...>& view1,
              const ::Kokkos::View<DataType2, Properties2...>& view2) {
  Impl::static_assert_is_admissible_to_kokkos_std_algorithms(view1);
  Impl::static_assert_is_admissible_to_kokkos_std_algorithms(view2);

  namespace KE = ::Kokkos::Experimental;
  return Impl::mismatch_exespace_impl(label, ex, KE::begin(view1),
                                      KE::end(view1), KE::begin(view2),
                                      KE::end(view2));
}

template <
    class ExecutionSpace, class DataType1, class... Properties1,
    class DataType2, class... Properties2, class BinaryPredicateType,
    std::enable_if_t<Kokkos::is_execution_space_v<ExecutionSpace>, int> = 0>
auto mismatch(const std::string& label, const ExecutionSpace& ex,
              const ::Kokkos::View<DataType1, Properties1...>& view1,
              const ::Kokkos::View<DataType2, Properties2...>& view2,
              BinaryPredicateType&& predicate) {
  Impl::static_assert_is_admissible_to_kokkos_std_algorithms(view1);
  Impl::static_assert_is_admissible_to_kokkos_std_algorithms(view2);

  namespace KE = ::Kokkos::Experimental;
  return Impl::mismatch_exespace_impl(
      label, ex, KE::begin(view1), KE::end(view1), KE::begin(view2),
      KE::end(view2), std::forward<BinaryPredicateType>(predicate));
}

//
// overload set accepting a team handle
// Note: for now omit the overloads accepting a label
// since they cause issues on device because of the string allocation.
//
template <class TeamHandleType, class IteratorType1, class IteratorType2,
          std::enable_if_t<Kokkos::is_team_handle_v<TeamHandleType>, int> = 0>
KOKKOS_FUNCTION ::Kokkos::pair<IteratorType1, IteratorType2> mismatch(
    const TeamHandleType& teamHandle, IteratorType1 first1, IteratorType1 last1,
    IteratorType2 first2, IteratorType2 last2) {
  return Impl::mismatch_team_impl(teamHandle, first1, last1, first2, last2);
}

template <class TeamHandleType, class IteratorType1, class IteratorType2,
          class BinaryPredicateType,
          std::enable_if_t<Kokkos::is_team_handle_v<TeamHandleType>, int> = 0>
KOKKOS_FUNCTION ::Kokkos::pair<IteratorType1, IteratorType2> mismatch(
    const TeamHandleType& teamHandle, IteratorType1 first1, IteratorType1 last1,
    IteratorType2 first2, IteratorType2 last2,
    BinaryPredicateType&& predicate) {
  return Impl::mismatch_team_impl(teamHandle, first1, last1, first2, last2,
                                  std::forward<BinaryPredicateType>(predicate));
}

template <class TeamHandleType, class DataType1, class... Properties1,
          class DataType2, class... Properties2,
          std::enable_if_t<Kokkos::is_team_handle_v<TeamHandleType>, int> = 0>
KOKKOS_FUNCTION auto mismatch(
    const TeamHandleType& teamHandle,
    const ::Kokkos::View<DataType1, Properties1...>& view1,
    const ::Kokkos::View<DataType2, Properties2...>& view2) {
  Impl::static_assert_is_admissible_to_kokkos_std_algorithms(view1);
  Impl::static_assert_is_admissible_to_kokkos_std_algorithms(view2);

  namespace KE = ::Kokkos::Experimental;
  return Impl::mismatch_team_impl(teamHandle, KE::begin(view1), KE::end(view1),
                                  KE::begin(view2), KE::end(view2));
}

template <class TeamHandleType, class DataType1, class... Properties1,
          class DataType2, class... Properties2, class BinaryPredicateType,
          std::enable_if_t<Kokkos::is_team_handle_v<TeamHandleType>, int> = 0>
KOKKOS_FUNCTION auto mismatch(
    const TeamHandleType& teamHandle,
    const ::Kokkos::View<DataType1, Properties1...>& view1,
    const ::Kokkos::View<DataType2, Properties2...>& view2,
    BinaryPredicateType&& predicate) {
  Impl::static_assert_is_admissible_to_kokkos_std_algorithms(view1);
  Impl::static_assert_is_admissible_to_kokkos_std_algorithms(view2);

  namespace KE = ::Kokkos::Experimental;
  return Impl::mismatch_team_impl(teamHandle, KE::begin(view1), KE::end(view1),
                                  KE::begin(view2), KE::end(view2),
                                  std::forward<BinaryPredicateType>(predicate));
}

}  // namespace Experimental
}  // namespace Kokkos

#endif
