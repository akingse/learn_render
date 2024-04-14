#pragma once
#include <Eigen/Dense> 
namespace eigen//eigen
{
    inline Eigen::Vector4f to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
    {
        return Eigen::Vector4f(v3[0], v3[0], v3[0], w);
    }

    inline Eigen::Matrix4f translate(double x, double y, double z = 0.0f)
    {
        Eigen::Matrix4f T;
        T <<
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1;
        return T;
    }

    inline Eigen::Matrix4f translate(const Eigen::Vector3f& vec)
    {
        Eigen::Matrix4f T;
        T <<
            1, 0, 0, vec[0],
            0, 1, 0, vec[1],
            0, 0, 1, vec[2],
            0, 0, 0, 1;
        return T;
    }

    inline Eigen::Matrix4f rotate(const Eigen::Vector3f& axis, float theta = 0.0f)
    {
        Eigen::Quaternionf q = Eigen::Quaternionf(Eigen::AngleAxisf(theta, axis));
        Eigen::Matrix3f R = q.toRotationMatrix();
        Eigen::Matrix4f mat4 = Eigen::Matrix4f::Identity();
        mat4.block<3, 3>(0, 0) = R;
        return mat4;
    }

    inline Eigen::Matrix4f rotate(const Eigen::Vector3f& position, const Eigen::Vector3f& axis, float theta = 0.0f)
    {
        Eigen::Quaternionf q = Eigen::Quaternionf(Eigen::AngleAxisf(theta, axis));
        Eigen::Matrix3f R = q.toRotationMatrix();
        Eigen::Matrix4f mat4 = Eigen::Matrix4f::Identity();
        mat4.block<3, 3>(0, 0) = R;
        return translate(-position) * mat4 * translate(position);
    }


    //get_view_transformation_matrix
    inline Eigen::Matrix4f get_viewing_matrix(const Eigen::Vector3f& eye_pos, const Eigen::Vector3f& gaze, const Eigen::Vector3f& to_up)
    {
    /* https://www.bilibili.com/video/BV1X7411F744?p=4&vd_source=dcb8b113123affd8262c0bfb1bbcbac3
        ^ y (to_up)
        |
        |
        |-----> x
       /
      / z toward front (-gaze)
    */
        //gaze is lookat direciton, to_up is up direciton, get transform of eye relative matrix to world origin
        Eigen::Matrix4f view;// = Eigen::Matrix4f::Identity();
        Eigen::Vector3f eye_x = gaze.cross(to_up);
        view << //been transpose
            eye_x[0], eye_x[1], eye_x[2], 0,
            to_up[0], to_up[1], to_up[2], 0,
            -gaze[0], -gaze[1], -gaze[2], 0,
            0, 0, 0, 1;
        Eigen::Matrix4f translate;
        translate << 
            1, 0, 0, -eye_pos[0],
            0, 1, 0, -eye_pos[1],
            0, 0, 1, -eye_pos[2],
            0, 0, 0, 1;
        return view * translate; //R*T
    }

    inline Eigen::Matrix4f get_model_matrix(const Eigen::Vector3f& position, const Eigen::Vector3f& axis, float theta = 0) //using rad
    {
		return rotate(position, axis, theta);
    }

    //perspective projection
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

    inline Eigen::Matrix4f get_perspective_othographic_matrix(float zNear, float zFar)
    {
        //right-hand coordinate system always negative
        Eigen::Matrix4f perspective;
        perspective <<
            zNear, 0, 0, 0,
            0, zNear, 0, 0,
			0, 0, zNear + zFar, -zNear * zFar,
            0, 0, 0, 1;
        return perspective;
    }

    inline Eigen::Matrix4f get_orthographic_projection_matrix(float l, float r, float b, float t, float n, float f)
    {
        //Scale into ¡°canonical¡± cube, Translate (center to origin) first, then scale (length/width/height to 2)
        Eigen::Matrix4f trans, scale;
        trans <<
			1, 0, 0, -0.5f * (r + l), //left+right
            0, 1, 0, -0.5f * (t + b), //bottom+top
            0, 0, 1, -0.5f * (n + f), //near+far (right-hand coordinate system always negative)
            0, 0, 0, 1;
        scale <<
			2 / (r - l), 0, 0, 0,
            0, 2 / (t - b), 0, 0,
            0, 0, 2 / (n - f), 0,
            0, 0, 0, 1;
        return scale * trans;
    }

    ////orthogonal
    //inline Eigen::Matrix4f get_orthographic_matrix(float l, float b, float n, float r, float t, float f) //left bottom near right top far
    //{
    //    Eigen::Matrix4f Mscale, Mtranslate;
    //    Mscale << 
    //        2.0f / (r - l), 0, 0, 0,
    //        0, 2.0f / (t - b), 0, 0,
    //        0, 0, 2.0f / (n - f), 0,
    //        0, 0, 0, 1;
    //    Mtranslate << 
    //        1, 0, 0, -0.5f * (r - l),
    //        0, 1, 0, -0.5f * (t - b),
    //        0, 0, 1, -0.5f * (n - f),
    //        0, 0, 0, 1;
    //    return Mscale * Mscale;
    //}

    inline std::array<Eigen::Vector3f, 3> operator*(const Eigen::Matrix4f& mat, const std::array<Eigen::Vector3f, 3>& trigon)
    {
        std::array<Eigen::Vector3f, 3> triangle = {
            (mat * trigon[0].homogeneous()).hnormalized(),
            (mat * trigon[1].homogeneous()).hnormalized(),
            (mat * trigon[2].homogeneous()).hnormalized() };
        return triangle;
    }


}