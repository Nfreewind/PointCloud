#pragma once

#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/algorithm.h>
#include <CGAL/assertions.h>

#include <CGAL/Partition_traits_2.h>
#include <CGAL/partition_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_polygon_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/create_offset_polygons_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/random_polygon_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include "Face.h"

namespace pointcloud {

	namespace face_segmentation {
		
		// The following definitions are for triangulation only.
		struct FaceInfo {
			FaceInfo() {}
			int nesting_level;
			bool in_domain(){
				return nesting_level % 2 == 1;
			}
		};

		// Type declarations
		typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
		typedef CGAL::Point_2<Kernel> Point2;
		typedef CGAL::Point_3<Kernel> Point3;
		typedef CGAL::Polygon_2<Kernel> Polygon2;
		typedef CGAL::Polygon_with_holes_2<Kernel> PolygonWithHoles2;

		typedef Kernel::FT                                           FT;
		typedef Kernel::Segment_2                                    Segment;

		typedef CGAL::Triangulation_vertex_base_2<Kernel>                      Vb;
		typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo, Kernel>    Fbb;
		typedef CGAL::Constrained_triangulation_face_base_2<Kernel, Fbb>        Fb;
		typedef CGAL::Triangulation_data_structure_2<Vb, Fb>               TDS;
		typedef CGAL::Exact_predicates_tag                                Itag;
		typedef CGAL::Constrained_Delaunay_triangulation_2<Kernel, TDS, Itag>  CDT;

		void segment(std::vector<Face>& faces);
		bool isInside(const Point2& pt, const PolygonWithHoles2& polygon);
		Kernel::FT area(const PolygonWithHoles2& polygon);

		std::vector<std::vector<Point2>> tessellate(const PolygonWithHoles2& points);
		void mark_domains(CDT& ct, CDT::Face_handle start, int index, std::list<CDT::Edge>& border);
		void mark_domains(CDT& cdt);

	};

}