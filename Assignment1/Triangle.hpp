//
// Created by LEI XU on 4/11/19.
//

#ifndef RASTERIZER_TRIANGLE_H
#define RASTERIZER_TRIANGLE_H

#include <Eigen/Dense> 

using namespace Eigen;
/*

	v2
	|\
	| \
	|__\
	v0  v1

*/
class Triangle
{
public:
	//std::array<Vector3f, 3> m_vertex;
	Vector3f vertex[3]; /*the original coordinates of the triangle, v0, v1, v2 in counter clockwise order*/
	Vector3f color[3];      // color at each vertex;
	Vector2f tex_coords[3]; // texture u,v
	Vector3f normal[3];     // normal vector for each vertex

	// Texture *tex;
	Triangle(); //memset

	//Eigen::Vector3f a() const { return vertex[0]; }
	//Eigen::Vector3f b() const { return vertex[1]; }
	//Eigen::Vector3f c() const { return vertex[2]; }

	void setVertex(int ind, Vector3f ver); /*set i-th vertex coordinates */
	void setNormal(int ind, Vector3f n);   /*set i-th vertex normal vector*/
	void setColor(int ind, float r, float g, float b); /*set i-th vertex color*/
	void setTexCoord(int ind, float s, float t); /*set i-th vertex texture coordinate*/
	//std::array<Vector4f, 3> toVector4() const;
};

#endif // RASTERIZER_TRIANGLE_H
