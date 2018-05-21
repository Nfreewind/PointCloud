#include "GLWidget3D.h"
#include "MainWindow.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <QDir>
#include <QTextStream>
#include <QDate>
#include <iostream>
#include <random>
#include <QProcess>
#include "FaceSegmentation.h"
#include "Utils.h"

GLWidget3D::GLWidget3D(MainWindow *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers)) {
	this->mainWin = parent;
	ctrlPressed = false;
	shiftPressed = false;

	first_paint = true;

	// This is necessary to prevent the screen overdrawn by OpenGL
	setAutoFillBackground(false);

	// light direction for shadow mapping
	light_dir = glm::normalize(glm::vec3(-1, 1, -3));

	// model/view/projection matrices for shadow mapping
	glm::mat4 light_pMatrix = glm::ortho<float>(-400, 400, -400, 400, 0.1, 800);
	glm::mat4 light_mvMatrix = glm::lookAt(-light_dir * 300.0f, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	light_mvpMatrix = light_pMatrix * light_mvMatrix;

	// spot light
	spot_light_pos = glm::vec3(20, 25, 30);

	face_coloring = FACE_RANDOM_COLOR;
	show_points = true;
	show_faces = false;
	face_detected = false;
	face_segmented = false;
}

/**
* Draw the scene.
*/
void GLWidget3D::drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(true);

	renderManager.renderAll();
}

void GLWidget3D::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PASS 1: Render to texture
	glUseProgram(renderManager.programs["pass1"]);

	glBindFramebuffer(GL_FRAMEBUFFER, renderManager.fragDataFB);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderManager.fragDataTex[0], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, renderManager.fragDataTex[1], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, renderManager.fragDataTex[2], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, renderManager.fragDataTex[3], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderManager.fragDepthTex, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, DrawBuffers); // "3" is the size of DrawBuffers
	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("+ERROR: GL_FRAMEBUFFER_COMPLETE false\n");
		exit(0);
	}

	glUniformMatrix4fv(glGetUniformLocation(renderManager.programs["pass1"], "mvpMatrix"), 1, false, &camera.mvpMatrix[0][0]);
	glUniform3f(glGetUniformLocation(renderManager.programs["pass1"], "lightDir"), light_dir.x, light_dir.y, light_dir.z);
	glUniformMatrix4fv(glGetUniformLocation(renderManager.programs["pass1"], "light_mvpMatrix"), 1, false, &light_mvpMatrix[0][0]);
	glUniform3f(glGetUniformLocation(renderManager.programs["pass1"], "spotLightPos"), spot_light_pos.x, spot_light_pos.y, spot_light_pos.z);
	glUniform3f(glGetUniformLocation(renderManager.programs["pass1"], "cameraPos"), camera.pos.x, camera.pos.y, camera.pos.z);

	glUniform1i(glGetUniformLocation(renderManager.programs["pass1"], "shadowMap"), 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, renderManager.shadow.textureDepth);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	drawScene();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PASS 2: Create AO
	if (renderManager.renderingMode == RenderManager::RENDERING_MODE_SSAO) {
		glUseProgram(renderManager.programs["ssao"]);
		glBindFramebuffer(GL_FRAMEBUFFER, renderManager.fragDataFB_AO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderManager.fragAOTex, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderManager.fragDepthTex_AO, 0);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			printf("++ERROR: GL_FRAMEBUFFER_COMPLETE false\n");
			exit(0);
		}

		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);

		glUniform2f(glGetUniformLocation(renderManager.programs["ssao"], "pixelSize"), 2.0f / this->width(), 2.0f / this->height());

		glUniform1i(glGetUniformLocation(renderManager.programs["ssao"], "tex0"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[0]);

		glUniform1i(glGetUniformLocation(renderManager.programs["ssao"], "tex1"), 2);
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[1]);

		glUniform1i(glGetUniformLocation(renderManager.programs["ssao"], "tex2"), 3);
		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[2]);

		glUniform1i(glGetUniformLocation(renderManager.programs["ssao"], "depthTex"), 8);
		glActiveTexture(GL_TEXTURE8);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDepthTex);

		glUniform1i(glGetUniformLocation(renderManager.programs["ssao"], "noiseTex"), 7);
		glActiveTexture(GL_TEXTURE7);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragNoiseTex);

		{
			glUniformMatrix4fv(glGetUniformLocation(renderManager.programs["ssao"], "mvpMatrix"), 1, false, &camera.mvpMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(renderManager.programs["ssao"], "pMatrix"), 1, false, &camera.pMatrix[0][0]);
		}

		glUniform1i(glGetUniformLocation(renderManager.programs["ssao"], "uKernelSize"), renderManager.uKernelSize);
		glUniform3fv(glGetUniformLocation(renderManager.programs["ssao"], "uKernelOffsets"), renderManager.uKernelOffsets.size(), (const GLfloat*)renderManager.uKernelOffsets.data());

		glUniform1f(glGetUniformLocation(renderManager.programs["ssao"], "uPower"), renderManager.uPower);
		glUniform1f(glGetUniformLocation(renderManager.programs["ssao"], "uRadius"), renderManager.uRadius);

		glBindVertexArray(renderManager.secondPassVAO);

		glDrawArrays(GL_QUADS, 0, 4);
		glBindVertexArray(0);
		glDepthFunc(GL_LEQUAL);
	}
	else if (renderManager.renderingMode == RenderManager::RENDERING_MODE_LINE || renderManager.renderingMode == RenderManager::RENDERING_MODE_HATCHING) {
		glUseProgram(renderManager.programs["line"]);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);

		glUniform2f(glGetUniformLocation(renderManager.programs["line"], "pixelSize"), 1.0f / this->width(), 1.0f / this->height());
		glUniformMatrix4fv(glGetUniformLocation(renderManager.programs["line"], "pMatrix"), 1, false, &camera.pMatrix[0][0]);
		if (renderManager.renderingMode == RenderManager::RENDERING_MODE_HATCHING) {
			glUniform1i(glGetUniformLocation(renderManager.programs["line"], "useHatching"), 1);
		}
		else {
			glUniform1i(glGetUniformLocation(renderManager.programs["line"], "useHatching"), 0);
		}

		glUniform1i(glGetUniformLocation(renderManager.programs["line"], "tex0"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[0]);

		glUniform1i(glGetUniformLocation(renderManager.programs["line"], "tex1"), 2);
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[1]);

		glUniform1i(glGetUniformLocation(renderManager.programs["line"], "tex2"), 3);
		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[2]);

		glUniform1i(glGetUniformLocation(renderManager.programs["line"], "tex3"), 4);
		glActiveTexture(GL_TEXTURE4);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[3]);

		glUniform1i(glGetUniformLocation(renderManager.programs["line"], "depthTex"), 8);
		glActiveTexture(GL_TEXTURE8);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDepthTex);

		glUniform1i(glGetUniformLocation(renderManager.programs["line"], "hatchingTexture"), 5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_3D, renderManager.hatchingTextures);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindVertexArray(renderManager.secondPassVAO);

		glDrawArrays(GL_QUADS, 0, 4);
		glBindVertexArray(0);
		glDepthFunc(GL_LEQUAL);
	}
	else if (renderManager.renderingMode == RenderManager::RENDERING_MODE_CONTOUR) {
		glUseProgram(renderManager.programs["contour"]);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);

		glUniform2f(glGetUniformLocation(renderManager.programs["contour"], "pixelSize"), 1.0f / this->width(), 1.0f / this->height());

		glUniform1i(glGetUniformLocation(renderManager.programs["contour"], "depthTex"), 8);
		glActiveTexture(GL_TEXTURE8);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDepthTex);

		glBindVertexArray(renderManager.secondPassVAO);

		glDrawArrays(GL_QUADS, 0, 4);
		glBindVertexArray(0);
		glDepthFunc(GL_LEQUAL);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Blur

	if (renderManager.renderingMode == RenderManager::RENDERING_MODE_BASIC || renderManager.renderingMode == RenderManager::RENDERING_MODE_SSAO) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);

		glUseProgram(renderManager.programs["blur"]);
		glUniform2f(glGetUniformLocation(renderManager.programs["blur"], "pixelSize"), 2.0f / this->width(), 2.0f / this->height());
		//printf("pixelSize loc %d\n", glGetUniformLocation(vboRenderManager.programs["blur"], "pixelSize"));

		glUniform1i(glGetUniformLocation(renderManager.programs["blur"], "tex0"), 1);//COLOR
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[0]);

		glUniform1i(glGetUniformLocation(renderManager.programs["blur"], "tex1"), 2);//NORMAL
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[1]);

		/*glUniform1i(glGetUniformLocation(renderManager.programs["blur"], "tex2"), 3);
		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDataTex[2]);*/

		glUniform1i(glGetUniformLocation(renderManager.programs["blur"], "depthTex"), 8);
		glActiveTexture(GL_TEXTURE8);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragDepthTex);

		glUniform1i(glGetUniformLocation(renderManager.programs["blur"], "tex3"), 4);//AO
		glActiveTexture(GL_TEXTURE4);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderManager.fragAOTex);

		if (renderManager.renderingMode == RenderManager::RENDERING_MODE_SSAO) {
			glUniform1i(glGetUniformLocation(renderManager.programs["blur"], "ssao_used"), 1); // ssao used
		}
		else {
			glUniform1i(glGetUniformLocation(renderManager.programs["blur"], "ssao_used"), 0); // no ssao
		}

		glBindVertexArray(renderManager.secondPassVAO);

		glDrawArrays(GL_QUADS, 0, 4);
		glBindVertexArray(0);
		glDepthFunc(GL_LEQUAL);

	}

	// REMOVE
	glActiveTexture(GL_TEXTURE0);
}

int GLWidget3D::loadVoxelData(const QString& filename) {
	// get directory
	QDir dir = QFileInfo(filename).absoluteDir();

	// scan all the files in the directory to get a voxel data
	QStringList files = dir.entryList(QDir::NoDotAndDotDot | QDir::Files, QDir::DirsFirst);
	std::vector<cv::Mat_<uchar>> voxel_data(files.size());
	for (int i = 0; i < files.size(); i++) {
		voxel_data[i] = cv::imread((dir.absolutePath() + "/" + files[i]).toUtf8().constData(), cv::IMREAD_GRAYSCALE);
	}

	convertVDB2PointCloud(voxel_data, point_cloud, 127, 0.3);
	pointcloud::util::estimateNormals(point_cloud);

	detected_faces.clear();
	segmented_faces.clear();
	detected_faces_history.clear();
	segmented_faces_history.clear();
	show_points = true;
	show_faces = false;
	face_detected = false;
	face_segmented = false;
	mainWin->ui.actionShowPoints->setChecked(true);
	mainWin->ui.actionShowFaces->setChecked(false);

	update3DGeometry();

	return point_cloud.size();
}

void GLWidget3D::convertVDB2PointCloud(std::vector<cv::Mat_<uchar>>& voxel_data, std::vector<std::pair<glm::dvec3, glm::dvec3>>& point_cloud, int threshold, double voxel_size) {
	point_cloud.clear();

	for (int z = 0; z < voxel_data.size(); z++) {		for (int r = 0; r < voxel_data[z].rows; r++) {			for (int c = 0; c < voxel_data[z].cols; c++) {				if (voxel_data[z](r, c) < threshold) continue;				int cnt = 0;				for (int zz = -1; zz <= 1; zz++) {					for (int rr = -1; rr <= 1; rr++) {						for (int cc = -1; cc <= 1; cc++) {							int r2 = r + rr;							int c2 = c + cc;							int z2 = z + zz;							if (z2 < 0 || z2 >= voxel_data.size() || r2 < 0 || r2 >= voxel_data[z].rows || c2 < 0 || c2 >= voxel_data[z].cols) cnt++;							else if (voxel_data[z2](r2, c2) < threshold) cnt++;						}					}				}				if (cnt == 0) continue;								double nx = 0;				double ny = 0;				double nz = 0;				if (z == 0 || voxel_data[z - 1](r, c) < threshold) nz--;				if (z == voxel_data.size() - 1 || voxel_data[z + 1](r, c) < threshold) nz++;				if (r == 0 || voxel_data[z](r - 1, c) < threshold) ny--;				if (r == voxel_data[z].rows - 1 || voxel_data[z](r + 1, c) < threshold) ny++;				if (c == 0 || voxel_data[z](r, c - 1) < threshold) nx--;				if (c == voxel_data[z].cols - 1 || voxel_data[z](r, c + 1) < threshold) nx++;				if (nx == 0 && ny == 0 && nz == 0) nx = ny = nz = 1;				double len = sqrt(nx * nx + ny * ny + nz * nz);				nx /= len;				ny /= len;				nz /= len;				glm::dvec3 pos((uniform_rand(c - 0.2, c + 0.2) - voxel_data[0].cols * 0.5) * voxel_size, (voxel_data[0].rows * 0.5 - uniform_rand(r - 0.2, r + 0.2)) * voxel_size, uniform_rand(z - 0.2, z + 0.2) * voxel_size);				glm::dvec3 normal(nx, -ny, nz);				point_cloud.push_back(std::make_pair(pos, normal));			}		}	}
}

void GLWidget3D::saveVG(const QString& filename) {
	int total_points = 0;
	for (int i = 0; i < detected_faces.size(); i++) {
		total_points += detected_faces[i].points.size();
	}

	QFile file(filename);
	file.open(QIODevice::WriteOnly);
	QTextStream out(&file);

	// write out coordinates of points
	out << "num_points: " << total_points << endl;
	for (int i = 0; i < detected_faces.size(); i++) {
		for (int j = 0; j < detected_faces[i].points.size(); j++) {
			out << detected_faces[i].points[j].first.x << " " << detected_faces[i].points[j].first.y << " " << detected_faces[i].points[j].first.z << endl;
		}
	}

	// write out colors of points
	out << "num_colors: " << total_points << endl;
	for (int i = 0; i < detected_faces.size(); i++) {
		glm::vec4 color = getColor(detected_faces[i].shape_id);
		for (int j = 0; j < detected_faces[i].points.size(); j++) {
			out << color.r << " " << color.g << " " << color.b << endl;
		}
	}

	// write out normals of points
	out << "num_points: " << total_points << endl;
	for (int i = 0; i < detected_faces.size(); i++) {
		for (int j = 0; j < detected_faces[i].points.size(); j++) {
			out << detected_faces[i].points[j].second.x << " " << detected_faces[i].points[j].second.y << " " << detected_faces[i].points[j].second.z << endl;
		}
	}

	// write out faces
	out << "num_groups: " << detected_faces.size() << endl;
	int point_idx = 0;
	for (int i = 0; i < detected_faces.size(); i++) {
		glm::vec4 color = getColor(detected_faces[i].shape_id);

		out << "group_type: 0" << endl;
		out << "num_group_parameters: 4" << endl;
		out << "group_parameters: " << detected_faces[i].normal.x << " " << detected_faces[i].normal.y << " " << detected_faces[i].normal.z << " " << detected_faces[i].d << endl;
		out << "group_label: unknown" << endl;
		out << "group_color: " << color.r << " " << color.g << " " << color.b << endl;
		out << "group_num_points: " << detected_faces[i].points.size() << endl;

		for (int j = 0; j < detected_faces[i].points.size(); j++) {
			if (j > 0) out << " ";
			out << point_idx++;
		}
		out << endl;

		out << "num_children: 0" << endl;
	}

	file.close();
}

void GLWidget3D::detect(double probability, double min_points, double epsilon, double cluster_epsilon, double normal_threshold) {
	double min_x = std::numeric_limits<double>::max();
	double min_y = std::numeric_limits<double>::max();
	double min_z = std::numeric_limits<double>::max();
	double max_x = -std::numeric_limits<double>::max();
	double max_y = -std::numeric_limits<double>::max();
	double max_z = -std::numeric_limits<double>::max();
	for (int i = 0; i < point_cloud.size(); i++) {
		min_x = std::min(min_x, point_cloud[i].first.x);
		min_y = std::min(min_y, point_cloud[i].first.y);
		min_z = std::min(min_z, point_cloud[i].first.z);
		max_x = std::max(max_x, point_cloud[i].first.x);
		max_y = std::max(max_y, point_cloud[i].first.y);
		max_z = std::max(max_z, point_cloud[i].first.z);
	}
	double diagonal = std::sqrt((max_x - min_x) * (max_x - min_x) + (max_y - min_y) * (max_y - min_y) + (max_z - min_z) * (max_z - min_z));

	pointcloud::shape::detect(point_cloud, probability, point_cloud.size() * 0.01 * min_points, diagonal * 0.01 * epsilon, diagonal * 0.01 * cluster_epsilon, normal_threshold, detected_faces);
	std::cout << detected_faces.size() << " faces were detected." << std::endl;
	detected_faces_history.clear();

	segmented_faces.clear();
	segmented_faces_history.clear();
	face_detected = true;
	face_segmented = false;
	show_points = false;
	mainWin->ui.actionShowPoints->setChecked(false);
	show_faces = true;
	mainWin->ui.actionShowFaces->setChecked(true);
	_selectedFace = -1;
	update3DGeometry();
}

void GLWidget3D::segment(double dilation_scale, double ratio_of_supporting_points_to_area) {
	if (face_detected) {
		segmented_faces = pointcloud::face_segmentation::segment(detected_faces, dilation_scale, ratio_of_supporting_points_to_area);

		face_segmented = true;
		show_faces = true;
		mainWin->ui.actionShowFaces->setChecked(true);
		_selectedFace = -1;

		update3DGeometry();
	}
}

int GLWidget3D::downSample(double ratio) {
	std::shuffle(point_cloud.begin(), point_cloud.end(), std::default_random_engine(0));
	point_cloud.resize(point_cloud.size() * ratio);

	if (!face_detected) update3DGeometry();

	return point_cloud.size();
}

double GLWidget3D::uniform_rand(double a, double b) {
	double r = (float)(rand() % 1000) / 1000;
	return (b - a) * r + a;
}

glm::vec4 GLWidget3D::getColor(int index) {
	if (face_coloring == FACE_RANDOM_COLOR) {
		glm::vec4 color;
		color.x = ((index * 313497) % 101) * 0.002 + 0.8;
		color.y = ((index * 460261) % 101) * 0.002 + 0.8;
		color.z = ((index * 282563) % 101) * 0.002 + 0.8;

		return color;
	}
	else {
		return glm::vec4(0.8, 1, 0.8, 1);
	}
}

glm::vec3 GLWidget3D::viewVector(const glm::vec2& point, const glm::mat4& mvMatrix, float focalLength, float aspect) {
	glm::vec3 dir((point.x - width() * 0.5f) * 2.0f / width() * aspect, (height() * 0.5f - point.y) * 2.0f / height(), -focalLength);
	return glm::vec3(glm::inverse(mvMatrix) * glm::vec4(dir, 0));
}

/**
* Ray-Triangle intersection
*/
bool GLWidget3D::rayTriangleIntersection(const glm::vec3& a, const glm::vec3& v, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, glm::vec3& intPt) {
	glm::vec3 n = glm::cross(p2 - p1, p3 - p1);
	intPt = a + v * glm::dot(p1 - a, n) / glm::dot(v, n);

	cv::Mat_<double> L(3, 1);
	L(0, 0) = (intPt - p1).x;
	L(1, 0) = (intPt - p1).y;
	L(2, 0) = (intPt - p1).z;
	cv::Mat_<double> R(3, 2);
	R(0, 0) = (p2 - p1).x;
	R(1, 0) = (p2 - p1).y;
	R(2, 0) = (p2 - p1).z;
	R(0, 1) = (p3 - p1).x;
	R(1, 1) = (p3 - p1).y;
	R(2, 1) = (p3 - p1).z;

	cv::Mat_<double> st = R.inv(cv::DECOMP_SVD) * L;

	if (st(0, 0) >= 0 && st(1, 0) >= 0 && st(0, 0) + st(1, 0) <= 1) return true;
	else return false;
}

int GLWidget3D::selectFace(const glm::vec3& cameraPos, const glm::vec3& viewDir) {
	if (!show_faces) return -1;

	glm::vec3 intPt;
	float min_dist = (std::numeric_limits<float>::max)();

	_selectedFace = -1;

	if (face_segmented) {
		for (int i = 0; i < segmented_faces.size(); i++) {
			if (segmented_faces[i].triangles.size() < 1) continue;
			for (int j = 0; j < segmented_faces[i].triangles.size(); j++) {
				if (rayTriangleIntersection(cameraPos, viewDir, segmented_faces[i].triangles[j][0], segmented_faces[i].triangles[j][1], segmented_faces[i].triangles[j][2], intPt)) {
					float dist = glm::length(intPt - cameraPos);

					if (dist < min_dist) {
						min_dist = dist;
						_selectedFace = i;
					}
				}

			}
		}
	}
	else if (face_detected) {
		for (int i = 0; i < detected_faces.size(); i++) {
			if (detected_faces[i].triangles.size() < 1) continue;
			for (int j = 0; j < detected_faces[i].triangles.size(); j++) {
				if (rayTriangleIntersection(cameraPos, viewDir, detected_faces[i].triangles[j][0], detected_faces[i].triangles[j][1], detected_faces[i].triangles[j][2], intPt)) {
					float dist = glm::length(intPt - cameraPos);

					if (dist < min_dist) {
						min_dist = dist;
						_selectedFace = i;
					}
				}

			}
		}
	}

	return _selectedFace;
}

void GLWidget3D::deleteFace() {
	if (_selectedFace != -1) {
		if (face_segmented) {
			segmented_faces_history.push_back(segmented_faces);
			segmented_faces.erase(segmented_faces.begin() + _selectedFace);
			_selectedFace = -1;
			update3DGeometry();
			update();
		}
		else if (face_detected) {
			detected_faces_history.push_back(detected_faces);
			detected_faces.erase(detected_faces.begin() + _selectedFace);
			_selectedFace = -1;
			update3DGeometry();
			update();
		}
	}
}

void GLWidget3D::undo() {
	if (face_segmented) {
		if (segmented_faces_history.size() > 0) {
			segmented_faces = segmented_faces_history.back();
			segmented_faces_history.pop_back();
		}
		else {
			face_segmented = false;
		}
		update3DGeometry();
		update();
	}
	else if (face_detected) {
		if (detected_faces_history.size() > 0) {
			detected_faces = detected_faces_history.back();
			detected_faces_history.pop_back();
		}
		else {
			face_detected = false;
		}
		update3DGeometry();
		update();
	}
}

void GLWidget3D::printDetectedFaces() {
	std::cout << "-----------------------------------------------------------------" << std::endl;
	for (int i = 0; i < detected_faces.size(); i++) {
		std::cout << "Face " << detected_faces[i].shape_id << std::endl;
		std::cout << "   Normal = (" << detected_faces[i].normal.x << "," << detected_faces[i].normal.y << "," << detected_faces[i].normal.z << ")" << std::endl;
		std::cout << "   #points = " << detected_faces[i].points.size() << std::endl;
		std::cout << "   Area = " << detected_faces[i].area << std::endl;
		std::cout << "   Density = " << detected_faces[i].points.size() / detected_faces[i].area << std::endl;
	}
	std::cout << "-----------------------------------------------------------------" << std::endl;
}

void GLWidget3D::saveImage(const QString& filename) {
	QImage image = grabFrameBuffer();
	image.save(filename);
}

void GLWidget3D::update3DGeometry() {
	renderManager.removeObjects();

	if (!face_detected) {
		std::vector<Vertex> vertices;
		for (int i = 0; i < point_cloud.size(); i++) {
			glm::vec3 pos(point_cloud[i].first);
			glm::vec4 color = glm::vec4(1, 1, 1, 1);

			glutils::drawBox(0.3, 0.3, 0.3, color, glm::translate(glm::mat4(), pos), vertices);
		}
		renderManager.addObject("point_cloud", "", vertices, true);
	}
	else {
		if (face_segmented) {
			for (int i = 0; i < segmented_faces.size(); i++) {
				glm::vec4 color = getColor(segmented_faces[i].shape_id);
				if (i == _selectedFace) color = glm::vec4(0, 0, 1, 1);

				if (show_points) {
					std::vector<Vertex> vertices;
					for (int j = 0; j < segmented_faces[i].points.size(); j++) {
						glm::vec3 pos(segmented_faces[i].points[j].first);
						glutils::drawBox(0.3, 0.3, 0.3, color, glm::translate(glm::mat4(), pos), vertices);
					}
					renderManager.addObject("point_cloud", "", vertices, true);
				}

				if (show_faces) {
					std::vector<Vertex> vertices;
					for (auto& triangle : segmented_faces[i].triangles) {
						glutils::drawPolygon(triangle, color, glm::mat4(), vertices);
					}
					renderManager.addObject("face", "", vertices, true);
				}
			}
		}
		else if (face_detected) {
			for (int i = 0; i < detected_faces.size(); i++) {
				glm::vec4 color = getColor(detected_faces[i].shape_id);
				if (i == _selectedFace) color = glm::vec4(0, 0, 1, 1);

				if (show_points) {
					std::vector<Vertex> vertices;
					for (int j = 0; j < detected_faces[i].points.size(); j++) {
						glm::vec3 pos(detected_faces[i].points[j].first);
						glutils::drawBox(0.3, 0.3, 0.3, color, glm::translate(glm::mat4(), pos), vertices);
					}
					renderManager.addObject("point_cloud", "", vertices, true);
				}

				if (show_faces) {
					std::vector<Vertex> vertices;
					for (auto& triangle : detected_faces[i].triangles) {
						glutils::drawPolygon(triangle, color, glm::mat4(), vertices);
					}
					renderManager.addObject("face", "", vertices, true);
				}
			}
		}
	}

	// update shadow map
	renderManager.updateShadowMap(this, light_dir, light_mvpMatrix);
}

void GLWidget3D::keyPressEvent(QKeyEvent *e) {
	ctrlPressed = false;
	shiftPressed = false;

	if (e->modifiers() & Qt::ControlModifier) {
		ctrlPressed = true;
	}
	if (e->modifiers() & Qt::ShiftModifier) {
		shiftPressed = true;
	}
	switch (e->key()) {
	case Qt::Key_Space:
		break;
	default:
		break;
	}
}

void GLWidget3D::keyReleaseEvent(QKeyEvent* e) {
	switch (e->key()) {
	case Qt::Key_Control:
		ctrlPressed = false;
		break;
	case Qt::Key_Shift:
		shiftPressed = false;
		break;
	default:
		break;
	}
}

/**
* This function is called once before the first call to paintGL() or resizeGL().
*/
void GLWidget3D::initializeGL() {
	// init glew
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}

	if (glewIsSupported("GL_VERSION_4_2"))
		printf("Ready for OpenGL 4.2\n");
	else {
		printf("OpenGL 4.2 not supported\n");
		exit(1);
	}
	const GLubyte* text = glGetString(GL_VERSION);
	printf("VERSION: %s\n", text);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_3D);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glDisable(GL_TEXTURE_3D);

	glEnable(GL_TEXTURE_2D_ARRAY);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glDisable(GL_TEXTURE_2D_ARRAY);

	////////////////////////////////
	renderManager.init("", "", "", true, 8192);
	renderManager.resize(this->width(), this->height());

	glUniform1i(glGetUniformLocation(renderManager.programs["ssao"], "tex0"), 0);//tex0: 0
}

/**
* This function is called whenever the widget has been resized.
*/
void GLWidget3D::resizeGL(int width, int height) {
	height = height ? height : 1;
	glViewport(0, 0, width, height);
	camera.updatePMatrix(width, height);

	renderManager.resize(width, height);
}

/**
* This function is called whenever the widget needs to be painted.
*/
void GLWidget3D::paintEvent(QPaintEvent *event) {
	if (first_paint) {
		std::vector<Vertex> vertices;
		glutils::drawQuad(0.001, 0.001, glm::vec4(1, 1, 1, 1), glm::mat4(), vertices);
		renderManager.addObject("dummy", "", vertices, true);
		renderManager.updateShadowMap(this, light_dir, light_mvpMatrix);
		first_paint = false;
	}

	// draw by OpenGL
	makeCurrent();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	render();

	// unbind texture
	glActiveTexture(GL_TEXTURE0);

	// restore the settings for OpenGL
	glShadeModel(GL_FLAT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	QPainter painter(this);
	painter.setOpacity(1.0f);
	////
	painter.end();

	glEnable(GL_DEPTH_TEST);
}

/**
* This event handler is called when the mouse press events occur.
*/
void GLWidget3D::mousePressEvent(QMouseEvent *e) {
	// This is necessary to get key event occured even after the user selects a menu.
	setFocus();

	if (e->buttons() & Qt::RightButton) {
		camera.mousePress(e->x(), e->y());
	}
	else if (e->buttons() & Qt::LeftButton) {
		// camera position in the world coordinates
		glm::vec3 cameraPos = camera.cameraPosInWorld();
		// view direction
		glm::vec3 view_dir = viewVector(glm::vec2(e->x(), e->y()), camera.mvMatrix, camera.f(), camera.get_aspect());

		// select a building
		selectFace(cameraPos, view_dir);
		update3DGeometry();
		update();
	}
}

/**
* This event handler is called when the mouse move events occur.
*/

void GLWidget3D::mouseMoveEvent(QMouseEvent *e) {
	if (e->buttons() & Qt::RightButton) {
		if (shiftPressed) {
			camera.move(e->x(), e->y());
		}
		else {
			camera.rotate(e->x(), e->y(), (ctrlPressed ? 0.1 : 1));
		}
	}

	update();
}

/**
* This event handler is called when the mouse release events occur.
*/
void GLWidget3D::mouseReleaseEvent(QMouseEvent *e) {
}

void GLWidget3D::wheelEvent(QWheelEvent* e) {
	camera.zoom(e->delta() * 0.2);
	update();
}
