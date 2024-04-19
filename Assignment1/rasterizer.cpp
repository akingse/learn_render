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

void rst::Rasterizer::set_pixel_color(const Eigen::Vector3f& point, const Eigen::Vector3f& color) //int coord in width*height
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
			t.setColor(i, col[face[i]]);
		}

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
	//std::vector<float> x_arry{ v[0].x(), v[1].x(), v[2].x() };
	//std::vector<float> y_arry{ v[0].y(), v[1].y(), v[2].y() };
	//std::sort(x_arry.begin(), x_arry.end());
	//std::sort(y_arry.begin(), y_arry.end());
	//int x_min = floor(x_arry[0]), x_max = ceil(x_arry[2]),
	//	y_min = floor(y_arry[0]), y_max = ceil(y_arry[2]);
	//using AABB 
	AlignedBox3f box;
	for (const auto& iter : t.vertex)
		box.extend(iter);
	auto _toArray = [](const Vector3f* v)->array<Eigen::Vector3f, 3>
		{
			return array<Eigen::Vector3f, 3>{v[0], v[1], v[2]};
		};
	auto _getColorInterp = [](const array<Eigen::Vector3f, 3>& trigon, Eigen::Vector3f bc)->Eigen::Vector3f
	{
		Eigen::Vector3f AB = trigon[1] - trigon[0];
		Eigen::Vector3f AC = trigon[2] - trigon[0];
		return trigon[0] + bc[0] * AB + bc[1] * AC;
	};
	// If so, use the following code to get the interpolated z value.
	//set the current pixel (use the set_pixel function) to the color of the triangle (use getColor function) if it should be painted.
	for (int i = floor(box.min()[0]); i < ceil(box.max()[0]); i++)
	{
		for (int j = floor(box.min()[1]); j < ceil(box.max()[1]); j++)
		{
			if (i < 0 || width < 0|| j < 0 || height < 0) //clip space
				continue;
			float x = i + 0.5f; //center of pixel
			float y = j + 0.5f;
			if (!insideTriangle(x, y, _toArray(t.vertex)))
				continue;
			//[alpha, beta, gamma]
			Vector3f abg = computeBarycentric2D(x, y, _toArray(t.vertex));
			//Vector3f color = computeBarycentric2D(x, y, t.getColor());
			//float w_reciprocal = 1.0 / (abg[0] / v[0].w() + abg[1] / v[1].w() + abg[2] / v[2].w());
			//float z_interpolated = abg[0] * v[0].z() / v[0].w() + abg[1] * v[1].z() / v[1].w() + abg[2] * v[2].z() / v[2].w();
			//z_interpolated *= w_reciprocal;
			Eigen::Vector3f bc = getBarycentricCoordinates(to_vec2(_toArray(t.vertex)), Eigen::Vector2f(x, y));
			Vector3f color = _getColorInterp(t.getColor(), bc);
			int id = get_index(i, j);
			//if (depth_buf.size() <= id || id < 0)
			//	continue;
			if (abg.z() < depth_buf[id])
			{
				Eigen::Vector3f point(i, j, 1.0f); //without offset
				set_pixel_color(point, color);
				depth_buf[get_index(i, j)] = abg.z();
			}
		}
	}
	////test coordinate origon
	//for (int i = 0; i < 10; ++i)
	//{
	//	for (int j = 0; j < width/2; ++j)
	//		set_pixel_color(Vector3f(j, i, 0), Vector3f(255, 255, 255));
	//}


}
