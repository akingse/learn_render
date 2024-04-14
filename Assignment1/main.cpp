#include "calculateTransform.h"
#include "Triangle.hpp"
#include "rasterizer.hpp"
#define _USE_MATH_DEFINES
#include<math.h>
#include <Eigen/Dense> //$(SolutionDir)..\TPL\eigen-3.4.0
#include <iostream>
#include <opencv2/opencv.hpp> //$(SolutionDir)..\TPL\vcpkg\packages\opencv4_x64-windows\include
//#pragma comment (lib, "opencv_core4.lib")// $(SolutionDir)..\TPL\vcpkg\packages\opencv4_x64-windows\lib

using namespace Eigen;
using namespace eigen;
using namespace rst;

//模拟一个基于 CPU 的光栅化渲染器的简化版本
int main(/*int argc, const char** argv*/)
{
    float angle = 0;

    rst::Rasterizer r(700, 700);
    Eigen::Vector3f eye_pos = {0, 0, 5};
    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};
    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};
    Mesh mesh(pos, ind);
    r.load_mesh(mesh);

    int key = 0;
    while (key != 27)  //ESC
    {
        r.clear();
		r.set_model(get_model_matrix(Vector3f(0, 0, 0), Vector3f(0, 0, 1), angle));
		r.set_view(get_viewing_matrix(eye_pos, -Vector3f(0, 0, 1), Vector3f(0, 1, 0)));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));
        r.draw();//Primitive::Triangle

        //opencv grammer
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        if (key == 'A') 
			angle += 10 * M_PI / 180;
        else if (key == 'D') 
			angle -= 10 * M_PI / 180;
        else if (key == 'W') 
            eye_pos.z() += 1;
        else if (key == 'S') 
            eye_pos.z() -= 1;
    }

    return 0;
}
