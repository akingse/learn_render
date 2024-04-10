#include "calculateTransform.h"
#include "Triangle.hpp"
#include "rasterizer.hpp"
#define _USE_MATH_DEFINES
#include<math.h>
//#include <eigen3/Eigen/Eigen>
#include <Eigen/Dense> //$(SolutionDir)..\TPL\eigen-3.4.0
#include <iostream>
#include <opencv2/opencv.hpp> //$(SolutionDir)..\TPL\vcpkg\packages\opencv4_x64-windows\include
//#pragma comment (lib, "opencv_core4.lib")// $(SolutionDir)..\TPL\vcpkg\packages\opencv4_x64-windows\lib
//使用vcpkg继承，无需指定加载lib


using namespace Eigen;
using namespace eigen;
//constexpr double MY_PI = 3.1415926;



int main(/*int argc, const char** argv*/)
{
    float angle = 0;
    std::string filename = "output.png";
    //bool command_line = false;
    //if (argc >= 3) 
    //{
    //    command_line = true;
    //    angle = std::stof(argv[2]); // -r by default
    //    if (argc == 4) 
    //        filename = std::string(argv[3]);
    //    else
    //        return 0;
    //}

    rst::Rasterizer r(700, 700);
    Eigen::Vector3f eye_pos = {0, 0, 5};
    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};
    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);
    int key = 0;
    int frame_count = 0;

    while (key != 27)  //ESC
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);
        r.set_model(get_model_matrix(Vector3f(1,1,1), angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));
        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        //opencv grammer
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);
        //std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') 
			angle += 10 * M_PI / 180;
        else if (key == 'd') 
			angle -= 10 * M_PI / 180;
        else if (key == 'w') 
            eye_pos.z() += 1;
        else if (key == 's') 
            eye_pos.z() -= 1;
    }

    return 0;
}
