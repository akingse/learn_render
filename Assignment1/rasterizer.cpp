#include <vector>
#include <algorithm>
#include "rasterizer.hpp"
#include "calculateTransform.h"
//#include <opencv2/opencv.hpp>
#include <math.h>
#include <stdexcept>
using namespace std;
using namespace rst;
using namespace Eigen;
using namespace eigen;

rst::Rasterizer::Rasterizer(int w, int h) : width(w), height(h)
{
	frame_buf.resize(w * h);
	depth_buf.resize(w * h);
}

void rst::Rasterizer::clear()
{
	std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f{ 0, 0, 0 });
	std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity()); //inf
}

void rst::Rasterizer::set_pixel_color(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
	//old index: auto ind = point.y() + point.x() * width;
	if (point.x() < 0 || point.x() >= width ||
		point.y() < 0 || point.y() >= height)
		return;
	int ind = (height - point.y()) * width + point.x();
	if (ind < 0 || frame_buf.size() <= ind)
		return;
	frame_buf[ind] = color;
}

// Bresenham's line drawing algorithm
// Code taken from a stack overflow answer: https://stackoverflow.com/a/16405254
void rst::Rasterizer::draw_line(Eigen::Vector3f begin, Eigen::Vector3f end)
{
	float x1 = begin.x();
	float y1 = begin.y();
	float x2 = end.x();
	float y2 = end.y();
	Eigen::Vector3f line_color = { 255, 255, 255 };
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1;
	dy = y2 - y1;
	dx1 = fabs(dx);
	dy1 = fabs(dy);
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1;
			y = y1;
			xe = x2;
		}
		else
		{
			x = x2;
			y = y2;
			xe = x1;
		}
		Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
		set_pixel_color(point, line_color);
		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
			{
				px = px + 2 * dy1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
					y = y + 1;
				else
					y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			//            delay(0);
			Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
			set_pixel_color(point, line_color);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1;
			y = y1;
			ye = y2;
		}
		else
		{
			x = x2;
			y = y2;
			ye = y1;
		}
		Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
		set_pixel_color(point, line_color);
		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
			{
				py = py + 2 * dx1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
					x = x + 1;
				else
					x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}
			Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
			set_pixel_color(point, line_color);
		}
	}
}

void rst::Rasterizer::draw()
{
	const vector<Eigen::Vector3f>& vbo = pos_buf[0];
	const vector<Eigen::Vector3i>& ibo = ind_buf[0];
	float f1 = (100 - 0.1) / 2.0;
	float f2 = (100 + 0.1) / 2.0;

	Eigen::Matrix4f mvp = projection * view * model;
	for (const auto& face : ibo)
	{
		array<Eigen::Vector3f, 3> trigon = mvp * array<Eigen::Vector3f, 3>{ vbo[face[0]], vbo[face[1]], vbo[face[2]] };
		for (auto& vert : trigon)
		{
			vert.x() = 0.5 * width * (vert.x() + 1.0);
			vert.y() = 0.5 * height * (vert.y() + 1.0);
			vert.z() = vert.z() * f1 + f2;
		}
		Triangle t;
		for (int i = 0; i < 3; ++i)
		{
			t.setVertex(i, trigon[i]);
		}
		t.setColor(0, 255.0, 0.0, 0.0);
		t.setColor(1, 0.0, 255.0, 0.0);
		t.setColor(2, 0.0, 0.0, 255.0);
		rasterize_wireframe(t);
	}
}

void rst::Rasterizer::rasterize_wireframe(const Triangle& t)
{
	draw_line(t.vertex[2], t.vertex[0]);
	draw_line(t.vertex[2], t.vertex[1]);
	draw_line(t.vertex[1], t.vertex[0]);
}


