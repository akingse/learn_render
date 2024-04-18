// clang-format off
#include <iostream>
#include <opencv2/opencv.hpp>
#include "rasterizer.hpp"
//#include "global.hpp"
#include "Triangle.hpp"
#include "calculateTransform.h"
using namespace Eigen;
using namespace eigen;
using namespace std;
using namespace rst;
#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif // !M_PI

int main(int argc, const char** argv)
{
	float angle = 0;
	int sz_width = 700;
	int sz_height = 700;
	float aspect_ratio = (float)sz_width / sz_height;
	rst::Rasterizer r(sz_width, sz_height);
	Eigen::Vector3f eye_pos = { 0,0,5 };

	std::vector<Eigen::Vector3f> pos
	{
		{2, 0, -2},
		{0, 2, -2},
		{-2, 0, -2},
		{3.5, -1, -5},
		{2.5, 1.5, -5},
		{-1, 0.5, -5}
	};

	std::vector<Eigen::Vector3i> ind
	{
		{0, 1, 2},
		{3, 4, 5}
	};

	std::vector<Eigen::Vector3f> cols
	{
		//{217.0, 238.0, 185.0},
		//{217.0, 238.0, 185.0},
		//{217.0, 238.0, 185.0},
		//½¥±ägradient ramp
		{255, 0, 0},
		{0, 255, 0},
		{0, 0, 255},
		{185.0, 217.0, 238.0},
		{185.0, 217.0, 238.0},
		{185.0, 217.0, 238.0}
	};
	Mesh mesh(pos, ind);
	mesh.m_col = cols;
	r.load_mesh(mesh);

	int key = 0;
	int frame_count = 0;

	while (key != 27)
	{
		r.clear();
		r.set_model(get_model_matrix(Vector3f(0, 0, 0), Vector3f(1, 1, 0), angle));
		r.set_view(get_viewing_matrix(eye_pos, Vector3f(0, 0, -1), Vector3f(0, 1, 0)));
		r.set_projection(get_projection_matrix(45 * M_PI / 180, aspect_ratio, 0.1, 10));
		r.draw(Mode::Shadering);

		cv::Mat image(sz_height, sz_width, CV_32FC3, r.frame_buffer().data());
		image.convertTo(image, CV_8UC3, 1.0f);
		cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
		cv::imshow("image", image);
		key = cv::waitKey(10);
		//std::cout << "frame count: " << frame_count++ << '\n';
	}

	return 0;
}
// clang-format on