#include "calculateTransform.h"
#include "Triangle.hpp"
#include "rasterizer.hpp"
#define _USE_MATH_DEFINES
#include<math.h>
#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif // !M_PI

#include <Eigen/Dense> //$(SolutionDir)..\TPL\eigen-3.4.0
#include <iostream>
#include <opencv2/opencv.hpp> //$(SolutionDir)..\TPL\vcpkg\packages\opencv4_x64-windows\include
//#pragma comment (lib, "opencv_core4.lib")// $(SolutionDir)..\TPL\vcpkg\packages\opencv4_x64-windows\lib

using namespace Eigen;
using namespace eigen;
using namespace std;
using namespace rst;

Mesh getUnitCube()
{
    Mesh mesh;
    std::vector<Eigen::Vector3f> vbo{
	    {-0.5f, -0.5f, 0.5f},  // 0
	    {0.5f, -0.5f, 0.5f},   // 1
	    {0.5f, 0.5f, 0.5f},    // 2
	    {-0.5f, 0.5f, 0.5f},   // 3
	    {-0.5f, -0.5f, -0.5f}, // 4
	    {0.5f, -0.5f, -0.5f},  // 5
	    {0.5f, 0.5f, -0.5f},   // 6
	    {-0.5f, 0.5f, -0.5f},  // 7
    };
    std::vector<Eigen::Vector3i> ibo{
        {0, 1, 2},
        {2, 3, 0},
        {4, 0, 3},
        {3, 7, 4},
        {5, 4, 7},
        {7, 6, 5},
        {1, 5, 6},
        {6, 2, 1},
        {3, 2, 6},
        {6, 7, 3},
        {4, 5, 1},
        {1, 0, 4} };
    mesh.m_vbo = vbo;
    mesh.m_ibo = ibo;
    return mesh;
}

//模拟一个基于 CPU 的光栅化渲染器的简化版本
int main(/*int argc, const char** argv*/)
{
    int sz_width = 700;
    int sz_height = 500;
    float aspect_ratio = (float)sz_width / sz_height;
    rst::Rasterizer r(sz_width, sz_height);
    //from up direction 
    Eigen::Vector3f eye_pos = {0, 0, 10};

    //model
    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};
    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};
    //Mesh mesh(pos, ind);
    Mesh mesh = getUnitCube();
    //mesh.m_mat = scale(30, 20);
    //Eigen::AlignedBox3f box = mesh.getBox();
    r.load_mesh(mesh);

    int key = 0;
    float angle = 0;
    while (key != 27)  //ESC
    {
        r.clear();
		r.set_model(get_model_matrix(Vector3f(0, 0, 0), Vector3f(1, 1,0), angle));
		r.set_view(get_viewing_matrix(eye_pos, Vector3f(0, 0, -1), Vector3f(0, 1, 0)));
		r.set_projection(get_projection_matrix(45 * M_PI / 180, aspect_ratio, 0.1, 50));
        //r.set_projection(get_projection_matrix(-1, 1, -1, 1, 1, 50));
        r.draw();//Primitive::Triangle
        vector3List debug_show = vector3List(r.frame_buffer());
        //opencv interface
		cv::Mat image(sz_height, sz_width, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);
        //cout << key << endl;
        if (key == 'A') //left
			angle += 10 * M_PI / 180;
        else if (key == 'D') //right
			angle -= 10 * M_PI / 180;
        else if (key == 'W') //up
            eye_pos.z() += 1;
        else if (key == 'S') //down
            eye_pos.z() -= 1;
    }

    return 0;
}
