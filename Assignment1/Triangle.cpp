//
// Created by LEI XU on 4/11/19.
//

#include "Triangle.hpp"
#include <algorithm>
#include <array>
#include <stdexcept>

Triangle::Triangle()
{
	memset(this, 0, 3 * (3 + 3 + 2 + 3) * sizeof(float));
}

void Triangle::setVertex(int ind, Eigen::Vector3f ver)
{
	vertex[ind] = ver;
}

void Triangle::setNormal(int ind, Vector3f n)
{
	normal[ind] = n;
}

void Triangle::setColor(int ind, float r, float g, float b)
{
	if ((r < 0.0) || (r > 255.) || (g < 0.0) || (g > 255.) || (b < 0.0) || (b > 255.))
	{
		throw std::runtime_error("Invalid color values");
	}
	color[ind] = Vector3f((float)r / 255., (float)g / 255., (float)b / 255.);
	return;
}
void Triangle::setTexCoord(int ind, float s, float t) // u v
{
	tex_coords[ind] = Vector2f(s, t);
}

//std::array<Vector4f, 3> Triangle::toVector4() const
//{
//	std::array<Vector4f, 3> res;
//	std::transform(std::begin(vertex), std::end(vertex), res.begin(), [](auto& vec) {
//		return Vector4f(vec.x(), vec.y(), vec.z(), 1.f);
//		});
//	return res;
//}
