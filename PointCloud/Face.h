#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "PointProcess.h"

namespace pointcloud {

	class Face {
	public:
		int shape_id;
		std::vector<glm::dvec3> points;
		std::vector<glm::dvec2> boundary;
		std::vector<std::vector<glm::dvec3>> triangles;
		glm::dvec3 normal;
		double d;
		double area;
	};

}