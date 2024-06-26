//
// Created by LEI XU on 4/27/19.
// head-only
#pragma once
#include"pch.h"

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H

class Texture
{
private:
    cv::Mat image_data; //opencv matrix

public:
    int width = 0, height = 0;
    Texture(const std::string& name) //read image uv-color using opencv
    {
        image_data = cv::imread(name);
        if (image_data.empty())
            return;
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    inline Eigen::Vector3f getColor(const Eigen::Vector2f& uv) const
    {
        if (image_data.empty())
			return Eigen::Vector3f(0, 0, 0);
        float u = uv[0];
        float v = uv[1];
        int u_img = u * width;
        int v_img = (1 - v) * height;
        if (u_img < 0) //safe check
            u_img = 0;
        if (u_img >= width) 
            u_img = width - 1;
        if (v_img < 0) 
            v_img = 0;
        if (v_img >= height) 
            v_img = height - 1;
        const cv::Vec3b& color = image_data.at<cv::Vec3b>(v_img, u_img);
		return 1.0f / 255 * Eigen::Vector3f(color[0], color[1], color[2]);
    }

};
#endif //RASTERIZER_TEXTURE_H
