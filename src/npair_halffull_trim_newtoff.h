/* -*- c++ -*- ----------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   https://www.lammps.org/, Sandia National Laboratories
   LAMMPS development team: developers@lammps.org

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#ifdef NPAIR_CLASS
// clang-format off
NPairStyle(halffull/trim/newtoff,
           NPairHalffullTrimNewtoff,
           NP_HALF_FULL | NP_NEWTOFF | NP_NSQ | NP_BIN | NP_MULTI | NP_MULTI_OLD | NP_HALF |
           NP_ORTHO | NP_TRI | NP_TRIM);

NPairStyle(halffull/trim/newtoff/skip,
           NPairHalffullTrimNewtoff,
           NP_HALF_FULL | NP_NEWTOFF | NP_NSQ | NP_BIN | NP_MULTI | NP_MULTI_OLD | NP_HALF |
           NP_ORTHO | NP_TRI | NP_TRIM | NP_SKIP);

NPairStyle(halffull/trim/newtoff/ghost,
           NPairHalffullTrimNewtoff,
           NP_HALF_FULL | NP_NEWTOFF | NP_NSQ | NP_BIN | NP_MULTI | NP_MULTI_OLD | NP_HALF |
           NP_ORTHO | NP_TRI | NP_TRIM | NP_GHOST);

NPairStyle(halffull/trim/newtoff/skip/ghost,
           NPairHalffullTrimNewtoff,
           NP_HALF_FULL | NP_NEWTOFF | NP_NSQ | NP_BIN | NP_MULTI | NP_MULTI_OLD | NP_HALF |
           NP_ORTHO | NP_TRI | NP_TRIM | NP_SKIP | NP_GHOST);
// clang-format on
#else

#ifndef LMP_NPAIR_HALFFULL_TRIM_NEWTOFF_H
#define LMP_NPAIR_HALFFULL_TRIM_NEWTOFF_H

#include "npair.h"

namespace LAMMPS_NS {

class NPairHalffullTrimNewtoff : public NPair {
 public:
  NPairHalffullTrimNewtoff(class LAMMPS *);
  void build(class NeighList *) override;
};

}    // namespace LAMMPS_NS

#endif
#endif
