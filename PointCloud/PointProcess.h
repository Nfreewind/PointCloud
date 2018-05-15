#pragma once

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <iostream>
#include <vector>

namespace pointcloud {

	// Type declarations
	typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
	typedef CGAL::Point_2<Kernel> Point2;
	typedef CGAL::Point_3<Kernel> Point3;
	typedef CGAL::Polygon_2<Kernel> Polygon2;
	typedef CGAL::Polygon_with_holes_2<Kernel> PolygonWithHoles2;


}