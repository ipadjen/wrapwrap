// Copyright (c) 2006  GeometryFactory (France). All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0-dev/Surface_mesh_simplification/include/CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_predicate.h $
// $Id: include/CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_predicate.h a484bfa $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Fernando Cacciola <fernando.cacciola@geometryfactory.com>
//
#ifndef CGAL_SURFACE_MESH_SIMPLIFICATION_POLICIES_EDGE_COLLAPSE_COUNT_RATIO_STOP_PREDICATE_H
#define CGAL_SURFACE_MESH_SIMPLIFICATION_POLICIES_EDGE_COLLAPSE_COUNT_RATIO_STOP_PREDICATE_H

#include <CGAL/license/Surface_mesh_simplification.h>

#define CGAL_DEPRECATED_HEADER "<CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_predicate.h>"
#define CGAL_REPLACEMENT_HEADER "<CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_count_ratio_stop_predicate.h>"
#include <CGAL/Installation/internal/deprecation_warning.h>

#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_count_ratio_stop_predicate.h>

#ifndef CGAL_NO_DEPRECATED_CODE

namespace CGAL {
namespace Surface_mesh_simplification {

// Stops when the ratio of initial to current number of edges is below some value.
template<class TM_>
using Count_ratio_stop_predicate CGAL_DEPRECATED = Edge_count_ratio_stop_predicate<TM_>;

} // namespace Surface_mesh_simplification
} // namespace CGAL

#endif // CGAL_NO_DEPRECATED_CODE

#endif // CGAL_SURFACE_MESH_SIMPLIFICATION_POLICIES_EDGE_COLLAPSE_COUNT_RATIO_STOP_PREDICATE_H
