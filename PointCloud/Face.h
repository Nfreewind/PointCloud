#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Polygon_traits_2.h>
#include "PointProcess.h"
#include "Utils.h"

namespace pointcloud {

	class Face {
	public:
		int shape_id;
		std::vector<std::pair<glm::dvec3, glm::dvec3>> points;
		std::vector<std::vector<glm::dvec3>> triangles;
		glm::dvec3 normal;
		double d;
		double area;

	public:
		float distance(const glm::dvec3& p) {
			return std::abs(glm::dot(p, normal) + d);
		}

		float signedDistance(const glm::dvec3& p) {
			return glm::dot(p, normal) + d;
		}

		void update() {
			Kernel::Plane_3 plane(normal.x, normal.y, normal.z, d);

			std::vector<Kernel::Point_2> projected_points;
			for (int i = 0; i < points.size(); i++) {
				projected_points.push_back(plane.to_2d(Kernel::Point_3(points[i].first.x, points[i].first.y, points[i].first.z)));
			}

			try {
				util::clockwiseOrder(projected_points);
				std::vector<Kernel::Point_2> pts = util::boundingPolyg(projected_points);
				area = CGAL::Polygon_2<Kernel>(pts.begin(), pts.end()).area();
				std::vector<std::vector<Kernel::Point_2>> tessellated = util::tessellate(pts);

				triangles.resize(tessellated.size());
				for (int j = 0; j < tessellated.size(); j++) {
					triangles[j].resize(tessellated[j].size());
					for (int k = 0; k < tessellated[j].size(); k++) {
						Kernel::Point_3 p = plane.to_3d(Kernel::Point_2(tessellated[j][k].x(), tessellated[j][k].y()));
						triangles[j][k] = glm::dvec3(p.x(), p.y(), p.z());
					}

					// orient the vertices of the triangle in counter-clockwise order.
					if (glm::dot(normal, glm::cross(triangles[j][1] - triangles[j][0], triangles[j][2] - triangles[j][1])) < 0) {
						std::reverse(triangles[j].begin(), triangles[j].end());
					}
				}
			}
			catch (...) {}
		}
	};

}