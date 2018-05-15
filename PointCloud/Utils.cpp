#include "Utils.h"
#include <iostream>
#include <list>
#include <random>

namespace pointcloud {
	
	namespace util {

		void createCubePoints(const std::string& filename, int num_points) {
			std::default_random_engine generator;
			std::uniform_real_distribution<double> distribution(0.0, 100.0);

			std::ofstream fout(filename);
			for (int iter = 0; iter < 60000; iter++) {
				int r = rand() % 6;
				double x, y, z, nx, ny, nz;
				switch (r) {
				case 0:
					x = distribution(generator);
					y = distribution(generator);
					z = 0;
					nx = ny = 0;
					nz = -1;
					break;
				case 1:
					x = distribution(generator);
					y = distribution(generator);
					z = 100;
					nx = ny = 0;
					nz = 1;
					break;
				case 2:
					x = 0;
					y = distribution(generator);
					z = distribution(generator);
					ny = nz = 0;
					nx = -1;
					break;
				case 3:
					x = 100;
					y = distribution(generator);
					z = distribution(generator);
					ny = nz = 0;
					nx = 1;
					break;
				case 4:
					x = distribution(generator);
					y = 0;
					z = distribution(generator);
					nx = nz = 0;
					ny = -1;
					break;
				case 5:
					x = distribution(generator);
					y = 100;
					z = distribution(generator);
					nx = nz = 0;
					ny = 1;
					break;
				}

				x += distribution(generator) * 0.01;
				y += distribution(generator) * 0.01;
				z += distribution(generator) * 0.01;

				double x2 = sqrt(3) / 2 * x - y / 2;
				double y2 = x / 2 + sqrt(3) / 2 * y;
				double nx2 = sqrt(3) / 2 * nx - ny / 2;
				double ny2 = nx / 2 + sqrt(3) / 2 * ny;

				fout << x2 << " " << y2 << " " << z << " " << nx2 << " " << ny2 << " " << nz << std::endl;
			}
			fout.close();
		}


		void clockwiseOrder(std::vector<Kernel::Point_2>& points) {
			float total = 0.0f;
			for (int i = 0; i < points.size(); i++) {
				int next = (i + 1) % points.size();
				total += (points[next].x() - points[i].x()) * (points[next].y() + points[i].y());
			}
			if (total > 0) {
				std::reverse(points.begin(), points.end());
			}
		}

		void counterClockwiseOrder(std::vector<Kernel::Point_2>& points) {
			float total = 0.0f;
			for (int i = 0; i < points.size(); i++) {
				int next = (i + 1) % points.size();
				total += (points[next].x() - points[i].x()) * (points[next].y() + points[i].y());
			}
			if (total < 0) {
				std::reverse(points.begin(), points.end());
			}
		}

		std::vector<Kernel::Point_2> boundingPolyg(const std::vector<Kernel::Point_2>& points) {
			// calculate the alpha shape of the points
			Alpha_shape_2 A(points.begin(), points.end(), FT(1000), Alpha_shape_2::GENERAL);
			std::vector<Segment> segments;
			alpha_edges(A, std::back_inserter(segments));

			std::map<std::pair<float, float>, int> pts_map;
			for (int i = 0; i < segments.size(); i++) {
				pts_map[{ segments[i].start().x(), segments[i].start().y() }] = i;
			}

			std::vector<Kernel::Point_2> pts;
			int start_index = 0;
			int cur_index = 0;
			while (true) {
				pts.push_back(Kernel::Point_2(segments[cur_index].start().x(), segments[cur_index].start().y()));
				cur_index = pts_map[{segments[cur_index].end().x(), segments[cur_index].end().y()}];
				if (cur_index == start_index) break;
			}

			return pts;
		}


		/**
		 * Triangulate the polygon with holes into triangles using the Delaunay triangulation.
		 * This function requires the polygon without any self-intersection. If the polygon has
		 * any self-intersection, this function will return no triangle.
		 *
		 * @param points	contour
		 * @param holes		holes
		 * @return			list of triangles
		 */
		std::vector<std::vector<Point2>> tessellate(const std::vector<Point2>& points) {
			std::vector<std::vector<Point2>> ans;

			// Insert the polygons into a constrained triangulation.
			CDT cdt;
			CGAL::Partition_traits_2<Kernel>::Polygon_2 polygon;
			for (int i = 0; i < points.size(); i++) {
				polygon.push_back(CDT::Point(points[i].x(), points[i].y()));
			}

			if (polygon.is_simple()) {
				cdt.insert_constraint(polygon.vertices_begin(), polygon.vertices_end(), true);

				//Mark facets that are inside the domain bounded by the polygon
				mark_domains(cdt);

				for (CDT::Finite_faces_iterator fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); ++fit) {
					if (fit->info().in_domain()) {
						std::vector<Point2> pol;
						for (int i = 0; i < 3; i++) {
							CDT::Vertex_handle vh = fit->vertex(i);
							pol.push_back(Point2(vh->point().x(), vh->point().y()));
						}

						ans.push_back(pol);
					}
				}
			}

			return ans;
		}

		/**
		* Triangulate the polygon with holes into triangles using the Delaunay triangulation.
		* This function requires the polygon without any self-intersection. If the polygon has
		* any self-intersection, this function will return no triangle.
		*
		* @param points	contour
		* @param holes		holes
		* @return			list of triangles
		*/
		std::vector<std::vector<Point2>> tessellate(const PolygonWithHoles2& polygon_with_holes) {
			std::vector<std::vector<Point2>> ans;

			// Insert the polygons into a constrained triangulation.
			CDT cdt;
			CGAL::Partition_traits_2<Kernel>::Polygon_2 polygon;
			for (auto it = polygon_with_holes.outer_boundary().vertices_begin(); it != polygon_with_holes.outer_boundary().vertices_end(); it++) {
				polygon.push_back(CDT::Point(it->x(), it->y()));
			}

			if (polygon.is_simple()) {
				cdt.insert_constraint(polygon.vertices_begin(), polygon.vertices_end(), true);
				for (auto it = polygon_with_holes.holes_begin(); it != polygon_with_holes.holes_end(); it++) {
					CGAL::Partition_traits_2<Kernel>::Polygon_2 hole;
					for (auto it2 = it->vertices_begin(); it2 != it->vertices_end(); it2++) {
						hole.push_back(CDT::Point(it2->x(), it2->y()));
					}
					cdt.insert_constraint(hole.vertices_begin(), hole.vertices_end(), true);
				}
				
				//Mark facets that are inside the domain bounded by the polygon
				mark_domains(cdt);

				for (CDT::Finite_faces_iterator fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); ++fit) {
					if (fit->info().in_domain()) {
						std::vector<Point2> pol;
						for (int i = 0; i < 3; i++) {
							CDT::Vertex_handle vh = fit->vertex(i);
							pol.push_back(Point2(vh->point().x(), vh->point().y()));
						}

						ans.push_back(pol);
					}
				}
			}

			return ans;
		}

		void mark_domains(CDT& ct, CDT::Face_handle start, int index, std::list<CDT::Edge>& border) {
			if (start->info().nesting_level != -1){
				return;
			}
			std::list<CDT::Face_handle> queue;
			queue.push_back(start);
			while (!queue.empty()){
				CDT::Face_handle fh = queue.front();
				queue.pop_front();
				if (fh->info().nesting_level == -1){
					fh->info().nesting_level = index;
					for (int i = 0; i < 3; i++){
						CDT::Edge e(fh, i);
						CDT::Face_handle n = fh->neighbor(i);
						if (n->info().nesting_level == -1){
							if (ct.is_constrained(e)) {
								border.push_back(e);


							}
							else queue.push_back(n);
						}
					}
				}
			}
		}

		//explore set of facets connected with non constrained edges,
		//and attribute to each such set a nesting level.
		//We start from facets incident to the infinite vertex, with a nesting
		//level of 0. Then we recursively consider the non-explored facets incident 
		//to constrained edges bounding the former set and increase the nesting level by 1.
		//Facets in the domain are those with an odd nesting level.
		void mark_domains(CDT& cdt) {
			for (CDT::All_faces_iterator it = cdt.all_faces_begin(); it != cdt.all_faces_end(); ++it){
				it->info().nesting_level = -1;
			}
			std::list<CDT::Edge> border;
			mark_domains(cdt, cdt.infinite_face(), 0, border);
			while (!border.empty()){
				CDT::Edge e = border.front();
				border.pop_front();
				CDT::Face_handle n = e.first->neighbor(e.second);
				if (n->info().nesting_level == -1){
					mark_domains(cdt, n, e.first->info().nesting_level + 1, border);
				}
			}
		}

	}

}
