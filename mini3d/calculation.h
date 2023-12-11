#pragma once
#include "pch.h"
using namespace std;
using namespace Eigen;

namespace eigen
{
	// Eigen::Matrix4d specialize into Eigen::Affine3d
	inline Eigen::Matrix4d translate(double x, double y, double z = 0.0)
	{
		//Eigen::Matrix4d translate(const Eigen::Vector3d& vec);
		Matrix4d T;
		T << 1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1;
		return T;
	}
	inline Eigen::Matrix4d scale(double x, double y, double z = 1.0)
	{
		Matrix4d T;
		T << x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1;
		return T;
	}
	Eigen::Matrix4d rotate(const Eigen::Vector3d& axis = { 0, 0, 1 }, double theta = 0.0)
	{
		Quaterniond q = Quaterniond(AngleAxisd(theta, axis.normalized()));
		Matrix3d R = q.toRotationMatrix();
		Matrix4d T = Matrix4d::Identity();
		T.block<3, 3>(0, 0) = R;
		return T;
	}
	//Eigen::Vector3d operator*=(const Eigen::Matrix4d& mat, const Eigen::Vector3d& vec) //operator* been occupied
	//{
	//	Eigen::Vector4d res = mat * vec.homogeneous();
	//	return res.hnormalized();
	//}

	int middle(int x, int min, int max)
	{
		return (x < min) ? min : ((x > max) ? max : x);
	}

	double interpolate(double x1, double x2, double t)
	{
		return x1 + (x2 - x1) * t;
	}
	// vector + - * |v|

	Vector4d vector_interp(const Vector4d& x1, const Vector4d& x2, double t)
	{
		Vector4d z;
		z.x() = interpolate(x1.x(), x2.x(), t);
		z.y() = interpolate(x1.y(), x2.y(), t);
		z.z() = interpolate(x1.z(), x2.z(), t);
		z.w() = 1;
		return z;
	}

	void vector_interp(Vector4d& z, const Vector4d& x1, const Vector4d& x2, double t)
	{
		z.x() = interpolate(x1.x(), x2.x(), t);
		z.y() = interpolate(x1.y(), x2.y(), t);
		z.z() = interpolate(x1.z(), x2.z(), t);
		z.w() = 1;
	}

	// ÉèÖÃÉãÏñ»ú
	void matrix_set_lookat(Matrix4d* m, const Vector4d& eye, const Vector4d& at, const Vector4d& up)
	{
		Vector4d xaxis, yaxis, zaxis;

		zaxis = at - eye;
		zaxis.hnormalized().normalized().homogeneous();
		xaxis = up.cross3(zaxis);


		//m->m[0][0] = xaxis.x;
		//m->m[1][0] = xaxis.y;
		//m->m[2][0] = xaxis.z;
		//m->m[3][0] = -vector_dotproduct(&xaxis, eye);

		//m->m[0][1] = yaxis.x;
		//m->m[1][1] = yaxis.y;
		//m->m[2][1] = yaxis.z;
		//m->m[3][1] = -vector_dotproduct(&yaxis, eye);

		//m->m[0][2] = zaxis.x;
		//m->m[1][2] = zaxis.y;
		//m->m[2][2] = zaxis.z;
		//m->m[3][2] = -vector_dotproduct(&zaxis, eye);

		//m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
		//m->m[3][3] = 1.0f;
	}

	// Í¸ÊÓ¾ØÕó
	void matrix_set_perspective(Matrix4d* m, double fovy, double aspect, double zn, double zf)
	{
		*m = Matrix4d::Identity();
		double fax = 1.0f / (double)tan(fovy * 0.5f);
		m->operator[](0) = fax / aspect;
		m->operator[](5) = fax;
		m->operator[](10) = zf / (zf - zn);
		m->operator[](14) = 1 - zn * zf / (zf - zn);
		m->operator[](11) = 1.0f;
	}
}
