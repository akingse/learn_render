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

void rst::Rasterizer::draw(Mode mode)
{
	const vector<Eigen::Vector3f>& vbo = pos_buf[0];
	const vector<Eigen::Vector3i>& ibo = ind_buf[0];
	const vector<Eigen::Vector3f>& col = col_buf[0];

	float f1 = (50 - 0.1) / 2.0;
	float f2 = (50 + 0.1) / 2.0;

	Eigen::Matrix4f mvp = projection * view * model;
	for (const auto& face : ibo)
	{
		array<Eigen::Vector3f, 3> trigon = array<Eigen::Vector3f, 3>{ vbo[face[0]], vbo[face[1]], vbo[face[2]] };
		trigon = mvp * trigon;
		for (auto& vert : trigon)
		{
			vert.x() = 0.5 * width * (vert.x() + 1.0f); //move canonical cube to origin
			vert.y() = 0.5 * height * (vert.y() + 1.0f);
			vert.z() = vert.z() * f1 + f2;
		}
		Triangle t;
		for (int i = 0; i < 3; ++i)
		{
			t.setVertex(i, trigon[i]);
		}
		Vector3f col_x = col[face[0]];
		Vector3f col_y = col[face[1]];
		Vector3f col_z = col[face[2]];
		t.setColor(0, col_x);
		t.setColor(1, col_y);
		t.setColor(2, col_z);
		if (Mode::Wireframe == mode)
			rasterize_wireframe(t);
		else if (Mode::Shadering == mode)
			rasterize_triangle(t);

	}
}

void rst::Rasterizer::rasterize_wireframe(const Triangle& t)
{
	draw_line(t.vertex[0], t.vertex[1]);
	draw_line(t.vertex[1], t.vertex[2]);
	draw_line(t.vertex[2], t.vertex[0]);
}

//Screen space rasterization
void Rasterizer::rasterize_triangle(const Triangle& t) 
{
	std::array<Vector4f, 3> v = t.toVector4();
	// Find out the bounding box of current triangle.
	// iterate through the pixel and find if the current pixel is inside the triangle
	std::vector<float> x_arry{ v[0].x(), v[1].x(), v[2].x() };
	std::vector<float> y_arry{ v[0].y(), v[1].y(), v[2].y() };
	std::sort(x_arry.begin(), x_arry.end());
	std::sort(y_arry.begin(), y_arry.end());
	int x_min = floor(x_arry[0]), x_max = ceil(x_arry[2]),
		y_min = floor(y_arry[0]), y_max = ceil(y_arry[2]);
	auto _toArray = [](const Vector3f* v)->array<Eigen::Vector3f, 3>
		{
			return array<Eigen::Vector3f, 3>{v[0], v[1], v[2]};
		};
	// If so, use the following code to get the interpolated z value.
	//set the current pixel (use the set_pixel function) to the color of the triangle (use getColor function) if it should be painted.
	for (int x = x_min; x < x_max; x++)
	{
		for (int y = y_min; y < y_max; y++) 
		{
			if (!insideTriangle(x + 0.5f, y + 0.5f, _toArray(t.vertex)))
				continue;
			//[alpha, beta, gamma]
			Vector3f abg = computeBarycentric2D(x, y, _toArray(t.vertex));
			float w_reciprocal = 1.0 / (abg[0] / v[0].w() + abg[1] / v[1].w() + abg[2] / v[2].w());
			float z_interpolated = abg[0] * v[0].z() / v[0].w() + abg[1] * v[1].z() / v[1].w() + abg[2] * v[2].z() / v[2].w();
			z_interpolated *= w_reciprocal;
			int id = get_index(x, y);
			if (depth_buf.size() <= id || id < 0)
				continue;
			if (z_interpolated < depth_buf[id]) 
			{
				Eigen::Vector3f point(x, y, 1.0f);
				set_pixel_color(point, t.getColor());
				depth_buf[get_index(x, y)] = z_interpolated;
			}
		}
	}
}
