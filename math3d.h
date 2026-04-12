#pragma once
#include <cmath>

struct Vec3
{
    float x,y,z;
    Vec3 operator+(const Vec3& b) const {
        return {x + b.x, y + b.y, z + b.z};
    }

    Vec3 operator-(const Vec3& b) const {
        return {x - b.x, y - b.y, z - b.z};
    }

    Vec3 operator*(float s) const {
        return {x * s, y * s, z * s};
    }

    Vec3 operator/(float s) const {
        return {x / s, y / s, z / s};
    }

    Vec3& operator+=(const Vec3& b) {
        x += b.x; y += b.y; z += b.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& b) {
        x -= b.x; y -= b.y; z -= b.z;
        return *this;
    }
};

struct Vec4{
    float x,y,z,w;
};

inline Vec3 operator*(float s, const Vec3& v)
{
    return {v.x * s, v.y * s, v.z * s};
}

struct Mat4
{
    float m[16]{};

    Mat4 operator*(const Mat4& b) const
    {
        Mat4 r{};

        for (int col = 0; col < 4; ++col)
        {
            for (int row = 0; row < 4; ++row)
            {
                r.m[col * 4 + row] =
                    m[0 * 4 + row] * b.m[col * 4 + 0] +
                    m[1 * 4 + row] * b.m[col * 4 + 1] +
                    m[2 * 4 + row] * b.m[col * 4 + 2] +
                    m[3 * 4 + row] * b.m[col * 4 + 3];
            }
        }

        return r;
    }
};

Vec3 normalize(Vec3 v);
Vec3 cross(Vec3 a, Vec3 b);
float dot(Vec3 a, Vec3 b);

Mat4 lookAt(Vec3 eye, Vec3 target, Vec3 up);
Mat4 perspective(float fov, float aspect, float near, float far);

struct Quat {
    float w, x, y, z;

    static Quat fromAxisAngle(const Vec3& axis, float angleRad) {
        float s = sin(angleRad * 0.5f);
        return {
            static_cast<float>(cos(angleRad * 0.5f)),
            axis.x * s,
            axis.y * s,
            axis.z * s
        };
    }

    Quat operator*(const Quat& b) const
    {
        return {
            w*b.w - x*b.x - y*b.y - z*b.z,
            w*b.x + x*b.w + y*b.z - z*b.y,
            w*b.y - x*b.z + y*b.w + z*b.x,
            w*b.z + x*b.y - y*b.x + z*b.w
        };
    }
};

Quat normalizeQ(const Quat& q);

Vec3 rotateVec(const Quat& q, const Vec3& v);

Mat4 quatToMat4(const Quat& q);

Vec3 transformVec3(const Mat4& m, const Vec3& v);

Mat4 inverseRigid(const Mat4& m);

Mat4 rotationMatrix(const Vec3& axis, float deg);

inline Mat4 identityMat4()
{
    Mat4 m{};

    m.m[0]  = 1.0f;
    m.m[5]  = 1.0f;
    m.m[10] = 1.0f;
    m.m[15] = 1.0f;

    return m;
};

inline Vec4 operator*(const Mat4& m, const Vec4& v)
{
    Vec4 r;

    r.x =
        m.m[0] * v.x +
        m.m[4] * v.y +
        m.m[8] * v.z +
        m.m[12] * v.w;

    r.y =
        m.m[1] * v.x +
        m.m[5] * v.y +
        m.m[9] * v.z +
        m.m[13] * v.w;

    r.z =
        m.m[2] * v.x +
        m.m[6] * v.y +
        m.m[10] * v.z +
        m.m[14] * v.w;

    r.w =
        m.m[3] * v.x +
        m.m[7] * v.y +
        m.m[11] * v.z +
        m.m[15] * v.w;

    return r;
};


