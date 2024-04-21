#include"pch.h"

using namespace std;
//using namespace Eigen;

#if 0
struct Vec3f
{
	float x;
	float y;
	float z;
	Vec3f() = default;
	Vec3f(float _x, float _y, float _z);
	Vec3f normalize() const
	{
		float k = x * x + y * y + z * z;
		if (k == 0)
			return *this;
		Vec3f c;
		k = 1.f / sqrt(k);
		c.x = k * x;
		c.y = k * y;
		c.z = k * z;
		return c;
	}
	Vec3f operator+(const Vec3f& rhs)
	{
		Vec3f c;
		c.x = x + rhs.x;
		c.y = x + rhs.y;
		c.z = x + rhs.z;
		return c;
	}
	Vec3f operator-(const Vec3f& rhs)
	{
		Vec3f c;
		c.x = x - rhs.x;
		c.y = x - rhs.y;
		c.z = x - rhs.z;
		return c;
	}
};

Vec3f cross(const Vec3f& x, const Vec3f& y)
{
	Vec3f c;
	c.x = x.y * y.z - x.z * y.y;
	c.y = x.z * y.x - x.x * y.z;
	c.z = x.x * y.y - x.y * y.x;
	return c;
}

struct Matrix
{
	float mat[4][4];
	Matrix() = default;
	Matrix(const array<float, 4>& r0, const array<float, 4>& r1, const array<float, 4>& r2, const array<float, 4>& r3)
	{
		for (int i = 0; i < 4; ++i)
		{
			mat[0][i] = r0[i];
			mat[1][i] = r1[i];
			mat[2][i] = r2[i];
			mat[3][i] = r3[i];
		}
	}
	//static Matrix Identify()
	//{
	//	Matrix identify;// = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
	//	for (int i = 0; i < 4; ++i)
	//		for (int j = 0; j < 4; ++j)
	//			identify.mat[i][j] = (i==j) ? 1.f : 0.f;
	//	return identify;
	//}

};

Matrix operator*(const Matrix& a, const Matrix& b)
{
	Matrix multi;
	for (int i = 0; i < 4; ++i) 
	{
		for (int j = 0; j < 4; ++j) 
		{
			multi.mat[j][i] = 
				(a.mat[j][0] * b.mat[0][i]) +
				(a.mat[j][1] * b.mat[1][i]) +
				(a.mat[j][2] * b.mat[2][i]) +
				(a.mat[j][3] * b.mat[3][i]);
		}
	}
	return multi;
}

//from //https://zhuanlan.zhihu.com/p/663950402?utm_id=0
Matrix g_model;
Matrix g_view;
Matrix g_proj;
Matrix g_screen;
int g_width, g_height;

//Model矩阵
void set_model_mat(Vec3f rot, Vec3f scale, Vec3f trans) {
	Matrix g_scale({ {scale.x,0,0,0},/**/ {0,scale.y,0,0},/**/ {0,0,scale.z,0},/**/ {0,0,0,1 } });
	Matrix g_trans({ {1,0,0,trans.x},/**/ {0,1,0,trans.y},/**/ {0,0,1,trans.z},/**/ {0,0,0,1 } });
	Matrix g_rot_x({ {1,0,0,0},/**/ {0,std::cos(rot.x),-std::sin(rot.x),0},/**/ {0,std::sin(rot.x),std::cos(rot.x),0},/**/ {0,0,0,1 } });
	Matrix g_rot_y({ {std::cos(rot.y),0,std::sin(rot.y),0},/**/ {0,1,0,0},/**/ {-std::sin(rot.y),0,std::cos(rot.y),0},/**/ {0,0,0,1 } });
	Matrix g_rot_z({ {std::cos(rot.z),-std::sin(rot.z),0,0},/**/ {std::sin(rot.z),std::cos(rot.z),0,0},/**/ {0,0,1,0},/**/ {0,0,0,1} });
	g_model = g_trans * (g_rot_x * g_rot_y * g_rot_z) * g_scale;
}

//View矩阵
//up一般是(0,1,0)即正y
//view matrix是将相机放到原点，(ex,ey,ez)分别旋转到(1,0,0)(0,1,0)(0,0,1)的位置，同时也将物体进行了变换，
//让物体和相机的相对位置不变
void set_view_mat(Vec3f eye, Vec3f at, Vec3f up) 
{
	Matrix g_trans({ {1,0,0,-eye.x},/**/ {0,1,0,-eye.y},/**/ {0,0,1,-eye.z},/**/ {0,0,0,1 } });
	//先确定相机的ex,ey,ez，是右手系
	Vec3f ez = (eye - at).normalize(); // z方向是eye-at，屏幕朝外
	Vec3f ex = cross(up, ez).normalize();
	Vec3f ey = cross(ez, ex).normalize();
	//先逆向考虑把z挪到ez，x挪到ex，y挪到ey
	//相当于把(0,0,1)挪到ez，(1,0,0)挪到ex，(0,1,0)挪到ey，即求A[(1,0,0)T,(0,1,0)T, (0,0,1)T] = [ex, ey, ez]
	//所以A = [ex,ey,ez]
	//所以反过来把相机挪到原点作为相机坐标系的话，则是A的逆矩阵，即A的转置
	Matrix g_rot({ {ex.x, ex.y, ex.z,0}, {ey.x, ey.y, ey.z,0}, {ez.x, ez.y, ez.z, 0}, {0,0,0,1} });
	g_view = g_rot * g_trans;
}

//这里是将视锥体转为裁剪空间，透视除法后变成NDC空间
void set_proj_mat(float near, float far, float left, float right, float bottom, float top) 
{
	Matrix g_ortho_scale({ {2.f / (right - left),0,0,0},{0,2.f / (top - bottom),0,0},{0,0,2.f / (near - far),0},{0,0,0,1} });
	Matrix g_ortho_trans({ {1,0,0,-(right + left) / 2.f},{0,1,0,-(top + bottom) / 2.f},{0,0,1,-(near + far) / 2.f},{0, 0,0,1} });
	Matrix g_ortho = g_ortho_scale * g_ortho_trans;

	//先把视锥体压缩成长方体,压缩x和y，长方体的大小是近平面的大小，有 A(x,y,z,1) = (xn/z,yn/z,unknown,1) = (nx,ny,unknown,z)
	//然后推出A=[{n,0,0,0}, {0,n,0,0}{unknown}{0,0,1,0}]\
	//然后将z设为n，近平面上的点z不变A(x,y,n,1) = (x,y,n,1) = (nx,ny,n2, n),将z设为f，
		//远平面的点z不变A(x,y,f,1)=(nx/f,ny/f,f,1)=(nx,ny,f2,f)
	//=> 设第三行为(0,0,A,B) An + B = n2, Af + B = f2,因此第三行为 A = n + f， B = -nf
	Matrix g_persp2ortho({ {near,0,0,0},{0,near,0,0},{0,0,near + far,-near * far},{0, 0,1,0} });
	g_proj = g_ortho * g_persp2ortho;
}

//变换x和y到[0,width]x[0,height]，让z映射到0-1的区间
void set_screen_mat() 
{
	g_screen = Matrix({ 
		{g_width * .5f, 0,0,g_width * .5f,},
		{0,g_height * .5f,0,g_height * .5f,},
		{0,0,.5f,.5f},
		{0,0,0,1} });
}
#endif
