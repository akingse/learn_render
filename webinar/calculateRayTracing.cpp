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

//AABB Bounding Volumes, Slab method
bool rayIntersectionwithAxisAlignedBoundingBox(const games::RayEquation& ray, const Eigen::AlignedBox3d& box)
{
	double t_enter, t_exit;
	if (ray.m_direction[0] == 0 && ray.m_direction[1] == 0) //z-axis
	{
		t_enter = (box.min()[2] - ray.m_origin[2]) / ray.m_direction[2];
		t_exit = (box.max()[2] - ray.m_origin[2]) / ray.m_direction[2];
		return t_enter < t_exit && 0 <= t_exit;
	}
	if (ray.m_direction[1] == 0 && ray.m_direction[2] == 0) //x-axis
	{

	}
	if (ray.m_direction[2] == 0 && ray.m_direction[0] == 0) //y-axis
	{

	}


	if (ray.m_direction[0] == 0)
	{
		if (ray.m_origin[0] < box.min()[0] || ray.m_origin[0] > box.max()[0])
			return false;
		if (ray.m_direction[1] == 0) //z-axis
		{
			if (ray.m_origin[1] < box.min()[1] || ray.m_origin[1] > box.max()[1])
				return false;
			t_enter = (box.min()[2] - ray.m_origin[2]) / ray.m_direction[2];
			t_exit = (box.max()[2] - ray.m_origin[2]) / ray.m_direction[2];
			return t_enter < t_exit && 0 <= t_exit;
		}
		if (ray.m_direction[2] == 0) //y-axis
		{
			if (ray.m_origin[2] < box.min()[2] || ray.m_origin[2] > box.max()[2])
				return false;
			t_enter = (box.min()[1] - ray.m_origin[1]) / ray.m_direction[1];
			t_exit = (box.max()[1] - ray.m_origin[1]) / ray.m_direction[1];
			return t_enter < t_exit && 0 <= t_exit;
		}
		t_enter = std::max((box.min()[1] - ray.m_origin[1]) / ray.m_direction[1], (box.min()[2] - ray.m_origin[2]) / ray.m_direction[2]);
		t_exit = std::min((box.max()[1] - ray.m_origin[1]) / ray.m_direction[1], (box.max()[2] - ray.m_origin[2]) / ray.m_direction[2]);
		return t_enter < t_exit && 0 <= t_exit;
	}
	if (ray.m_direction[1] == 0)
	{

	}
	if (ray.m_direction[2] == 0) //xy-plane
	{
		if (ray.m_origin[2] < box.min()[2] || ray.m_origin[2] > box.max()[2])
			return false;
		double tx0 = (box.min()[0] - ray.m_origin[0]) / ray.m_direction[0];
		double ty0 = (box.min()[1] - ray.m_origin[1]) / ray.m_direction[1];
		double tx1 = (box.max()[0] - ray.m_origin[0]) / ray.m_direction[0];
		double ty1 = (box.max()[1] - ray.m_origin[1]) / ray.m_direction[1];
		t_enter = std::max(std::min(tx0, ty0), std::min(tx1, ty1));
		t_exit = std::min(std::max(tx0, ty0), std::max(tx1, ty1));
		//return t_enter < t_exit && 0 <= t_exit;
	}
	//if (ray.m_direction[0] != 0 && ray.m_direction[1] != 0 && ray.m_direction[2] != 0)
	Vector3d t_min = (box.min() - ray.m_origin).cwiseQuotient(ray.m_direction);
	Vector3d t_max = (box.max() - ray.m_origin).cwiseQuotient(ray.m_direction);
	Eigen::Vector3d tmin_v = t_min.cwiseMin(t_max);
	Eigen::Vector3d tmax_v = t_min.cwiseMax(t_max);
	t_enter = tmin_v.maxCoeff();
	t_exit = tmax_v.minCoeff();

	t_enter = std::max(std::max((box.min()[0] - ray.m_origin[0]) / ray.m_direction[0], (box.min()[1] - ray.m_origin[1]) / ray.m_direction[1]), (box.min()[2] - ray.m_origin[2]) / ray.m_direction[2]);
	t_exit = std::min(std::min((box.max()[0] - ray.m_origin[0]) / ray.m_direction[0], (box.max()[1] - ray.m_origin[1]) / ray.m_direction[1]), (box.max()[2] - ray.m_origin[2]) / ray.m_direction[2]);
	return t_enter < t_exit && 0 <= t_exit; //ray and AABB intersect iff
}


