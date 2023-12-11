#include "Triangle.hpp"
#include "rasterizer.hpp"
//#include <eigen3/Eigen/Eigen>
#include <Eigen/Dense> //$(SolutionDir)..\TPL\eigen-3.4.0
#include <iostream>
#include <opencv2/opencv.hpp> //$(SolutionDir)..\TPL\vcpkg\packages\opencv4_x64-windows\include
//#pragma comment (lib, "opencv_core4.lib")// $(SolutionDir)..\TPL\vcpkg\packages\opencv4_x64-windows\lib
//使用vcpkg继承，无需指定加载lib

// 知乎参考 https://zhuanlan.zhihu.com/p/425153734

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(const Eigen::Vector3f& eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0],
                0, 1, 0, -eye_pos[1],
		        0, 0, 1, -eye_pos[2],
                0, 0, 0, 1;
    view = translate * view;
    return view;
}

Eigen::Matrix4f get_model_matrix(const Eigen::Vector3f& axis, float theta=0) //using rad
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    // Create the model matrix for rotating the triangle around the Z axis.
    model << cos(theta), -sin(theta), 0, 0,
            sin(theta), cos(theta), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;
    // any axis rot
    Quaternionf q = Quaternionf(AngleAxisf(theta, axis.normalized()));
    Matrix3f R = q.toRotationMatrix();
    model.block<3, 3>(0, 0) = R;
    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    // Create the projection matrix for the given parameters.
    float ty = -1.0f / tan(eye_fov / 2.0f); //field of view
    projection << (ty / aspect_ratio), 0, 0, 0,
                    0, ty, 0, 0,
                    0, 0, (zNear + zFar) / (zNear - zFar), (-2 * zNear * zFar) / (zNear - zFar),
                    0, 0, 1, 0;
    return projection;
}

Eigen::Matrix4f get_orthographic_matrix(float l, float b, float n, float r, float t, float f) //left bottom near right top far
{
    Eigen::Matrix4f Mscale, Mtranslate;
	Mscale << 2.0f / (r - l), 0, 0, 0,
            0, 2.0f / (t - b), 0, 0,
            0, 0, 2.0f / (n - f), 0,
            0, 0, 0, 1;
    Mtranslate << 1, 0, 0, -0.5f * (r - l),
		        0, 1, 0, -0.5f * (t - b),
		        0, 0, 1, -0.5f * (n - f),
		        0, 0, 0, 1;
	return Mscale * Mscale;
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) 
    {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    rst::Rasterizer r(700, 700);
    Eigen::Vector3f eye_pos = {0, 0, 5};
    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};
    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);
    int key = 0;
    int frame_count = 0;

    //if (command_line) 
    //{
    //    r.clear(rst::Buffers::Color | rst::Buffers::Depth);
    //    r.set_model(get_model_matrix(angle));
    //    r.set_view(get_view_matrix(eye_pos));
    //    r.set_projection(get_projection_matrix(45, 1, 0.1, 50));
    //    r.draw(pos_id, ind_id, rst::Primitive::Triangle);
    //    cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
    //    image.convertTo(image, CV_8UC3, 1.0f);
    //    cv::imwrite(filename, image);
    //    return 0;
    //}

    while (key != 27)  //ESC
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);
        r.set_model(get_model_matrix(Vector3f(1,1,1), angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));
        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);
        //std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') 
            angle += 10*MY_PI/180;
        else if (key == 'd') 
            angle -= 10* MY_PI / 180;
        else if (key == 'w') 
            eye_pos.z() += 1;
        else if (key == 's') 
            eye_pos.z() -= 1;


    }

    return 0;
}
