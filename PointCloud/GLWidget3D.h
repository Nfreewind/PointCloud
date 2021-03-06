#pragma once

#include <glew.h>
#include "Shader.h"
#include <QGLWidget>
#include <QMouseEvent>
#include "Camera.h"
#include "RenderManager.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "GLUtils.h"
#include "PointSetShapeDetection.h"

class MainWindow;

class GLWidget3D : public QGLWidget {
	Q_OBJECT

public:
	enum { FACE_RANDOM_COLOR = 0, FACE_SAME_COLOR };

public:
	MainWindow* mainWin;

	// camera
	Camera camera;
	glm::vec3 light_dir;
	glm::mat4 light_mvpMatrix;
	glm::vec3 spot_light_pos;

	// input voxel data
	std::vector<std::pair<glm::dvec3, glm::dvec3>> point_cloud;
	std::vector<pointcloud::Face> detected_faces;
	std::vector<pointcloud::Face> segmented_faces;

	// rendering engine
	RenderManager renderManager;

	int face_coloring;
	bool show_points;
	bool show_faces;
	bool face_detected;
	bool face_segmented;
	
	// key status
	bool shiftPressed;
	bool ctrlPressed;

	bool first_paint;

	// face selection
	int _selectedFace = -1;
	std::vector<std::vector<pointcloud::Face>> detected_faces_history;
	std::vector<std::vector<pointcloud::Face>> segmented_faces_history;

public:
	GLWidget3D(MainWindow *parent = 0);

	void drawScene();
	void render();
	int loadVoxelData(const QString& filename);
	void convertVDB2PointCloud(std::vector<cv::Mat_<uchar>>& voxel_data, std::vector<std::pair<glm::dvec3, glm::dvec3>>& point_cloud, int threshold, double voxel_size);
	void saveVG(const QString& filename);
	void detect(double probability, double min_points, double epsilon, double cluster_epsilon, double normal_threshold);
	void segment(double dilation_scale, double ratio_of_supporting_points_to_area);
	int downSample(double ratio);
	double uniform_rand(double a, double b);
	glm::vec4 getColor(int index);
	glm::vec3 viewVector(const glm::vec2& point, const glm::mat4& mvMatrix, float focalLength, float aspect);
	bool rayTriangleIntersection(const glm::vec3& a, const glm::vec3& v, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, glm::vec3& intPt);
	int selectFace(const glm::vec3& cameraPos, const glm::vec3& viewDir);
	void deleteFace();
	void undo();
	void printDetectedFaces();
	void saveImage(const QString& filename);
	void update3DGeometry();
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent* e);
};
