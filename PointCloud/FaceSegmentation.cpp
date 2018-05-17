#include "FaceSegmentation.h"
#include <algorithm>
#include <CGAL/intersections.h>
#include "Utils.h"

namespace pointcloud {

	namespace face_segmentation {

		std::vector<Face> segment(const std::vector<Face>& faces, float dilation_scale, float ratio_of_supporting_points_to_area) {
			std::vector<Face> new_faces;
			int shape_id = 1;

			for (int i = 0; i < faces.size(); i++) {
				Kernel::Plane_3 plane(faces[i].normal.x, faces[i].normal.y, faces[i].normal.z, faces[i].d);

				// calculate the 2D coordinates of the points on the plane
				std::vector<Point2> points_on_plane(faces[i].points.size());
				for (int j = 0; j < faces[i].points.size(); j++) {
					Point3 pt(faces[i].points[j].x, faces[i].points[j].y, faces[i].points[j].z);
					points_on_plane[j] = plane.to_2d(pt);
				}
				
				// calculate the enlarged boundary
				BoundingBox bbox = calculateBBox(points_on_plane);
				glm::vec2 center_pt = bbox.center();
				bbox.min_pt = center_pt + (bbox.min_pt - center_pt) * dilation_scale;
				bbox.max_pt = center_pt + (bbox.max_pt - center_pt) * dilation_scale;

				// create a large polygon and make it a single element of list
				PolygonWithHoles2 polygon;
				polygon.outer_boundary().push_back(Point2(bbox.min_pt.x, bbox.min_pt.y));
				polygon.outer_boundary().push_back(Point2(bbox.max_pt.x, bbox.min_pt.y));
				polygon.outer_boundary().push_back(Point2(bbox.max_pt.x, bbox.max_pt.y));
				polygon.outer_boundary().push_back(Point2(bbox.min_pt.x, bbox.max_pt.y));
				
				std::vector<PolygonWithHoles2> polygons;
				polygons.push_back(polygon);

				for (int j = 0; j < faces.size(); j++) {
					if (i == j) continue;

					Kernel::Plane_3 plane2(faces[j].normal.x, faces[j].normal.y, faces[j].normal.z, faces[j].d);

					CGAL::cpp11::result_of<Kernel::Intersect_3(Kernel::Plane_3, Kernel::Plane_3)>::type result = intersection(plane, plane2);
					if (!result) continue;

					if (const Kernel::Line_3* s = boost::get<Kernel::Line_3>(&*result)) {
						//std::cout << "point=" << s->point() << std::endl;
						//std::cout << "dir=" << s->direction() << std::endl;

						// normalize the direction vector of the intersection line
						Kernel::Vector_3 ndir(s->direction().dx(), s->direction().dy(), s->direction().dz());
						ndir /= ndir * ndir;

						// calculate an intersection line on the plane
						Point2 pt = plane.to_2d(s->point());
						Point2 pt1 = plane.to_2d(s->point() - ndir * 100000);
						Point2 pt2 = plane.to_2d(s->point() + ndir * 100000);

						// create large polygons that are connected via the intersection line
						Kernel::Vector_2 dir = pt2 - pt1;
						Kernel::Vector_2 perp(-dir.y(), dir.x());
						Polygon2 splitter_polygon1;
						splitter_polygon1.push_back(pt1);
						splitter_polygon1.push_back(pt2);
						splitter_polygon1.push_back(pt2 + perp);
						splitter_polygon1.push_back(pt1 + perp);
						Polygon2 splitter_polygon2;
						splitter_polygon2.push_back(pt2);
						splitter_polygon2.push_back(pt1);
						splitter_polygon2.push_back(pt1 - perp);
						splitter_polygon2.push_back(pt2 - perp);


						// use intersection to calculate intersection between polygons
						std::vector<PolygonWithHoles2> new_polygons;
						for (int k = 0; k < polygons.size(); k++) {
							std::vector<PolygonWithHoles2> intersection_polygons1;
							CGAL::intersection(splitter_polygon1, polygons[k], std::back_inserter(intersection_polygons1));
							new_polygons.insert(new_polygons.end(), intersection_polygons1.begin(), intersection_polygons1.end());

							std::vector<PolygonWithHoles2> intersection_polygons2;
							CGAL::intersection(splitter_polygon2, polygons[k], std::back_inserter(intersection_polygons2));
							new_polygons.insert(new_polygons.end(), intersection_polygons2.begin(), intersection_polygons2.end());
						}
						polygons = new_polygons;
					}
				}

				// remove polygonts that have too few supporting points
				for (int j = polygons.size() - 1; j >= 0; j--) {
					// calculate enlarged polygon
					PolygonWithHoles2 enlarged_polygon = dilatePolygon(polygons[j], dilation_scale);

					int cnt_points_inside = 0;
					for (auto& point_on_plane : points_on_plane) {
						if (isInside(point_on_plane, enlarged_polygon)) {
							cnt_points_inside++;
						}
					}

					// remove the polygon because there is no supporting point
					if (cnt_points_inside <= area(polygons[j]) * ratio_of_supporting_points_to_area) {
						polygons.erase(polygons.begin() + j);
					}
				}
				
				// update triangles of the face based on the remaining polygons
				/*
				faces[i].triangles.clear();
				for (int j = 0; j < polygons.size(); j++) {
					std::vector<std::vector<Point2>> triangles = tessellate(polygons[j]);

					std::vector<std::vector<glm::dvec3>> glm_triangles(triangles.size());
					for (int k = 0; k < triangles.size(); k++) {
						glm_triangles[k].resize(triangles[k].size());
						for (int l = 0; l < triangles[k].size(); l++) {
							Point3 pt3d = plane.to_3d(triangles[k][l]);
							glm_triangles[k][l] = glm::dvec3(CGAL::to_double(pt3d.x()), CGAL::to_double(pt3d.y()), CGAL::to_double(pt3d.z()));
						}
					}
					faces[i].triangles.insert(faces[i].triangles.end(), glm_triangles.begin(), glm_triangles.end());
				}
				*/
				for (int j = 0; j < polygons.size(); j++) {
					Face face;
					face.shape_id = shape_id++;
					face.normal = faces[i].normal;
					face.d = faces[i].d;
					for (auto& point_on_plane : points_on_plane) {
						if (isInside(point_on_plane, polygons[j])) {
							Point3 pt = plane.to_3d(point_on_plane);
							face.points.push_back(glm::vec3(CGAL::to_double(pt.x()), CGAL::to_double(pt.y()), CGAL::to_double(pt.z())));
						}
					}
					face.area = CGAL::to_double(area(polygons[j]));
					std::vector<std::vector<Point2>> triangles = tessellate(polygons[j]);
					face.triangles.resize(triangles.size());
					for (int k = 0; k < triangles.size(); k++) {
						face.triangles[k].resize(triangles[k].size());
						for (int l = 0; l < triangles[k].size(); l++) {
							Point3 pt3d = plane.to_3d(triangles[k][l]);
							face.triangles[k][l] = glm::dvec3(CGAL::to_double(pt3d.x()), CGAL::to_double(pt3d.y()), CGAL::to_double(pt3d.z()));
						}
					}
					new_faces.push_back(face);
				}
			}

			return new_faces;
		}

		bool isInside(const Point2& pt, const PolygonWithHoles2& polygon) {
			if (polygon.outer_boundary().bounded_side(pt) == CGAL::ON_UNBOUNDED_SIDE) return false;

			for (auto it = polygon.holes_begin(); it != polygon.holes_end(); it++) {
				if (it->bounded_side(pt) == CGAL::ON_BOUNDED_SIDE) return false;
			}

			return true;
		}

		Kernel::FT area(const PolygonWithHoles2& polygon) {
			Kernel::FT area = polygon.outer_boundary().area();
			for (auto it = polygon.holes_begin(); it != polygon.holes_end(); it++) {
				area -= it->area();
			}
			return area;
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

		BoundingBox calculateBBox(const std::vector<Point2>& points) {
			double min_x = (std::numeric_limits<float>::max)();
			double min_y = (std::numeric_limits<float>::max)();
			double max_x = -(std::numeric_limits<float>::max)();
			double max_y = -(std::numeric_limits<float>::max)();
			for (int i = 0; i < points.size(); i++) {
				min_x = std::min(min_x, CGAL::to_double(points[i].x()));
				min_y = std::min(min_y, CGAL::to_double(points[i].y()));
				max_x = std::max(max_x, CGAL::to_double(points[i].x()));
				max_y = std::max(max_y, CGAL::to_double(points[i].y()));
			}

			return BoundingBox(min_x, min_y, max_x, max_y);
		}

		/**
		 * Dilate a polygon by the specified scale.
		 */
		PolygonWithHoles2 dilatePolygon(const PolygonWithHoles2& polygon, double scale) {
			CGAL::Bbox_2 bbox = polygon.outer_boundary().bbox();
			Point2 center((bbox.xmin() + bbox.xmax()) * 0.5, (bbox.ymin() + bbox.ymax()) * 0.5);

			PolygonWithHoles2 ans;
			for (auto it = polygon.outer_boundary().vertices_begin(); it != polygon.outer_boundary().vertices_end(); it++) {
				ans.outer_boundary().push_back(Point2((it->x() - center.x()) * scale + center.x(), (it->y() - center.y()) * scale + center.y()));
			}

			return ans;
		}

	}

}