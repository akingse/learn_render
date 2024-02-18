#pragma once

namespace games
{
	class RayEquation
	{
		//r(t)=o+t*d (0<=t<oo)
	public:
		RayEquation() = default;
		RayEquation(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction) :
			m_origin(origin) , m_direction(direction) {}
		Eigen::Vector3d m_origin;
		Eigen::Vector3d m_direction; //normalized vector

	};

	class PlaneEquation
	{
		// (p-po)dot(N)==0 , ax+by+cz+d=0
	public:
		PlaneEquation() = default;
		PlaneEquation(const Eigen::Vector3d& origin, const Eigen::Vector3d& normal) :
			m_origin(origin), m_normal(normal) {}
		Eigen::Vector3d m_origin;
		Eigen::Vector3d m_normal;
	};

	struct Sphere
	{
	//public:
		double r;
		Eigen::Vector3d m_center;
	};

	enum class IntersectStatus
	{
		RAY_SEPARATE = 0,
		RAY_TANGENT,
		RAY_INTERSECT,
		RAY_NEGATIVE,
	};

}
//Ray-Surface Intersection

//Ray Intersection With Implicit Surface
std::array<Eigen::Vector3d, 2> rayIntersectionWithSphere(const games::RayEquation& ray, const games::Sphere& sphere);
//Ray Intersection With Triangle Mesh
std::tuple<bool, double> rayIntersectionWithTriangle(const games::RayEquation& ray, const psykronix::Triangle& triangle); // is intersect | ray t
//Ray Intersection With Plane
std::tuple<bool, double> rayIntersectionWithPlane(const games::RayEquation& ray, const games::PlaneEquation& plane);
