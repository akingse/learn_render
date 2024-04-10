//
// Created by goksu on 4/6/19.
//
#pragma once
#include "Triangle.hpp"
#include <map>
#include <vector>
#include <algorithm>
#include <Eigen/Dense> 
using namespace Eigen;

namespace rst //rasterizer
{
	enum class Buffers:int
	{
		Color = 1,
		Depth = 2
	};

	inline Buffers operator|(Buffers a, Buffers b)
	{
		return Buffers((int)a | (int)b);
	}

	inline Buffers operator&(Buffers a, Buffers b)
	{
		return Buffers((int)a & (int)b);
	}

	enum class Primitive
	{
		Line,
		Triangle,
		Cube,
	};

	/*
	 * For the curious : The draw function takes two buffer id's as its arguments.
	 * These two structs make sure that if you mix up with their orders, the
	 * compiler won't compile it. Aka : Type safety
	 * */
	//struct pos_buf_id //position
	//{
	//	int pos_id = 0;
	//};
	//struct ind_buf_id //index
	//{
	//	int ind_id = 0;
	//};

	typedef int pos_buf_id;
	typedef int ind_buf_id;

	//rasterizer 光栅(Raster)由像素构成的一个矩形网格。
	class Rasterizer
	{
	public:
		Rasterizer(int w, int h);
		void clear(Buffers buff);

		pos_buf_id load_positions(const std::vector<Eigen::Vector3f>& positions)
		{
			int id = get_next_id();
			pos_buf.emplace(id, positions);
			return { id };
		}
		ind_buf_id load_indices(const std::vector<Eigen::Vector3i>& indices)
		{
			int id = get_next_id();
			ind_buf.emplace(id, indices);
			return { id };
		}

		void set_model(const Eigen::Matrix4f& m) {	model = m; }
		void set_view(const Eigen::Matrix4f& v) { view = v; }
		void set_projection(const Eigen::Matrix4f& p) { projection = p; }

		void set_pixel_color(const Eigen::Vector3f& point, const Eigen::Vector3f& color);//frame_buf
		void draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, Primitive type);
		std::vector<Eigen::Vector3f>& frame_buffer() { return frame_buf; } //

	private:
		void draw_line(Eigen::Vector3f begin, Eigen::Vector3f end); //draw_segment
		void rasterize_wireframe(const Triangle& t);

	private:
		Eigen::Matrix4f model;
		Eigen::Matrix4f view;
		Eigen::Matrix4f projection;
		std::map<int, std::vector<Eigen::Vector3f>> pos_buf;
		std::map<int, std::vector<Eigen::Vector3i>> ind_buf;

		// one dimension vector, size=width*height
		int width, height;
		std::vector<Vector3f> frame_buf;
		std::vector<float> depth_buf;

		//int get_index(int x, int y)
		//{
		//	return (height - y) * width + x;
		//}

		int next_id = 0;
		int get_next_id() 
		{
			return next_id++;
		}
	};
} // namespace rst
