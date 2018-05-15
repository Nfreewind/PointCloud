#pragma once

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/read_xyz_points.h>
#include <CGAL/Point_with_normal_3.h>
#include <CGAL/property_map.h>
#include <CGAL/Timer.h>
#include <CGAL/number_utils.h>
#include <CGAL/Shape_detection_3.h>
#include <CGAL/Point_set_3.h>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <glm/glm.hpp>
#include "PointProcess.h"
#include "VerticalPlane.h"
#include "HorizontalPlane.h"
#include "Face.h"

namespace pointcloud {

	namespace shape {

		// Type declarations
		typedef Kernel::FT                                           FT;
		typedef std::pair<Kernel::Point_3, Kernel::Vector_3>         Point_with_normal;
		typedef std::vector<Point_with_normal>                       Pwn_vector;
		typedef CGAL::First_of_pair_property_map<Point_with_normal>  Point_map;
		typedef CGAL::Second_of_pair_property_map<Point_with_normal> Normal_map;

		// In Shape_detection_traits the basic types, i.e., Point and Vector types
		// as well as iterator type and property maps, are defined.
		typedef CGAL::Shape_detection_3::Shape_detection_traits<Kernel, Pwn_vector, Point_map, Normal_map> Traits;
		typedef CGAL::Shape_detection_3::Efficient_RANSAC<Traits>   Efficient_ransac;
		typedef VerticalPlane<Traits>								VPlane;
		typedef HorizontalPlane<Traits>								HPlane;

		typedef CGAL::Point_set_3<Kernel::Point_3> Point_set;

		void detect(std::vector<std::pair<glm::vec3, glm::vec3>>& point_cloud, double probability, int min_points, double epsilon, double cluster_epsilon, double normal_threshold, std::vector<Face>& faces);

	}
}