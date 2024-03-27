#include "pch.h"
using namespace std;
using namespace Eigen;
using namespace eigen;
using namespace games;
using namespace clash;

std::array<Eigen::Vector3f, 2> rayIntersectionWithSphere(const games::RayEquation& ray, const games::Sphere& sphere)
{
	std::array<Eigen::Vector3f, 2> points = { Vector3f(nan("0"), 0,0), Vector3f(nan("0"), 0,0) };
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
std::tuple<bool, double> rayIntersectionWithTriangle(const games::RayEquation& ray, const array<Eigen::Vector3f, 3>& triangle)
{
	//ignore ray on triangle plane
	Vector3f normal = (triangle[1] - triangle[0]).cross(triangle[2] - triangle[1]);
	if (fabs(ray.m_direction.dot(normal) < FLT_EPSILON))//isPerpendi3d
		return { false, 0 };
	//get intersect point of ray and plane 
	std::tuple<bool, double> intersect = rayIntersectionWithPlane(ray, PlaneEquation(triangle[0], normal));
	if (!std::get<0>(intersect))
		return { false, 0 };
	// is point in triangle
	double t = std::get<1>(intersect);
	Vector3f point = ray.m_origin + t * ray.m_direction;
	return {};// { isPointInTriangle(point, triangle), t };
}

//#define USING_MATRIX_LIBRARY_EIGEN
//AABB Bounding Volumes, Slab method
bool rayIntersectionwithAxisAlignedBoundingBox(const games::RayEquation& ray, const Eigen::AlignedBox3f& box)
{
#ifndef USING_MATRIX_LIBRARY_EIGEN
	//allow divided by zero
	double tx0 = (box.min()[0] - ray.m_origin[0]) / ray.m_direction[0];
	double ty0 = (box.min()[1] - ray.m_origin[1]) / ray.m_direction[1];
	double tz0 = (box.min()[2] - ray.m_origin[2]) / ray.m_direction[2];
	double tx1 = (box.max()[0] - ray.m_origin[0]) / ray.m_direction[0];
	double ty1 = (box.max()[1] - ray.m_origin[1]) / ray.m_direction[1];
	double tz1 = (box.max()[2] - ray.m_origin[2]) / ray.m_direction[2];
	double t_enter = std_max(std_min(tx0, tx1), std_min(ty0, ty1), std_min(tz0, tz1));
	double t_exit = std_min(std_max(tx0, tx1), std_max(ty0, ty1), std_max(tz0, tz1));
#else
	//if (ray.m_direction[0] != 0 && ray.m_direction[1] != 0 && ray.m_direction[2] != 0)
	Vector3f t_min = (box.min() - ray.m_origin).cwiseQuotient(ray.m_direction);
	Vector3f t_max = (box.max() - ray.m_origin).cwiseQuotient(ray.m_direction);
	Eigen::Vector3f tmin_v = t_min.cwiseMin(t_max);
	Eigen::Vector3f tmax_v = t_min.cwiseMax(t_max);
	double t_enter = tmin_v.maxCoeff();
	double t_exit = tmax_v.minCoeff();
#endif //USING_MATRIX_LIBRARY_EIGEN
	//stay time means isnot boxs edge
	return t_enter < t_exit && 0 <= t_exit; //ray and AABB intersect iff
}

tuple<bool, Vector3f> raySceneIntersection(const RayEquation& ray, const vector<ModelMesh>& models)
{
	// #cells = C * #objs, C = 27 in 3D

	//Bresenham line algorithm
	return { false,Vector3f() };
}

float probability_distribution_function(const Vector3f& w)//pdf()
{
	return 1;
}

float bidirectional_reflectance_distribution_function(const Vector3f& w)//brdf()
{
	return 1;
}

float lightRadiance(const Vector3f& p, const Vector3f& wi)
{
	return 1;
}

//A Simple Monte Carlo Solution
float shadeIntegration(const Vector3f& p, const Vector3f& n, const ModelMesh& light, const vector<ModelMesh>& models)
{
	float Lo = 0.0f;
	int N = 100;
	Vector3f wi = n.cross(Vector3f::UnitX());
	//for (int i = 0; i < N; ++i)
	int i = rand() % N;
	N = 1;
	{
		//wi = rotate(p, n, i / N * 2 * M_PI) * wi;
		//if ray hit the light
		if (std::get<0>(raySceneIntersection(RayEquation(p, wi), { light })))
		{
			float fr = bidirectional_reflectance_distribution_function(wi);
			float pdf = probability_distribution_function(wi);
			float Li = lightRadiance(p, wi);
			Lo += (1 / N) + Li * fr * (n.dot(wi)) / pdf;
		}
		else if (std::get<0>(raySceneIntersection(RayEquation(p, wi), models)))
		{
			//using recursion
			float fr = bidirectional_reflectance_distribution_function(wi);
			float pdf = probability_distribution_function(wi);
			float Li = lightRadiance(p, wi);
			Vector3f q = std::get<1>(raySceneIntersection(RayEquation(p, wi), models));
			Lo += (1 / N) + shadeIntegration(q, -wi, light, models) * fr * (n.dot(wi)) / pdf;

		}

	}
	return Lo / N;
}

