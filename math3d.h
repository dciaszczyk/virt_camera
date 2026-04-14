#pragma once
#include <cmath>

struct Vec3 {
    float x, y, z;

    Vec3 operator+(const Vec3& b) const { return {x + b.x, y + b.y, z + b.z}; }
    Vec3 operator-(const Vec3& b) const { return {x - b.x, y - b.y, z - b.z}; }
    Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }

    Vec3& operator+=(const Vec3& b) { x += b.x; y += b.y; z += b.z; return *this; }
    Vec3& operator-=(const Vec3& b) { x -= b.x; y -= b.y; z -= b.z; return *this; }
};

inline Vec3 operator*(float s, const Vec3& v) { return {v.x * s, v.y * s, v.z * s}; }

struct Mat4 {
    float m[16]{};

    Mat4 operator*(const Mat4& b) const {
        Mat4 r;
        for (int col = 0; col < 4; ++col) {
            for (int row = 0; row < 4; ++row) {
                r.m[col * 4 + row] =
                    m[row] * b.m[col * 4] +
                    m[4 + row] * b.m[col * 4 + 1] +
                    m[8 + row] * b.m[col * 4 + 2] +
                    m[12 + row] * b.m[col * 4 + 3];
            }
        }
        return r;
    }
};

struct Quat {
    float w, x, y, z;

    static Quat fromAxisAngle(const Vec3& axis, float angleRad) {
        float s = sin(angleRad * 0.5f);
        return {static_cast<float>(cos(angleRad * 0.5f)), axis.x * s, axis.y * s, axis.z * s};
    }

    Quat operator*(const Quat& b) const {
        return {
            w*b.w - x*b.x - y*b.y - z*b.z,
            w*b.x + x*b.w + y*b.z - z*b.y,
            w*b.y - x*b.z + y*b.w + z*b.x,
            w*b.z + x*b.y - y*b.x + z*b.w
        };
    }
};

inline float dot(const Vec3& a, const Vec3& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}
inline Vec3 normalize(const Vec3& v) {
    float l = sqrt(dot(v, v));
    return l < 1e-6f ? Vec3{0,0,0} : Vec3{v.x/l, v.y/l, v.z/l};
}

inline Mat4 identityMat4() {
    Mat4 m;
    m.m[0] = m.m[5] = m.m[10] = m.m[15] = 1.0f;
    return m;
}

Mat4 perspective(float fov, float aspect, float near, float far);
Quat normalizeQ(const Quat& q);
Vec3 rotateVec(const Quat& q, const Vec3& v);
Mat4 quatToMat4(const Quat& q);
Quat mat4ToQuat(const Mat4& m);

Quat quatFromBasis(const Vec3& right, const Vec3& up, const Vec3& forward);

Mat4 inverseRigid(const Mat4& m);

