//
// Created by goksu on 4/6/19.
//
#pragma once
#include "Triangle.hpp"
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <Eigen/Dense> 
using namespace Eigen;

namespace rst //rasterizer 光栅器
{
	struct Mesh
	{
		Mesh() = default;
		Mesh(const std::vector<Eigen::Vector3f>& vbo, const std::vector<Eigen::Vector3i>& ibo) 
			:m_vbo(vbo), m_ibo(ibo) {}
		std::vector<Eigen::Vector3f> m_vbo;
		std::vector<Eigen::Vector3i> m_ibo; //std::array<int, 3>
		Matrix4f m_mat = Matrix4f::Identity();
		std::vector<Eigen::Vector3f> vbo() const
		{
			std::vector<Eigen::Vector3f> vbo;
			for (const auto& iter : m_vbo)
				vbo.push_back((m_mat * iter.homogeneous()).hnormalized());
			return vbo;
		}
		Eigen::AlignedBox3f getBox() const
		{
			Eigen::AlignedBox3f box;
			for (const auto& iter : m_vbo)
				box.extend((m_mat * iter.homogeneous()).hnormalized());
			return box;
		}
	};

	class vector3List
	{
	public:
		//std::vector<std::array<float, 3>> xyz;
		std::vector<std::tuple<float, float, float>> coorVct;
		std::set<std::tuple<float, float, float>> coorSet;
		std::map<std::tuple<float, float, float>,int> coorMap;
		vector3List(const std::vector<Eigen::Vector3f>& coords)
		{ 
			coorVct.resize(coords.size());
			for (int i = 0; i < coords.size(); ++i)
			{
				std::tuple<float, float, float> key = { coords[i][0], coords[i][1], coords[i][2] };
				coorVct[i] = key;
				coorSet.insert(key);
				if (coorMap.find(key) != coorMap.end())
					coorMap[key]++;
				else
					coorMap.emplace(key, 1);
			}
		}
	};

	//rasterizer 光栅(Raster)由像素构成的一个矩形网格。
	class Rasterizer
	{
	public:
		Rasterizer(int w, int h);
		void clear();

		void load_mesh(const Mesh& mesh)
		{
			pos_buf.emplace(0, mesh.vbo());
			ind_buf.emplace(0, mesh.m_ibo);
		}

		void set_model(const Eigen::Matrix4f& m) {	model = m; }
		void set_view(const Eigen::Matrix4f& v) { view = v; }
		void set_projection(const Eigen::Matrix4f& p) { projection = p; }

		void set_pixel_color(const Eigen::Vector3f& point, const Eigen::Vector3f& color);//frame_buf
		void draw();
		std::vector<Eigen::Vector3f>& frame_buffer() { return frame_buf; } //

	private:
		void draw_line(Eigen::Vector3f begin, Eigen::Vector3f end); //draw_segment
		void rasterize_wireframe(const Triangle& t);

	private:
		Eigen::Matrix4f model;
		Eigen::Matrix4f view;
		Eigen::Matrix4f projection;
		std::map<int, std::vector<Eigen::Vector3f>> pos_buf; //vbo
		std::map<int, std::vector<Eigen::Vector3i>> ind_buf; //ibo

		// one dimension vector, size=width*height
		int width, height;
		std::vector<Vector3f> frame_buf;
		std::vector<float> depth_buf;

	};
} // namespace rst
