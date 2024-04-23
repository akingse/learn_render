//
// Created by LEI XU on 4/11/19.
//

#ifndef RASTERIZER_TRIANGLE_H
#define RASTERIZER_TRIANGLE_H
#pragma once

struct Mesh
{
	Mesh() = default;
	Mesh(const std::vector<Eigen::Vector3f>& vbo, const std::vector<Eigen::Vector3i>& ibo)
		:m_vbo(vbo), m_ibo(ibo) {}
	//face
	std::vector<Eigen::Vector3i> m_ibo; //std::array<int, 3>
	std::vector<Eigen::Vector3f> m_nbo; //normal of face
	//vertex
	std::vector<Eigen::Vector3f> m_vbo; //
	std::vector<Eigen::Vector3f> m_vno; //normal of vertex
	std::vector<Eigen::Vector3f> m_col; //rgb color of vertex
	std::vector<Eigen::Vector2f> m_tuv; //tex_coords uv of vertex
	Eigen::Matrix4f m_mat = Eigen::Matrix4f::Identity();
	std::vector<Eigen::Vector3f> vbo() const
	{
		std::vector<Eigen::Vector3f> vbo;
		for (const auto& iter : m_vbo)
			vbo.push_back((m_mat * iter.homogeneous()).hnormalized());
		return vbo;
	}
	std::vector<Eigen::Vector3f> nbo() const
	{
		std::vector<Eigen::Vector3f> nbo;
		for (const auto& iter : m_nbo)
			nbo.push_back((m_mat * iter.homogeneous()).hnormalized());
		return nbo;
	}
	Eigen::AlignedBox3f getBox() const
	{
		Eigen::AlignedBox3f box;
		for (const auto& iter : m_vbo)
			box.extend((m_mat * iter.homogeneous()).hnormalized());
		return box;
	}
};

class Triangle
{
public:
	//std::array<Vector3f, 3> m_vertex;
	Eigen::Vector3f vertex[3]; /*the original coordinates of the triangle, v0, v1, v2 in counter clockwise order*/
	Eigen::Vector3f normal[3];     // normal vector for each vertex
	Eigen::Vector3f color[3];      // color at each vertex;
	Eigen::Vector2f tex_coords[3]; // texture u,v
	Texture* tex = nullptr;

public:
	Triangle()
	{
		memset(this, 0, 3 * (3 + 3 + 2 + 3) * sizeof(float));
	}
	void setVertex(int ind, const Eigen::Vector3f& ver)
	{
		vertex[ind] = ver;
	}; /*set i-th vertex coordinates */
	void setNormal(int ind, const Eigen::Vector3f& n)
	{
		normal[ind] = n;
	};   /*set i-th vertex normal vector*/
	//void setColor(int ind, float r, float g, float b) { color[ind] = Vector3f(r / 255.f, g / 255.f, b / 255.f); }; /*set i-th vertex color*/
	void setColor(int ind, const Eigen::Vector3f& rgb)
	{
		color[ind] = 1 / 255.f * rgb;
	};
	void setColors(const std::array<Eigen::Vector3f, 3>& colors)
	{
		setColor(0, colors[0]);
		setColor(1, colors[1]);
		setColor(2, colors[2]);
	}
	void setTexCoord(int ind, float s, float t)
	{
		tex_coords[ind] = Eigen::Vector2f(s, t);
	}; /*set i-th vertex texture coordinate*/
	void setTexCoord(int ind, Eigen::Vector2f uv) /*set i-th vertex texture coordinate*/
	{
		tex_coords[ind] = uv;
	}
	std::array<Eigen::Vector3f, 3> getColor() const // format: 1->255
	{
		return { 255 * color[0], 255 * color[1], 255 * color[2] };
	} // Only one color per triangle.
	std::array<Eigen::Vector4f, 3> toVector4() const
	{
		std::array<Eigen::Vector4f, 3> res;
		std::transform(std::begin(vertex), std::end(vertex), res.begin(), [](auto& vec) { return Eigen::Vector4f(vec.x(), vec.y(), vec.z(), 1.f); });
		return res;
	}

};

inline std::vector<Triangle*> loadTriangles(const std::string& filename)
{
	objl::Loader loader;
	bool loadout = loader.LoadFile(filename);
	if (!loadout)
		return {};
	std::vector<Triangle*> TriangleList;
	for (const auto& mesh : loader.LoadedMeshes)
	{
		for (int i = 0; i < mesh.Vertices.size(); i += 3)
		{
			Triangle* t = new Triangle();
			for (int j = 0; j < 3; j++)
			{
				t->setVertex(j, Eigen::Vector3f(mesh.Vertices[i + j].Position.X, mesh.Vertices[i + j].Position.Y, mesh.Vertices[i + j].Position.Z));
				t->setNormal(j, Eigen::Vector3f(mesh.Vertices[i + j].Normal.X, mesh.Vertices[i + j].Normal.Y, mesh.Vertices[i + j].Normal.Z));
				t->setTexCoord(j, Eigen::Vector2f(mesh.Vertices[i + j].TextureCoordinate.X, mesh.Vertices[i + j].TextureCoordinate.Y));
			}
			TriangleList.push_back(t);
		}
	}
	return TriangleList;
}

inline void deleteTriangles(std::vector<Triangle*>& triangles)
{
	for (auto& iter : triangles)
	{
		delete iter;
		iter = nullptr;
	}
}

inline std::vector<Mesh> loadMeshs(const std::string& filename)
{
	objl::Loader loader;
	bool loadout = loader.LoadFile(filename);
	if (!loadout)
		return {};
	std::vector<Mesh> meshVct;
	for (const auto& iter : loader.LoadedMeshes)
	{
		Mesh mesh;
		for (int i = 0; i < iter.Indices.size();  i += 3)
		{
			mesh.m_ibo.push_back(Eigen::Vector3i{ i,i + 1,i + 2 });
		}
		for (int i = 0; i < iter.Vertices.size(); i += 3)
		{
			for (int j = 0; j < 3; j++)
			{
				mesh.m_vbo.push_back(Eigen::Vector3f(iter.Vertices[i + j].Position.X, iter.Vertices[i + j].Position.Y, iter.Vertices[i + j].Position.Z));
				mesh.m_nbo.push_back(Eigen::Vector3f(iter.Vertices[i + j].Normal.X, iter.Vertices[i + j].Normal.Y, iter.Vertices[i + j].Normal.Z));
				mesh.m_tuv.push_back(Eigen::Vector2f(iter.Vertices[i + j].TextureCoordinate.X, iter.Vertices[i + j].TextureCoordinate.Y));
			}
		}
		meshVct.push_back(mesh);
	}
	return meshVct;
}
#endif // RASTERIZER_TRIANGLE_H
