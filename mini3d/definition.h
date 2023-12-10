#pragma once
using namespace eigen;

namespace mini3d
{
	// class
	struct Transform // matrix transform of eigen
	{
		Matrix4d world;         // 世界坐标变换 model
		Matrix4d view;          // 摄影机坐标变换
		Matrix4d projection;    // 投影变换
		Matrix4d transform;     // transform = world * view * projection
		double w, h;             // 屏幕大小
	};
	struct color_t
	{
		double r, g, b;
	};
	struct texcoord_t
	{
		double u, v;
	};
	struct Vertex
	{
		Vector4d pos;
		texcoord_t tc;
		color_t color;
		double rhw;
	};

	struct edge_t
	{
		Vertex v, v1, v2;
	};
	struct trapezoid_t //梯形
	{
		double top, bottom;
		edge_t left, right;
	};
	struct scanline_t
	{
		Vertex v, step;
		int x, y, w;
	};

	struct Device {
		Transform transform;      // 坐标变换器
		int width;                  // 窗口宽度
		int height;                 // 窗口高度
		uint32_t** framebuffer;      // 像素缓存：framebuffer[y] 代表第 y行
		double** zbuffer;            // 深度缓存：zbuffer[y] 为第 y行指针
		uint32_t** texture;          // 纹理：同样是每行索引
		int tex_width;              // 纹理宽度
		int tex_height;             // 纹理高度
		double max_u;                // 纹理最大宽度：tex_width - 1
		double max_v;                // 纹理最大高度：tex_height - 1
		int render_state;           // 渲染状态
		uint32_t background;         // 背景颜色
		uint32_t foreground;         // 线框颜色
	};

	static constexpr uint32_t RENDER_STATE_WIREFRAME = 1;		// 渲染线框
	static constexpr uint32_t RENDER_STATE_TEXTURE = 2;		// 渲染纹理
	static constexpr uint32_t RENDER_STATE_COLOR = 4;		// 渲染颜色


	//Transform 成员函数
	// 矩阵更新，计算 transform = world * view * projection
	void transform_update(Transform* ts)
	{
		ts->transform = ts->world * ts->view * ts->transform;
	}

	// 初始化，设置屏幕长宽
	void transform_init(Transform* ts, int width, int height)
	{
		double aspect = (double)width / ((double)height);
		ts->world = Matrix4d::Identity();
		ts->view = Matrix4d::Identity();

		matrix_set_perspective(&ts->projection, 3.1415926f * 0.5f, aspect, 1.0f, 500.0f);
		ts->w = (double)width;
		ts->h = (double)height;
		transform_update(ts);
	}

	// 将矢量 x 进行 project 
	void transform_apply(const Transform* ts, Vector4d* y, const Vector4d* x)
	{
		*y = ts->transform * (*x);
	}

	// 检查齐次坐标同 cvv 的边界用于视锥裁剪
	int transform_check_cvv(const Vector4d* v) {
		double w = v->w();
		int check = 0;
		if (v->z() < 0.0f) check |= 1;
		if (v->z() > w) check |= 2;
		if (v->x() < -w) check |= 4;
		if (v->x() > w) check |= 8;
		if (v->y() < -w) check |= 16;
		if (v->y() > w) check |= 32;
		return check;
	}

	// 归一化，得到屏幕坐标
	void transform_homogenize(const Transform* ts, Vector4d* y, const Vector4d* x)
	{
		double rhw = 1.0f / x->w();
		y->x() = (x->x() * rhw + 1.0f) * ts->w * 0.5f;
		y->y() = (1.0f - x->y() * rhw) * ts->h * 0.5f;
		y->z() = x->z() * rhw;
		y->w() = 1.0f;
	}

	//顶点计算
	void vertex_rhw_init(Vertex* v)
	{
		double rhw = 1.0f / v->pos.w();
		v->rhw = rhw;
		v->tc.u *= rhw;
		v->tc.v *= rhw;
		v->color.r *= rhw;
		v->color.g *= rhw;
		v->color.b *= rhw;
	}

	void vertex_interp(Vertex* y, const Vertex* x1, const Vertex* x2, double t)
	{
		vector_interp(y->pos, x1->pos, x2->pos, t);
		y->tc.u = interpolate(x1->tc.u, x2->tc.u, t);
		y->tc.v = interpolate(x1->tc.v, x2->tc.v, t);
		y->color.r = interpolate(x1->color.r, x2->color.r, t);
		y->color.g = interpolate(x1->color.g, x2->color.g, t);
		y->color.b = interpolate(x1->color.b, x2->color.b, t);
		y->rhw = interpolate(x1->rhw, x2->rhw, t);
	}

	void vertex_division(Vertex* y, const Vertex* x1, const Vertex* x2, double w)
	{
		double inv = 1.0f / w;
		y->pos.x() = (x2->pos.x() - x1->pos.x()) * inv;
		y->pos.y() = (x2->pos.y() - x1->pos.y()) * inv;
		y->pos.z() = (x2->pos.z() - x1->pos.z()) * inv;
		y->pos.w() = (x2->pos.w() - x1->pos.w()) * inv;
		y->tc.u = (x2->tc.u - x1->tc.u) * inv;
		y->tc.v = (x2->tc.v - x1->tc.v) * inv;
		y->color.r = (x2->color.r - x1->color.r) * inv;
		y->color.g = (x2->color.g - x1->color.g) * inv;
		y->color.b = (x2->color.b - x1->color.b) * inv;
		y->rhw = (x2->rhw - x1->rhw) * inv;
	}

	//Vertex operator+(Vertex& y, const Vertex& x);
	void vertex_add(Vertex* y, const Vertex* x)
	{
		y->pos.x() += x->pos.x();
		y->pos.y() += x->pos.y();
		y->pos.z() += x->pos.z();
		y->pos.w() += x->pos.w();
		y->rhw += x->rhw;
		y->tc.u += x->tc.u;
		y->tc.v += x->tc.v;
		y->color.r += x->color.r;
		y->color.g += x->color.g;
		y->color.b += x->color.b;
	}


	// 根据三角形生成 0-2 个梯形，并且返回合法梯形的数量
	int trapezoid_init_triangle(trapezoid_t* trap, const Vertex* p1, const Vertex* p2, const Vertex* p3)
	{
		const Vertex* p;
		double k, x;


		return 2;
	}

	// 按照 Y 坐标计算出左右两条边纵坐标等于 Y 的顶点
	void trapezoid_edge_interp(trapezoid_t* trap, double y)
	{
		double s1 = trap->left.v2.pos.y() - trap->left.v1.pos.y();
		double s2 = trap->right.v2.pos.y() - trap->right.v1.pos.y();
		double t1 = (y - trap->left.v1.pos.y()) / s1;
		double t2 = (y - trap->right.v1.pos.y()) / s2;
		vertex_interp(&trap->left.v, &trap->left.v1, &trap->left.v2, t1);
		vertex_interp(&trap->right.v, &trap->right.v1, &trap->right.v2, t2);
	}

	// 根据左右两边的端点，初始化计算出扫描线的起点和步长
	void trapezoid_init_scan_line(const trapezoid_t* trap, scanline_t* scanline, int y)
	{
		double width = trap->right.v.pos.x() - trap->left.v.pos.x();
		scanline->x = (int)(trap->left.v.pos.x() + 0.5f);
		scanline->w = (int)(trap->right.v.pos.x() + 0.5f) - scanline->x;
		scanline->y = y;
		scanline->v = trap->left.v;
		if (trap->left.v.pos.x() >= trap->right.v.pos.x()) scanline->w = 0;
		vertex_division(&scanline->step, &trap->left.v, &trap->right.v, width);
	}
}