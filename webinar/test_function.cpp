#include "pch.h"
#include <iostream>

using namespace std;
using namespace Eigen;
using namespace games;
using namespace clash;

static void test0()
{
	RayEquation ray(Vector3f(0, 0, 0), Vector3f(-1, 0, 0));
	//RayEquation ray(Vector3f(0, 0, -1), Vector3f(0, 0, -1));
	PlaneEquation plane(Vector3f(0, 0, 0), Vector3f(0, 0, 1));
	std::tuple<bool, double> t0 = rayIntersectionWithPlane(ray, plane);

	Vector3f p0(1, 0, 0);
	Vector3f p1(0, 1, 0);
	Vector3f origin(0, 0, 0);
	Vector3f direct(1, 1, 0);
	Vector3f left = (origin - p0).cross(origin - p1);
	Vector3f right = (2*origin - p0 - p1).cross(direct);

	RayEquation ray1(Vector3f(0, 0, 0), Vector3f(2, 1, 0));
	RayEquation ray2(Vector3f(1, 0, 0), Vector3f(-1, 1, 0));

	left = (ray1.m_origin - ray2.m_origin).cross(ray2.m_direction);
	right = (ray1.m_direction).cross(ray2.m_direction);
	double k0 = left.dot(right) / right.dot(right);

	array<Eigen::Vector3f, 3> trigon = { Vector3f(1, 0, 0), Vector3f(0, 1, 0), Vector3f(0, 0, 0) };
	Vector3f normal = (trigon[1] - trigon[0]).cross(trigon[2] - -trigon[1]);
	double t;// = (trigon[0] - ray1.m_origin).cross(trigon[1] - trigon[0]).dot(normal) / ray1.m_direction.cross(trigon[1] - trigon[0]).dot(normal);
	Vector3f point = ray1.m_origin + t * ray1.m_direction;
	// two segment intersect
	array<Eigen::Vector3f, 2> segmA = { Vector3f(1, 0, 0), Vector3f(0, 1, 0) };
	array<Eigen::Vector3f, 2> segmB = { Vector3f(0, 0, 0), Vector3f(2, 1, 0) };
	Vector3f vecSeg = segmA[1] - segmA[0];
	normal = (segmA[1] - segmA[0]).cross(segmB[1] - segmB[0]);
	double k1= (segmA[0] - segmB[0]).cross(segmA[0] - segmB[1]).norm() / normal.norm(); //k
	double k2= (segmB[0] - segmA[0]).cross(vecSeg).dot(normal) / normal.squaredNorm();

	return;
}

bool IntersectRayWithAABB(const Eigen::Vector3f& ray_origin, const Eigen::Vector3f& ray_direction, const Eigen::AlignedBox3f& aabb)
{
	Eigen::Vector3f inv_dir = Eigen::Vector3f(1.0, 1.0, 1.0).cwiseQuotient(ray_direction); //coefficient-wise Quotient
	Eigen::Vector3f t0 = (aabb.min() - ray_origin).cwiseProduct(inv_dir);
	Eigen::Vector3f t1 = (aabb.max() - ray_origin).cwiseProduct(inv_dir);
	Eigen::Vector3f tmin_v = t0.cwiseMin(t1);
	Eigen::Vector3f tmax_v = t0.cwiseMax(t1);
	// Reduce to 1D problem -- find the overlapping t range for all axes
	double t_enter = tmin_v.maxCoeff();
	double t_exit = tmax_v.minCoeff();
	return t_enter <= t_exit && 0.0 < t_exit;
}

static void test1()
{
	// 0/0=nan
	// 1/0=inf
	double a0 = 0;
	double dnan = a0 / a0;
	double dinf = 1.0 / a0;
	double m1 = std::min(dnan, dinf);
	double m2 = std::min(dnan, 1.0);

	bool inter;
	Eigen::AlignedBox3f box(Vector3f(0, 0, 0), Vector3f(10, 10, 10));
	RayEquation ray1(Vector3f(5, 5, -1), Vector3f(0, 0, 1));
	inter = rayIntersectionwithAxisAlignedBoundingBox(ray1, box);
	RayEquation ray2(Vector3f(5, 5, 1), Vector3f(0, 0, 1));
	inter = rayIntersectionwithAxisAlignedBoundingBox(ray2, box);
	RayEquation ray3(Vector3f(5, 5, 11), Vector3f(0, 0, -1));
	inter = rayIntersectionwithAxisAlignedBoundingBox(ray3, box);
	inter = IntersectRayWithAABB(ray3.m_origin, ray3.m_direction, box);
	//
	RayEquation ray;
	ray = RayEquation(Vector3f(-1, 5, 0), Vector3f(3, 1, 0));
	inter = rayIntersectionwithAxisAlignedBoundingBox(ray, box);
	ray = RayEquation(Vector3f(11, 5, 0), Vector3f(-1, 1, 0));
	inter = rayIntersectionwithAxisAlignedBoundingBox(ray, box);
	inter = IntersectRayWithAABB(ray.m_origin, ray.m_direction, box);

	Eigen::Vector3f ray_origin(0, 0, 0);
	Eigen::Vector3f ray_direction(1, 0, 0); // 射线方向需要被归一化，如果不是，请先进行归一化处理
	Eigen::AlignedBox3f aabb(Eigen::Vector3f(-1, -1, -1), Eigen::Vector3f(1, 1, 1));

	double t_min, t_max;
	bool intersects = IntersectRayWithAABB(ray_origin, ray_direction, aabb);


	return;
}

static int enrol = []()->int
{
	//test0();
	test1();
	//test2();
	//test3();
	cout << "test_eigen finished.\n" << endl;
	return 0;
}();
