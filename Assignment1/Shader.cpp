#include "pch.h"
using namespace std;
using namespace rst;
using namespace Eigen;
using namespace eigen;

Eigen::Vector3f texture_fragment_shader(const fragment_shader_payload& payload)
{
    Eigen::Vector3f return_color = { 0, 0, 0 };
    if (payload.texture)
    {
        return_color = payload.texture->getColor(payload.tex_coords);
    }
    Eigen::Vector3f texture_color = return_color;
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = texture_color / 255.f;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    light l1 = light{ {20, 20, 20}, {500, 500, 500} };
    light l2 = light{ {-20, 20, 0}, {500, 500, 500} };

    std::vector<light> lights = { l1, l2 };
    Eigen::Vector3f amb_light_intensity{ 10, 10, 10 };
    Eigen::Vector3f eye_pos{ 0, 0, 10 };
    float p = 150;
    Eigen::Vector3f color = texture_color;
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;
    Eigen::Vector3f viewDir = -point.normalized();
    Eigen::Vector3f result_color = { 0, 0, 0 };
    for (auto& light : lights)
    {
        Eigen::Vector3f lightDir = (light.position - point).normalized();
        Eigen::Vector3f halfVector = ((lightDir + viewDir) / 2.0f).normalized();
        float r2 = (light.position - point).squaredNorm();
        Vector3f ambient = ka.cwiseProduct(amb_light_intensity); //间接光也是光源发出的，所以也要累加
        Vector3f diffuse = kd.cwiseProduct(light.intensity / r2 * std::max(0.0f, normal.dot(lightDir)));
        Vector3f specular = ks.cwiseProduct(light.intensity / r2 * std::pow(std::max(0.0f, normal.dot(halfVector)), p));
        result_color += ambient + diffuse + specular;
    }

    return result_color * 255.f;
}

//linght = 环境光(Ambient)+漫反射(Diffuse)+高光反射(Specular)
Eigen::Vector3f phong_fragment_shader(const fragment_shader_payload& payload)
{
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = payload.color;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);
    light l1 = light{ {20, 20, 20}, 2*Vector3f{500, 500, 500} };
    light l2 = light{ {-20, 20, 0}, 2*Vector3f{500, 500, 500} };
    std::vector<light> lights = { l1, l2 };
    Eigen::Vector3f amb_light_intensity{ 10, 10, 10 }; //Ambient
    //Eigen::Vector3f eye_pos{ 0, 2, 5 };
    float p = 150;
    Eigen::Vector3f color = payload.color;
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;
    Eigen::Vector3f viewDir = -point.normalized();
    Eigen::Vector3f result_color = { 0, 0, 0 };
    for (const auto& light : lights)
    {
        // For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
        // components are. Then, accumulate that result on the *result_color* object.
        Eigen::Vector3f lightDir = (light.position - point).normalized();
        Eigen::Vector3f halfVector = ((lightDir + viewDir) / 2.0f).normalized();
        float r2 = (light.position - point).squaredNorm();
        Vector3f ambient = ka.cwiseProduct(amb_light_intensity); //间接光也是光源发出的，所以也要累加
        Vector3f diffuse = kd.cwiseProduct(light.intensity / r2 * std::max(0.0f, normal.dot(lightDir)));
        Vector3f specular = ks.cwiseProduct(light.intensity / r2 * std::pow(std::max(0.0f, normal.dot(halfVector)), p));
        result_color += ambient + diffuse + specular;
    }
    return result_color * 255.f;
}

Eigen::Vector3f displacement_fragment_shader(const fragment_shader_payload& payload)
{
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = payload.color;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);
    light l1 = light{ {20, 20, 20}, {500, 500, 500} };
    light l2 = light{ {-20, 20, 0}, {500, 500, 500} };
    std::vector<light> lights = { l1, l2 };
    Eigen::Vector3f amb_light_intensity{ 10, 10, 10 };
    Eigen::Vector3f eye_pos{ 0, 0, 10 };
    float p = 150;
    Eigen::Vector3f color = payload.color;
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;
    float kh = 0.2, kn = 0.1;

    // TODO: Implement displacement mapping here
    // Let n = normal = (x, y, z)
    // Vector t = (x*y/sqrt(x*x+z*z),sqrt(x*x+z*z),z*y/sqrt(x*x+z*z))
    // Vector b = n cross product t
    // Matrix TBN = [t b n]
    // dU = kh * kn * (h(u+1/w,v)-h(u,v))
    // dV = kh * kn * (h(u,v+1/h)-h(u,v))
    // Vector ln = (-dU, -dV, 1)
    // Position p = p + kn * n * h(u,v)
    // Normal n = normalize(TBN * ln)
    float x = normal.x();
    float y = normal.y();
    float z = normal.z();
    Eigen::Vector3f t = Eigen::Vector3f(x * y / std::sqrt(x * x + z * z), std::sqrt(x * x + z * z), z * y / std::sqrt(x * x + z * z));
    Eigen::Vector3f b = normal.cross(t);
    Eigen::Matrix3f TBN;
    TBN <<
        t.x(), b.x(), normal.x(),
        t.y(), b.y(), normal.y(),
        t.z(), b.z(), normal.z();
    float u = payload.tex_coords.x();
    float v = payload.tex_coords.y();
    float w = payload.texture->width;
    float h = payload.texture->height;
    float dU = kh * kn * (payload.texture->getColor({ u + 1.0f / w, v }).norm() - payload.texture->getColor({ u, v }).norm());
    float dV = kh * kn * (payload.texture->getColor({ u, v + 1.0f / h }).norm() - payload.texture->getColor({ u, v }).norm());
    Eigen::Vector3f ln = Eigen::Vector3f(-dU, -dV, 1.0f);
    point += (kn * normal * payload.texture->getColor({ u, v }).norm());
    normal = (TBN * ln).normalized();
    Eigen::Vector3f viewDir = -point.normalized();
    Eigen::Vector3f result_color = { 0, 0, 0 };
    for (auto& light : lights)
    {
        Eigen::Vector3f lightDir = (light.position - point).normalized();
        Eigen::Vector3f halfVector = ((lightDir + viewDir) / 2.0f).normalized();
        float r2 = (light.position - point).squaredNorm();
        Vector3f ambient = ka.cwiseProduct(amb_light_intensity); //间接光也是光源发出的，所以也要累加
        Vector3f diffuse = kd.cwiseProduct(light.intensity / r2 * std::max(0.0f, normal.dot(lightDir)));
        Vector3f specular = ks.cwiseProduct(light.intensity / r2 * std::pow(std::max(0.0f, normal.dot(halfVector)), p));
        result_color += ambient + diffuse + specular;
    }
    return result_color * 255.f;
}

Eigen::Vector3f bump_fragment_shader(const fragment_shader_payload& payload)
{
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = payload.color;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);
    light l1 = light{ {20, 20, 20}, {500, 500, 500} };
    light l2 = light{ {-20, 20, 0}, {500, 500, 500} };
    std::vector<light> lights = { l1, l2 };
    Eigen::Vector3f amb_light_intensity{ 10, 10, 10 };
    Eigen::Vector3f eye_pos{ 0, 0, 10 };
    float p = 150;
    Eigen::Vector3f color = payload.color;
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;
    float kh = 0.2, kn = 0.1;

    // TODO: Implement bump mapping here
    // Let n = normal = (x, y, z)
    // Vector t = (x*y/sqrt(x*x+z*z),sqrt(x*x+z*z),z*y/sqrt(x*x+z*z))
    // Vector b = n cross product t
    // Matrix TBN = [t b n]
    // dU = kh * kn * (h(u+1/w,v)-h(u,v))
    // dV = kh * kn * (h(u,v+1/h)-h(u,v))
    // Vector ln = (-dU, -dV, 1)
    // Normal n = normalize(TBN * ln)
    float x = normal.x();
    float y = normal.y();
    float z = normal.z();
    Eigen::Vector3f t = Eigen::Vector3f(x * y / std::sqrt(x * x + z * z), std::sqrt(x * x + z * z), z * y / std::sqrt(x * x + z * z));
    Eigen::Vector3f b = normal.cross(t);
    Eigen::Matrix3f TBN;
    TBN <<
        t.x(), b.x(), normal.x(),
        t.y(), b.y(), normal.y(),
        t.z(), b.z(), normal.z();
    float u = payload.tex_coords.x();
    float v = payload.tex_coords.y();
    float w = payload.texture->width;
    float h = payload.texture->height;
	float dU = kh * kn * (payload.texture->getColor({ u + 1.0f / w, v }).norm() - payload.texture->getColor({u, v}).norm());
    float dV = kh * kn * (payload.texture->getColor({ u, v + 1.0f / h }).norm() - payload.texture->getColor({ u, v }).norm());
    Eigen::Vector3f ln = Eigen::Vector3f(-dU, -dV, 1.0f);
    normal = TBN * ln;
    Eigen::Vector3f result_color = { 0, 0, 0 };
    result_color = normal;
    return result_color * 255.f;
    return result_color * 255.f;
}
