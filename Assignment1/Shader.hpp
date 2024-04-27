//
// Created by LEI XU on 4/27/19.
//
#pragma once

#ifndef RASTERIZER_SHADER_H
#define RASTERIZER_SHADER_H

struct fragment_shader_payload //pixel shader
{
    fragment_shader_payload() = default;
    fragment_shader_payload(const Eigen::Vector3f& vp, const Eigen::Vector3f& col, const Eigen::Vector3f& nor,const Eigen::Vector2f& tc, Texture* tex) :
        shading_point(vp), color(col), normal(nor), tex_coords(tc), texture(tex) {}
    //public:
    Eigen::Vector3f shading_point; //shading point // in loacl flat surface
    //get from interpolation
    Eigen::Vector3f color; 
    Eigen::Vector3f normal;
    Eigen::Vector2f tex_coords; //uv
    Texture* texture = nullptr;
};

struct Light
{
    Eigen::Vector3f position;
    Eigen::Vector3f intensity; //I 
};

class WorldLighting //singleton
{
public:
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);//Ambient coefficient
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);//Specular coefficient
    Eigen::Vector3f amb_light_intensity{ 10, 10, 10 }; //Ambient
    Eigen::Vector3f eye_pos{ 0, 0, 10 };
    int p = 100; //specular exponent
    //Eigen::Vector3f kd; //Diffuse
    std::vector<Light> getLights() const
    {
        Light l1 = Light{ Eigen::Vector3f{20, 20, 20}, Eigen::Vector3f{500, 500, 500} };
        Light l2 = Light{ Eigen::Vector3f{-20, 20, 0}, Eigen::Vector3f{500, 500, 500} };
        std::vector<Light> lights = { l1, l2 };
        return lights;
    }

    static WorldLighting& getInstance()
    {
        static WorldLighting ligting;
        return ligting;
    }
private:
    WorldLighting() = default;
    WorldLighting(const WorldLighting&) = delete;
    WorldLighting operator=(const WorldLighting&) = delete;
};

// shader function
inline Eigen::Vector3f normal_fragment_shader(const fragment_shader_payload& payload)
{
    //Eigen::Vector3f return_color = (payload.normal.head<3>().normalized() + Eigen::Vector3f(1.0f, 1.0f, 1.0f)) / 2.f;
    //Eigen::Vector3f result;
    //result << return_color.x() * 255, return_color.y() * 255, return_color.z() * 255;
    //Eigen::Vector3f color = payload.normal.normalized();
    Eigen::Vector3f color = 0.5 * (payload.normal.normalized() + Eigen::Vector3f(1.0f, 1.0f, 1.0f));
    return 255 * color;
}
Eigen::Vector3f texture_fragment_shader(const fragment_shader_payload& payload);
Eigen::Vector3f phong_fragment_shader(const fragment_shader_payload& payload); //Blinn-Phong
Eigen::Vector3f bump_fragment_shader(const fragment_shader_payload& payload);
Eigen::Vector3f displacement_fragment_shader(const fragment_shader_payload& payload);

#endif //RASTERIZER_SHADER_H
