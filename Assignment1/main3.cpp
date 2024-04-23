#include"pch.h"
//#include "rasterizer.hpp"
//#include "Triangle.hpp"
//#include "Shader.hpp"
//#include "Texture.hpp"
using namespace std;
using namespace rst;
using namespace eigen;
using namespace Eigen;

// argument count //argument  value
int main(int argc, const char** argv)
{
	//read OBJ
	std::string filename = "output.png";
	std::string obj_path = "C:/Users/wangk/source/repos/learn_render/Assignment1/models/"; //fix path

	//Rasterizer
	int sz_width = 700;
	int sz_height = 700;
	float zNear = 0.1;
	float zFar = 50;
	float aspect_ratio = (float)sz_width / sz_height;
	Eigen::Vector3f eye_pos = { 0,0,3 };
	float angle = 140.0;
	rst::Rasterizer r(sz_width, sz_height, zNear, zFar);
	string texture_path = "spot/hmap.jpg";
	//std::vector<Mesh> meshVct = loadMeshs(obj_path + "spot/spot_triangulated_good.obj");
	std::vector<Triangle*> TriangleList = loadTriangles(obj_path + "spot/spot_triangulated_good.obj");
	//std::vector<Triangle*> TriangleList = loadTriangles(obj_path + "bunny/bunny.obj");
	r.load_mesh(TriangleList);
	r.set_texture(Texture(obj_path + texture_path));

	std::function<Eigen::Vector3f(fragment_shader_payload)> active_shader;// = phong_fragment_shader;
	argv[2] = "displacement";
	if (argc == 3)
	{
		if (std::string(argv[2]) == "texture")
		{
			std::cout << "Rasterizing using the texture shader\n";
			active_shader = texture_fragment_shader;
			texture_path = "spot/spot_texture.png";
			r.set_texture(Texture(obj_path + texture_path));
		}
		else if (std::string(argv[2]) == "normal")
		{
			std::cout << "Rasterizing using the normal shader\n";
			active_shader = normal_fragment_shader;
		}
		else if (std::string(argv[2]) == "phong")
		{
			std::cout << "Rasterizing using the phong shader\n";
			active_shader = phong_fragment_shader;
		}
		else if (std::string(argv[2]) == "bump")
		{
			std::cout << "Rasterizing using the bump shader\n";
			active_shader = bump_fragment_shader;
		}
		else if (std::string(argv[2]) == "displacement")
		{
			std::cout << "Rasterizing using the displacement shader\n";
			active_shader = displacement_fragment_shader;
		}
	}

	//r.set_vertex_shader(vertex_shader);
	r.set_fragment_shader(active_shader);

	int key = 0;
	int frame_count = 0;
	while (key != 27)
	{
		r.clear();
		r.set_model(get_model_matrix(Vector3f(0, 0, 0), Vector3f(0, 1, 0), angle));
		r.set_view(get_viewing_matrix(eye_pos, Vector3f(0, 0, -1), Vector3f(0, 1, 0)));
		r.set_projection(get_projection_matrix(45* M_PI / 180, aspect_ratio, zNear, zFar));

		//r.draw(pos_id, ind_id, col_id, rst::Primitive::Triangle);
		r.draw(Mode::Texture);
		cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
		image.convertTo(image, CV_8UC3, 1.0f);
		cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
		cv::imshow("image", image);
		//cv::imwrite(filename, image);
		key = cv::waitKey(10);

		if (key == 'A')
			angle -= 0.1;
		else if (key == 'D')
			angle += 0.1;
		else if (key == 'W') //up
			eye_pos.z() += 1;
		else if (key == 'S') //down
			eye_pos.z() -= 1;
	}
	return 0;
}
