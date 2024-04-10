//
// Created by LEI XU on 4/11/19.
//

#ifndef RASTERIZER_TRIANGLE_H
#define RASTERIZER_TRIANGLE_H

#include <Eigen/Dense> 

using namespace Eigen;
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
	Vector3f vertex[3]; /*the original coordinates of the triangle, v0, v1, v2 in counter clockwise order*/
	Vector3f normal[3];     // normal vector for each vertex
	Vector3f color[3];      // color at each vertex;
	Vector2f tex_coords[3]; // texture u,v

	Triangle() { memset(this, 0, 3 * (3 + 3 + 2 + 3) * sizeof(float)); }

	void setVertex(int ind, Vector3f ver) { vertex[ind] = ver; }; /*set i-th vertex coordinates */
	void setNormal(int ind, Vector3f n) { normal[ind] = n; };   /*set i-th vertex normal vector*/
	void setColor(int ind, float r, float g, float b) { color[ind] = Vector3f(r / 255.f, g / 255.f, b / 255.f); }; /*set i-th vertex color*/
	void setTexCoord(int ind, float s, float t) { tex_coords[ind] = Vector2f(s, t); }; /*set i-th vertex texture coordinate*/
};

#endif // RASTERIZER_TRIANGLE_H
