#pragma once
namespace eigen//eigen
{
    typedef std::array<Eigen::Vector2f, 3> Trangle2f;
    typedef std::array<Eigen::Vector3f, 3> Trangle3f;
    typedef std::array<Eigen::Vector2f, 2> Segment2f;
    typedef std::array<Eigen::Vector3f, 2> Segment3f;

    inline Eigen::Vector4f to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
    {
        return Eigen::Vector4f(v3[0], v3[1], v3[2], w);
    }

    inline std::array<Eigen::Vector4f,3> to_vec4(const std::array<Eigen::Vector3f, 3>& v3, float w = 1.0f)
    {
        return { to_vec4(v3[0],w), to_vec4(v3[1],w), to_vec4(v3[2],w) };
    }

    inline std::array<Eigen::Vector2f, 3> to_vec2(const std::array<Eigen::Vector3f, 3>& v3) //input pointer array
    {
        return { 
            Eigen::Vector2f(v3[0][0], v3[0][1]),
            Eigen::Vector2f(v3[1][0], v3[1][1]),
            Eigen::Vector2f(v3[2][0], v3[2][1]) };
    }

    inline double cross2f(const Eigen::Vector2f& v0, const Eigen::Vector2f& v1)
    {
        return v0[0] * v1[1] - v0[1] * v1[0];
    }

    inline Eigen::Vector3f reflect(const Eigen::Vector3f& vec, const Eigen::Vector3f& axis) // get input vec's mirror vector
    {
        float costheta = vec.dot(axis);
        return (2 * costheta * axis - vec).normalized();
    }


    inline std::array<Eigen::Vector3f, 3> _toArray(const Eigen::Vector3f* v)
    {
        return std::array<Eigen::Vector3f, 3>{v[0], v[1], v[2]};
    };

    inline std::array<Eigen::Vector2f, 3> _toArray(const Eigen::Vector2f* v)
    {
        return std::array<Eigen::Vector2f, 3>{v[0], v[1], v[2]};
    };

    inline std::array<Eigen::Vector3f, 3> operator*(const Eigen::Matrix4f& mat, const std::array<Eigen::Vector3f, 3>& trigon)
    {
        std::array<Eigen::Vector3f, 3> triangle;
        for (int i = 0; i < 3; ++i)
            triangle[i] = (mat * trigon[i].homogeneous()).hnormalized();
        return triangle;
    }

	inline Eigen::Matrix4f scale(float kx, float ky, float kz = 1.0f)
    {
        Eigen::Matrix4f T;
        T <<
            kx, 0, 0, 0,
            0, ky, 0, 0,
            0, 0, kz, 0,
            0, 0, 0, 1;
        return T;
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

    //----------------------------------------------------------------------------------
    //                   MVP
    //----------------------------------------------------------------------------------

    inline Eigen::Matrix4f get_model_matrix(const Eigen::Vector3f& position, const Eigen::Vector3f& axis, float theta = 0) //using rad
    {
        return rotate(position, axis, theta);
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
        Eigen::Vector3f eye_x = gaze.cross(to_up);
        Eigen::Matrix4f view;// = Eigen::Matrix4f::Identity();
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

    //perspective projection
    inline Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
    {
        Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
        // Create the projection matrix for the given parameters.
		float ty = -1.0f / tan(eye_fov * 0.5f); //field/frustum of view
        projection << 
            (ty / aspect_ratio), 0, 0, 0,
            0, ty, 0, 0,
            0, 0, (zNear + zFar) / (zNear - zFar), (-2.f * zNear * zFar) / (zNear - zFar),
            0, 0, 1, 0;
        return projection;
    }

    inline Eigen::Matrix4f get_projection_matrix(float l, float r, float b, float t, float n, float f)
    {
        //Scale into ¡°canonical¡± cube, Translate (center to origin) first, then scale (length/width/height to 2)
        Eigen::Matrix4f trans, scale, perspective;
        trans <<
            1, 0, 0, -0.5f * (r + l), //left+right
            0, 1, 0, -0.5f * (t + b), //bottom+top
            0, 0, 1, -0.5f * (n + f), //near+far (right-hand coordinate system always negative)
            0, 0, 0, 1;
        scale <<
            2.f / (r - l), 0, 0, 0,
            0, 2.f / (t - b), 0, 0,
            0, 0, 2.f / (n - f), 0,
            0, 0, 0, 1;
        //right-hand coordinate system always negative
        perspective <<
            n, 0, 0, 0,
            0, n, 0, 0,
            0, 0, n + f, -n * f,
            0, 0, 1, 0;
		return scale * trans * perspective;
    }

    //--------------------------------------------------------------------------------------------------------
    // Computational Geometry
    //--------------------------------------------------------------------------------------------------------

    inline bool insideTriangle(float x, float y, const std::array<Eigen::Vector3f, 3>& _v)
    {
        // check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]
        Eigen::Vector3f p0p1(_v[0].x() - _v[1].x(), _v[0].y() - _v[1].y(), 1.0f);
        Eigen::Vector3f p1p2(_v[1].x() - _v[2].x(), _v[1].y() - _v[2].y(), 1.0f);
        Eigen::Vector3f p2p0(_v[2].x() - _v[0].x(), _v[2].y() - _v[0].y(), 1.0f);
        Eigen::Vector3f p0p(_v[0].x() - x, _v[0].y() - y, 1.0f);
        Eigen::Vector3f p1p(_v[1].x() - x, _v[1].y() - y, 1.0f);
        Eigen::Vector3f p2p(_v[2].x() - x, _v[2].y() - y, 1.0f);
        if (p0p1.cross(p0p).z() > 0.f) 
            return p1p2.cross(p1p).z() > 0.f && p2p0.cross(p2p).z() > 0.f;
        else 
            return p1p2.cross(p1p).z() < 0.f && p2p0.cross(p2p).z() < 0.f;
    }

    // std::tuple<float, float, float>
    inline Eigen::Vector3f computeBarycentric2D(float x, float y, const std::array<Eigen::Vector3f, 3>& v)
    {
        float c1 = (x * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * y + v[1].x() * v[2].y() - v[2].x() * v[1].y()) 
            / (v[0].x() * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * v[0].y() + v[1].x() * v[2].y() - v[2].x() * v[1].y());
        float c2 = (x * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * y + v[2].x() * v[0].y() - v[0].x() * v[2].y()) 
            / (v[1].x() * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * v[1].y() + v[2].x() * v[0].y() - v[0].x() * v[2].y());
        float c3 = (x * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * y + v[0].x() * v[1].y() - v[1].x() * v[0].y()) 
            / (v[2].x() * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * v[2].y() + v[0].x() * v[1].y() - v[1].x() * v[0].y());
        if (std::isnan(c1))
            c1 = v[0].x();
        if (std::isnan(c2))
            c2 = v[0].y();
        if (std::isnan(c3))
            c3 = v[0].z();
        return Eigen::Vector3f(c1, c2, c3);
    }

    inline Eigen::Vector3f computeBarycentric(const Eigen::Vector3f& abc, const std::array<Eigen::Vector3f, 3>& trigon)
    {
        return abc[0] * trigon[0] + abc[1] * trigon[1] + abc[2] * trigon[2];
    }
    inline Eigen::Vector2f computeBarycentric(const Eigen::Vector3f& abc, const std::array<Eigen::Vector2f, 3>& trigon)
    {
        return abc[0] * trigon[0] + abc[1] * trigon[1] + abc[2] * trigon[2];
    }

    inline Eigen::Vector3f getBarycentricInterpolate(const std::array<Eigen::Vector3f, 3>& trigon, Eigen::Vector2f bc)//->Eigen::Vector3f
    {
        Eigen::Vector3f AB = trigon[1] - trigon[0];
        Eigen::Vector3f AC = trigon[2] - trigon[0];
        return trigon[0] + bc[0] * AB + bc[1] * AC;
    }

    inline Eigen::Vector2f getBarycentricCoordinate(const std::array<Eigen::Vector2f, 3>& trigon, const Eigen::Vector2f& P)
    {
        Eigen::Vector2f AB = trigon[1] - trigon[0];
        Eigen::Vector2f AC = trigon[2] - trigon[0];
        Eigen::Vector2f AP = P - trigon[0];
        float areaABC = cross2f(AB, AC);
        float a = cross2f(AB, AP) / areaABC;
        float b = cross2f(AP, AC) / areaABC;
        //float c = 1.0f - a - b;
        return Eigen::Vector2f(b, a); //its inverse
    }

    inline Eigen::Vector3f getBarycentricCoordinate(const std::array<Eigen::Vector3f, 3>& trigon, const Eigen::Vector2f& P)
    {
        Eigen::Vector2f bc = getBarycentricCoordinate(to_vec2(trigon), P);
        return getBarycentricInterpolate(trigon, bc);
    }

    inline Eigen::Vector3f interpolate(float alpha, float beta, float gamma, const Eigen::Vector3f& vert1, const Eigen::Vector3f& vert2, const Eigen::Vector3f& vert3, float weight)
    {
        return (alpha * vert1 + beta * vert2 + gamma * vert3) / weight;
    }

    inline Eigen::Vector2f interpolate(float alpha, float beta, float gamma, const Eigen::Vector2f& vert1, const Eigen::Vector2f& vert2, const Eigen::Vector2f& vert3, float weight)
    {
        float u = (alpha * vert1[0] + beta * vert2[0] + gamma * vert3[0]);
        float v = (alpha * vert1[1] + beta * vert2[1] + gamma * vert3[1]);
        u /= weight;
        v /= weight;
        return Eigen::Vector2f(u, v);
    }

}