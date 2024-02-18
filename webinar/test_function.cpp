#include "pch.h"
#include <iostream>

using namespace std;
using namespace Eigen;
using namespace games;
using namespace psykronix;

static void test0()
{
	RayEquation ray(Vector3d(0, 0, 0), Vector3d(-1, 0, 0));
	//RayEquation ray(Vector3d(0, 0, -1), Vector3d(0, 0, -1));
	PlaneEquation plane(Vector3d(0, 0, 0), Vector3d(0, 0, 1));
	std::tuple<bool, double> t0 = rayIntersectionWithPlane(ray, plane);

	Vector3d p0(1, 0, 0);
	Vector3d p1(0, 1, 0);
	Vector3d origin(0, 0, 0);
	Vector3d direct(1, 1, 0);
	Vector3d left = (origin - p0).cross(origin - p1);
	Vector3d right = (2*origin - p0 - p1).cross(direct);

	RayEquation ray1(Vector3d(0, 0, 0), Vector3d(2, 1, 0));
	RayEquation ray2(Vector3d(1, 0, 0), Vector3d(-1, 1, 0));

	left = (ray1.m_origin - ray2.m_origin).cross(ray2.m_direction);
	right = (ray1.m_direction).cross(ray2.m_direction);
	double k1 = left.dot(right) / right.dot(right);

	Triangle trigon = { Vector3d(1, 0, 0), Vector3d(0, 1, 0), Vector3d(0, 0, 0) };
	Vector3d normal = (trigon[1] - trigon[0]).cross(trigon[2] - -trigon[1]);
	double t = (trigon[0] - ray1.m_origin).cross(trigon[1] - trigon[0]).dot(normal) / ray1.m_direction.cross(trigon[1] - trigon[0]).dot(normal);
	Vector3d point = ray1.m_origin + t * ray1.m_direction;
	return;
}

static int enrol = []()->int
{
	test0();
	//test1();
	//test2();
	//test3();
	cout << "test_eigen finished.\n" << endl;
	return 0;
}();
