//
// Created by goksu on 4/6/19.
//
#pragma once

namespace rst //rasterizer 光栅器
{
	struct Mesh
	{
		Mesh() = default;
		Mesh(const std::vector<Eigen::Vector3f>& vbo, const std::vector<Eigen::Vector3i>& ibo) 
			:m_vbo(vbo), m_ibo(ibo) {}
		std::vector<Eigen::Vector3f> m_vbo;
		std::vector<Eigen::Vector3i> m_ibo; //std::array<int, 3>
		std::vector<Eigen::Vector3f> m_col;
		Eigen::Matrix4f m_mat = Eigen::Matrix4f::Identity();
		std::vector<Eigen::Vector3f> vbo() const
		{
			std::vector<Eigen::Vector3f> vbo;
			for (const auto& iter : m_vbo)
				vbo.push_back((m_mat * iter.homogeneous()).hnormalized());
			return vbo;
		}
		Eigen::AlignedBox3f getBox() const
		{
			Eigen::AlignedBox3f box;
			for (const auto& iter : m_vbo)
				box.extend((m_mat * iter.homogeneous()).hnormalized());
			return box;
		}
	};

	class vector3List
	{
	public:
		//std::vector<std::array<float, 3>> xyz;
		std::vector<std::tuple<float, float, float>> coorVct;
		std::set<std::tuple<float, float, float>> coorSet;
		std::map<std::tuple<float, float, float>,int> coorMap;
		vector3List(const std::vector<Eigen::Vector3f>& coords)
		{ 
			coorVct.resize(coords.size());
			for (int i = 0; i < coords.size(); ++i)
			{
				std::tuple<float, float, float> key = { coords[i][0], coords[i][1], coords[i][2] };
				coorVct[i] = key;
				coorSet.insert(key);
				if (coorMap.find(key) != coorMap.end())
					coorMap[key]++;
				else
					coorMap.emplace(key, 1);
			}
		}
	};

	enum class Mode
	{
		Wireframe = 1,
		Shadering = 2,
		Shader_SSAA = 3,
	};

	static const Eigen::Vector3f colorWhite{ 255, 255, 255 };

	//rasterizer 光栅(Raster)由像素构成的一个矩形网格。
	class Rasterizer
	{
	public:
		Rasterizer(int w, int h, float n, float f);
		void clear();

		void load_mesh(const Mesh& mesh)
		{
			pos_buf.emplace(0, mesh.vbo());
			ind_buf.emplace(0, mesh.m_ibo);
			col_buf.emplace(0, mesh.m_col);
		}
		//int load_colors(const std::vector<Eigen::Vector3f>& colors)
		//{
		//	int id = next_id++;
		//	col_buf.emplace(id, colors);
		//	return id;
		//}

		void set_model(const Eigen::Matrix4f& m) {	model = m; }
		void set_view(const Eigen::Matrix4f& v) { view = v; }
		void set_projection(const Eigen::Matrix4f& p) { projection = p; }
		void set_clip_space(float n, float f) { zNear = n; zFar = f; }//l r b t
		//texture
		void set_texture(Texture tex) { texture = tex; }
		void set_vertex_shader(std::function<Eigen::Vector3f(vertex_shader_payload)> vert_shader) { vertex_shader = vert_shader; };
		void set_fragment_shader(std::function<Eigen::Vector3f(fragment_shader_payload)> frag_shader) { fragment_shader = frag_shader; };

		void set_pixel_color(const Eigen::Vector3f& point, const Eigen::Vector3f& color);//frame_buf
		void draw(Mode mode);
		void draw(std::vector<Triangle*>& TriangleList);
		std::vector<Eigen::Vector3f>& frame_buffer() { return frame_buf; } //output to opencv

	private:
		void draw_line(Eigen::Vector3f begin, Eigen::Vector3f end); //draw_segment
		void rasterize_triangle(const Triangle& t, const std::array<Eigen::Vector3f, 3>& view_pos);
		void rasterize_wireframe(const Triangle& t);
		void rasterize_triangle(const Triangle& t);
		void rasterize_triangle_ssaa(const Triangle& t);

	private:
		Eigen::Matrix4f model;
		Eigen::Matrix4f view;
		Eigen::Matrix4f projection;
		std::map<int, std::vector<Eigen::Vector3f>> pos_buf; //vbo
		std::map<int, std::vector<Eigen::Vector3i>> ind_buf; //ibo
		std::map<int, std::vector<Eigen::Vector3f>> nor_buf; //nbo
		std::map<int, std::vector<Eigen::Vector3f>> col_buf; //color
		std::optional<Texture> texture; //C++17, to avoid empty
		std::function<Eigen::Vector3f(fragment_shader_payload)> fragment_shader;
		std::function<Eigen::Vector3f(vertex_shader_payload)> vertex_shader;

		// one dimension vector, size=width*height
		int width, height;
		float zNear, zFar;//for clip space
		std::vector<Eigen::Vector3f> frame_buf; //record color of pixel (0-255)
		std::vector<float> depth_buf; //record zbuffer of pixel
		std::vector<std::array<Eigen::Vector3f,4>> frame_buf_2xSSAA;//super-sampling Anti-aliasing
		std::vector<std::array<float,4>> depth_buf_2xSSAA;

		int get_index(int x, int y) { return (height - 1 - y) * width + x; } //Rasterizer origin at left-down, opencv origin at left-up
		//int next_id = 0;
		//int get_next_id() { return next_id++; }


	};
} // namespace rst
