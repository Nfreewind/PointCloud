#pragma once
#pragma warning(disable:4996)

#include <vector>
#include <map>
#include <tuple>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/read_xyz_points.h>
#include <CGAL/Point_with_normal_3.h>
#include <CGAL/property_map.h>
#include <CGAL/Timer.h>
#include <CGAL/number_utils.h>
#include <CGAL/Shape_detection_3.h>
#include <CGAL/Alpha_shape_2.h>
#include <CGAL/Alpha_shape_vertex_base_2.h>
#include <CGAL/Alpha_shape_face_base_2.h>
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
#include <glm/glm.hpp>
#include "PointProcess.h"

namespace pointcloud {

	namespace util {

		class BoundingBox {
		public:
			glm::vec2 min_pt;
			glm::vec2 max_pt;

		public:
			BoundingBox(float min_x, float min_y, float max_x, float max_y) : min_pt(min_x, min_y), max_pt(max_x, max_y) {}
			glm::vec2 center() {
				return glm::vec2((min_pt.x + max_pt.x) * 0.5, (min_pt.y + max_pt.y) * 0.5);
			}
		};

		// The following definitions are for triangulation only.
		struct FaceInfo {
			FaceInfo() {}
			int nesting_level;
			bool in_domain(){
				return nesting_level % 2 == 1;
			}
		};

		typedef Kernel::FT                                           FT;
		typedef Kernel::Segment_2                                    Segment;
		typedef CGAL::Alpha_shape_vertex_base_2<Kernel>              Alpha_Vb;
		typedef CGAL::Alpha_shape_face_base_2<Kernel>                Alpha_Fb;
		typedef CGAL::Triangulation_data_structure_2<Alpha_Vb, Alpha_Fb> Alpha_TDS;
		typedef CGAL::Delaunay_triangulation_2<Kernel, Alpha_TDS>        Alpha_Triangulation;
		typedef CGAL::Alpha_shape_2<Alpha_Triangulation>             Alpha_shape_2;
		typedef Alpha_shape_2::Alpha_shape_edges_iterator            Alpha_shape_edges_iterator;

		typedef CGAL::Triangulation_vertex_base_2<Kernel>                      Vb;
		typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo, Kernel>    Fbb;
		typedef CGAL::Constrained_triangulation_face_base_2<Kernel, Fbb>        Fb;
		typedef CGAL::Triangulation_data_structure_2<Vb, Fb>               TDS;
		typedef CGAL::Exact_predicates_tag                                Itag;
		typedef CGAL::Constrained_Delaunay_triangulation_2<Kernel, TDS, Itag>  CDT;


		template <class OutputIterator>
		void alpha_edges(const Alpha_shape_2& A, OutputIterator out) {
			Alpha_shape_edges_iterator it = A.alpha_shape_edges_begin(), end = A.alpha_shape_edges_end();
			for (; it != end; ++it) {
				*out++ = A.segment(*it);
			}
		}

		void createCubePoints(const std::string& filename, int num_points);

		BoundingBox calculateBBox(const std::vector<glm::dvec2>& points);

		void clockwiseOrder(std::vector<Kernel::Point_2>& points);
		void counterClockwiseOrder(std::vector<Kernel::Point_2>& points);
		std::vector<Kernel::Point_2> boundingPolyg(const std::vector<Kernel::Point_2>& points);

		// tessellation
		std::vector<std::vector<Point2>> tessellate(const std::vector<Point2>& points);
		std::vector<std::vector<Point2>> tessellate(const PolygonWithHoles2& points);
		void mark_domains(CDT& ct, CDT::Face_handle start, int index, std::list<CDT::Edge>& border);
		void mark_domains(CDT& cdt);

		void estimateNormals(std::vector<std::pair<glm::dvec3, glm::dvec3>>& points);

	}

}
