#include "PointSetShapeDetection.h"
#include "Utils.h"

namespace pointcloud {

	namespace shape {

		void detect(std::vector<std::pair<glm::dvec3, glm::dvec3>>& point_cloud, double probability, int min_points, double epsilon, double cluster_epsilon, double normal_threshold, std::vector<Face>& faces) {
			// Points with normals.
			Pwn_vector points;
			for (int i = 0; i < point_cloud.size(); i++) {
				glm::vec3 pos = point_cloud[i].first;
				glm::vec3 normal = point_cloud[i].second;
				points.push_back({ Kernel::Point_3(pos.x, pos.y, pos.z), Kernel::Vector_3(normal.x, normal.y, normal.z) });
			}

			// Instantiates shape detection engine.
			Efficient_ransac ransac;

			// Provides the input data.
			ransac.set_input(points);

			// Registers detection of planes
			//ransac.add_shape_factory<Plane>();
			ransac.add_shape_factory<VPlane>();
			ransac.add_shape_factory<HPlane>();

			// Sets parameters for shape detection.
			Efficient_ransac::Parameters parameters;	// probability to miss the largest primitive at each iteration.
			parameters.probability = probability;
			parameters.min_points = min_points;
			parameters.epsilon = epsilon;	// maximum Euclidean distance between a point and a shape.
			parameters.cluster_epsilon = cluster_epsilon;	// maximum Euclidean distance between points to be clustered.
			parameters.normal_threshold = normal_threshold;	// maximum normal deviation.

			// Detects shapes.
			ransac.detect(parameters);

			// Efficient_ransac::shapes() provides
			// an iterator range to the detected shapes. 
			Efficient_ransac::Shape_range shapes = ransac.shapes();

			faces.clear();

			int shape_id = 1;
			for (auto& it = shapes.begin(); it != shapes.end(); it++, shape_id++) {
				boost::shared_ptr<Efficient_ransac::Shape> shape = *it;

				Face face;
				face.shape_id = shape_id;

				// Get specific parameters depending on detected shape.
				if (VPlane* plane = dynamic_cast<VPlane*>(shape.get())) {
					face.normal = glm::dvec3(plane->plane_normal().x(), plane->plane_normal().y(), plane->plane_normal().z());
					face.d = plane->d();
				}
				else if (HPlane* plane = dynamic_cast<HPlane*>(shape.get())) {
					face.normal = glm::dvec3(plane->plane_normal().x(), plane->plane_normal().y(), plane->plane_normal().z());
					face.d = plane->d();
				}

				int cnt_same_normal_direction = 0;

				// Iterates through point indices assigned to each detected shape.
				for (auto& index_it = (*it)->indices_of_assigned_points().begin(); index_it != (*it)->indices_of_assigned_points().end(); index_it++) {
					// Retrieves point
					Point_with_normal &p = *(points.begin() + (*index_it));
					face.points.push_back(glm::dvec3(p.first.x(), p.first.y(), p.first.z()));

					if (p.second * Kernel::Vector_3(face.normal.x, face.normal.y, face.normal.z) >= 0) cnt_same_normal_direction++;
				}

				if (cnt_same_normal_direction < (*it)->indices_of_assigned_points().size() * 0.5) {
					face.normal = -face.normal;
					face.d = -face.d;
				}

				face.update();
				faces.push_back(face);
			}

			for (int i = 0; i < points.size(); i++) {
				glm::vec3 pos(points[i].first.x(), points[i].first.y(), points[i].first.z());
				glm::vec3 normal(points[i].second.x(), points[i].second.y(), points[i].second.z());
				point_cloud[i] = std::make_pair(pos, normal);
			}
		}

	}

}