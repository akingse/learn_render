//
// Created by LEI XU on 4/27/19.
//
#pragma once

#ifndef RASTERIZER_SHADER_H
#define RASTERIZER_SHADER_H
#include <Eigen/Dense> //$(SolutionDir)..\TPL\eigen-3.4.0
#include "Texture.hpp"


struct fragment_shader_payload
{
    fragment_shader_payload()
    {
        texture = nullptr;
    }

    fragment_shader_payload(const Eigen::Vector3f& col, const Eigen::Vector3f& nor,const Eigen::Vector2f& tc, Texture* tex) :
         color(col), normal(nor), tex_coords(tc), texture(tex) {}

    Eigen::Vector3f view_pos;
    Eigen::Vector3f color;
    Eigen::Vector3f normal;
    Eigen::Vector2f tex_coords;
    Texture* texture;
};

struct vertex_shader_payload
{
    Eigen::Vector3f position;
};

#endif //RASTERIZER_SHADER_H

struct light
{
    Eigen::Vector3f position;
    Eigen::Vector3f intensity;
};

inline Eigen::Vector3f vertex_shader(const vertex_shader_payload& payload)
{
    return payload.position;
}

inline Eigen::Vector3f normal_fragment_shader(const fragment_shader_payload& payload)
{
    Eigen::Vector3f return_color = (payload.normal.head<3>().normalized() + Eigen::Vector3f(1.0f, 1.0f, 1.0f)) / 2.f;
    Eigen::Vector3f result;
    result << return_color.x() * 255, return_color.y() * 255, return_color.z() * 255;
    return result;
}

inline Eigen::Vector3f reflect(const Eigen::Vector3f& vec, const Eigen::Vector3f& axis)
{
    float costheta = vec.dot(axis);
    return (2 * costheta * axis - vec).normalized();
}

Eigen::Vector3f texture_fragment_shader(const fragment_shader_payload& payload);
Eigen::Vector3f phong_fragment_shader(const fragment_shader_payload& payload);
Eigen::Vector3f displacement_fragment_shader(const fragment_shader_payload& payload);
Eigen::Vector3f bump_fragment_shader(const fragment_shader_payload& payload);
