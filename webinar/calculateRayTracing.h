#pragma once

namespace games
{
	class RayEquation
	{
		//r(t)=o+t*d (0<=t<oo)
	public:
		RayEquation() = default;
		RayEquation(const Eigen::Vector3f& origin, const Eigen::Vector3f& direction) :
			m_origin(origin) , m_direction(direction) {}
		Eigen::Vector3f m_origin;
		Eigen::Vector3f m_direction; //normalized vector

	};

	class PlaneEquation
	{
		// (p-po)dot(N)==0 , ax+by+cz+d=0
	public:
		PlaneEquation() = default;
		PlaneEquation(const Eigen::Vector3f& origin, const Eigen::Vector3f& normal) :
			m_origin(origin), m_normal(normal) {}
		Eigen::Vector3f m_origin;
		Eigen::Vector3f m_normal;
	};

	struct Sphere
	{
	//public:
		double r;
		Eigen::Vector3f m_center;
	};

	enum class IntersectStatus
	{
		RAY_SEPARATE = 0,
		RAY_TANGENT,
		RAY_INTERSECT,
		RAY_NEGATIVE,
	};

}

//inline function
namespace games
{
	//ignore nan and inf
	inline double std_min(double a, double b)
	{
		if (std::isnan(a) || std::isinf(a))
			return b;
		if (std::isnan(b) || std::isinf(b))
			return a;
		return std::min(a, b);
	}
	inline double std_max(double a, double b)
	{
		if (std::isnan(a) || std::isinf(a))
			return b;
		if (std::isnan(b) || std::isinf(b))
			return a;
		return std::max(a, b);
	}
	inline double std_min(double a, double b, double c)
	{
		//return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
		//return std::min(std::min(a, b), c);
		return std_min(std_min(a, b), c);
	}
	inline double std_max(double a, double b, double c)
	{
		//return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
		//return std::max(std::max(a, b), c);
		return std_max(std_max(a, b), c);
	}

}

//Ray-Surface Intersection

//Ray Intersection With Implicit Surface
std::array<Eigen::Vector3f, 2> rayIntersectionWithSphere(const games::RayEquation& ray, const games::Sphere& sphere);
//Ray Intersection With Triangle Mesh
std::tuple<bool, double> rayIntersectionWithTriangle(const games::RayEquation& ray, const std::array<Eigen::Vector3f, 3>& triangle); // is intersect | ray t
//Ray Intersection With Plane
std::tuple<bool, double> rayIntersectionWithPlane(const games::RayEquation& ray, const games::PlaneEquation& plane);
//Accelerating Ray - Surface Intersection
bool rayIntersectionwithAxisAlignedBoundingBox(const games::RayEquation& ray, const Eigen::AlignedBox3f& box);
//Build Acceleration Grid
void raySceneIntersection();