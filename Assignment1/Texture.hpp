//
// Created by LEI XU on 4/27/19.
// head-only
#pragma once

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H

class Texture
{
private:
    cv::Mat image_data; //opencv matrix

public:
    Texture(const std::string& name) //read image uv-color using opencv
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;
    inline Eigen::Vector3f getColor(float u, float v) const
    {
        int u_img = u * width;
        int v_img = (1 - v) * height;
        const cv::Vec3b& color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

};
#endif //RASTERIZER_TEXTURE_H
