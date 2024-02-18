#include "pch.h"
using namespace std;
using namespace Eigen;
using namespace games;
using namespace psykronix;

std::array<Eigen::Vector3d, 2> rayIntersectionWithSphere(const games::RayEquation& ray, const games::Sphere& sphere)
{
	std::array<Eigen::Vector3d, 2> points = { gVecNaN, gVecNaN };
	double a = ray.m_direction.squaredNorm(); // vec.dot(vec)
	double b = 2 * (ray.m_origin - sphere.m_center).dot(ray.m_direction);
	double c = (ray.m_origin - sphere.m_center).squaredNorm() - sphere.r * sphere.r;
	double delta = b * b - 4 * a * c;
	if (delta == 0) //abslute zero
	{
		double t = -b / (2 * a);
		if (t >= 0) //only assign positive ray
			points[0] = ray.m_origin + t * ray.m_direction;
	}
	else
	{
		delta = sqrt(delta);
		double t0 = (-b + delta) / (2 * a); // a>0, so t0>t1
		if (t0 >= 0)
			points[0] = ray.m_origin + t0 * ray.m_direction;
		double t1 = (-b - delta) / (2 * a);
		if (t1 >= 0)
			points[1] = ray.m_origin + t1 * ray.m_direction;
	}
	return points;
}

std::tuple<bool, double> rayIntersectionWithPlane(const games::RayEquation& ray, const games::PlaneEquation& plane)
{
	//if (ray.m_direction.dot(plane.m_normal) == 0)
	//	return -1; //-nan(ind)
	double t = (plane.m_origin - ray.m_origin).dot(plane.m_normal) / ray.m_direction.dot(plane.m_normal);
	return { (0 <= t && !isnan(t)),t }; //check 0<=t<oo
}

//isRayLineCrossTriangleMTA
std::tuple<bool, double> rayIntersectionWithTriangle(const games::RayEquation& ray, const psykronix::Triangle& triangle)
{
	//ignore ray on triangle plane
	Vector3d normal = (triangle[1] - triangle[0]).cross(triangle[2] - triangle[1]);
	if (isPerpendi(ray.m_direction, normal))
		return { false, 0 };
	//get intersect point of ray and plane 
	std::tuple<bool, double> intersect = rayIntersectionWithPlane(ray, PlaneEquation(triangle[0], normal));
	if (!std::get<0>(intersect))
		return { false, 0 };
	// is point in triangle
	double t = std::get<1>(intersect);
	Vector3d point = ray.m_origin + t * ray.m_direction;
	return { isPointInTriangle(point, triangle), t };
}


