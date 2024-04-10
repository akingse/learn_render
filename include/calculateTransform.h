#pragma once
#include <Eigen/Dense> 
namespace eigen//eigen
{
    inline Eigen::Matrix4f get_view_matrix(const Eigen::Vector3f& eye_pos)
    {
        Eigen::Matrix4f view = Eigen::Matrix4f::Identity();
        Eigen::Matrix4f translate;
        translate << 
            1, 0, 0, -eye_pos[0],
            0, 1, 0, -eye_pos[1],
            0, 0, 1, -eye_pos[2],
            0, 0, 0, 1;
        view = translate * view;
        return view;
    }

    inline Eigen::Matrix4f get_model_matrix(const Eigen::Vector3f& axis, float theta = 0) //using rad
    {
        Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
        // Create the model matrix for rotating the triangle around the Z axis.
        model << 
            cos(theta), -sin(theta), 0, 0,
            sin(theta), cos(theta), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;
        // any axis rot
        Eigen::Quaternionf q = Eigen::Quaternionf(Eigen::AngleAxisf(theta, axis.normalized()));
        Eigen::Matrix3f R = q.toRotationMatrix();
        model.block<3, 3>(0, 0) = R;
        return model;
    }

    inline Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
    {
        Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
        // Create the projection matrix for the given parameters.
        float ty = -1.0f / tan(eye_fov / 2.0f); //field of view
        projection << 
            (ty / aspect_ratio), 0, 0, 0,
            0, ty, 0, 0,
            0, 0, (zNear + zFar) / (zNear - zFar), (-2 * zNear * zFar) / (zNear - zFar),
            0, 0, 1, 0;
        return projection;
    }

    //orthogonal
    inline Eigen::Matrix4f get_orthographic_matrix(float l, float b, float n, float r, float t, float f) //left bottom near right top far
    {
        Eigen::Matrix4f Mscale, Mtranslate;
        Mscale << 
            2.0f / (r - l), 0, 0, 0,
            0, 2.0f / (t - b), 0, 0,
            0, 0, 2.0f / (n - f), 0,
            0, 0, 0, 1;
        Mtranslate << 
            1, 0, 0, -0.5f * (r - l),
            0, 1, 0, -0.5f * (t - b),
            0, 0, 1, -0.5f * (n - f),
            0, 0, 0, 1;
        return Mscale * Mscale;
    }
}