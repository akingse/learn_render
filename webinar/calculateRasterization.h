#pragma once
namespace games
{
	class RayEquation
	{
	public:
		//r(t)=o+t*d (0<=t<oo)
		Eigen::Vector3d m_origin;
		Eigen::Vector3d m_direction; //normalized vector

	};
	class Sphere
	{
	public:
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

