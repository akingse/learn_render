#pragma once
using namespace eigen;

namespace mini3d
{
	// class
	struct Transform // matrix transform of eigen
	{
		Matrix4d world;         // ��������任 model
		Matrix4d view;          // ��Ӱ������任
		Matrix4d projection;    // ͶӰ�任
		Matrix4d transform;     // transform = world * view * projection
		double w, h;             // ��Ļ��С
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
	struct trapezoid_t //����
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
		Transform transform;      // ����任��
		int width;                  // ���ڿ��
		int height;                 // ���ڸ߶�
		uint32_t** framebuffer;      // ���ػ��棺framebuffer[y] ����� y��
		double** zbuffer;            // ��Ȼ��棺zbuffer[y] Ϊ�� y��ָ��
		uint32_t** texture;          // ����ͬ����ÿ������
		int tex_width;              // ������
		int tex_height;             // ����߶�
		double max_u;                // ��������ȣ�tex_width - 1
		double max_v;                // �������߶ȣ�tex_height - 1
		int render_state;           // ��Ⱦ״̬
		uint32_t background;         // ������ɫ
		uint32_t foreground;         // �߿���ɫ
	};

	static constexpr uint32_t RENDER_STATE_WIREFRAME = 1;		// ��Ⱦ�߿�
	static constexpr uint32_t RENDER_STATE_TEXTURE = 2;		// ��Ⱦ����
	static constexpr uint32_t RENDER_STATE_COLOR = 4;		// ��Ⱦ��ɫ


	//Transform ��Ա����
	// ������£����� transform = world * view * projection
	void transform_update(Transform* ts)
	{
		ts->transform = ts->world * ts->view * ts->transform;
	}

	// ��ʼ����������Ļ����
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

	// ��ʸ�� x ���� project 
	void transform_apply(const Transform* ts, Vector4d* y, const Vector4d* x)
	{
		*y = ts->transform * (*x);
	}

	// ����������ͬ cvv �ı߽�������׶�ü�
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

	// ��һ�����õ���Ļ����
	void transform_homogenize(const Transform* ts, Vector4d* y, const Vector4d* x)
	{
		double rhw = 1.0f / x->w();
		y->x() = (x->x() * rhw + 1.0f) * ts->w * 0.5f;
		y->y() = (1.0f - x->y() * rhw) * ts->h * 0.5f;
		y->z() = x->z() * rhw;
		y->w() = 1.0f;
	}

	//�������
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


	// �������������� 0-2 �����Σ����ҷ��غϷ����ε�����
	int trapezoid_init_triangle(trapezoid_t* trap, const Vertex* p1, const Vertex* p2, const Vertex* p3)
	{
		const Vertex* p;
		double k, x;


		return 2;
	}

	// ���� Y ��������������������������� Y �Ķ���
	void trapezoid_edge_interp(trapezoid_t* trap, double y)
	{
		double s1 = trap->left.v2.pos.y() - trap->left.v1.pos.y();
		double s2 = trap->right.v2.pos.y() - trap->right.v1.pos.y();
		double t1 = (y - trap->left.v1.pos.y()) / s1;
		double t2 = (y - trap->right.v1.pos.y()) / s2;
		vertex_interp(&trap->left.v, &trap->left.v1, &trap->left.v2, t1);
		vertex_interp(&trap->right.v, &trap->right.v1, &trap->right.v2, t2);
	}

	// �����������ߵĶ˵㣬��ʼ�������ɨ���ߵ����Ͳ���
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