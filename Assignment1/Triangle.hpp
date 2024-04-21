//
// Created by LEI XU on 4/11/19.
//

#ifndef RASTERIZER_TRIANGLE_H
#define RASTERIZER_TRIANGLE_H
#pragma once

/*

		.v2
	   / \
	  /	  \
	 /__ __\
	v0      v1

*/
class Triangle
{
public:
	//std::array<Vector3f, 3> m_vertex;
	Eigen::Vector3f vertex[3]; /*the original coordinates of the triangle, v0, v1, v2 in counter clockwise order*/
	Eigen::Vector3f normal[3];     // normal vector for each vertex
	Eigen::Vector3f color[3];      // color at each vertex;
	Eigen::Vector2f tex_coords[3]; // texture u,v
	Texture* tex = nullptr;

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
}

inline void deleteTriangles(std::vector<Triangle*>& triangles)
{
	for (auto& iter : triangles)
	{
		delete iter;
		iter = nullptr;
	}
}

#endif // RASTERIZER_TRIANGLE_H
