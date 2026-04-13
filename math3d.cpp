#include "math3d.h"

Vec3 normalize(Vec3 v)
{
    float l = std::sqrt(dot(v,v));
    return {v.x/l, v.y/l, v.z/l};
}

Vec3 cross(Vec3 a, Vec3 b)
{
    return {
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    };
}

float dot(Vec3 a, Vec3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}



Mat4 perspective(float fov, float aspect, float n, float f)
{
    Mat4 m{};

    float t = 1.0f / tan(fov * 0.5f);

    m.m[0] = t / aspect;
    m.m[5] = t;
    m.m[10] = -(f+n)/(f-n);
    m.m[11] = -1.0f;
    m.m[14] = -(2*f*n)/(f-n);

    return m;
}

Quat normalizeQ(const Quat& q)
{
    float len = sqrt(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
    return {q.w/len, q.x/len, q.y/len, q.z/len};
}

Vec3 rotateVec(const Quat& q, const Vec3& v)
{
    Quat p{0, v.x, v.y, v.z};
    Quat qInv{q.w, -q.x, -q.y, -q.z};

    Quat r = q * p * qInv;

    return {r.x, r.y, r.z};
}

Mat4 quatToMat4(const Quat& q)
{
    Mat4 m{};

    float x = q.x, y = q.y, z = q.z, w = q.w;

    m.m[0]  = 1 - 2*y*y - 2*z*z;
    m.m[1]  = 2*x*y + 2*w*z;
    m.m[2]  = 2*x*z - 2*w*y;
    m.m[3]  = 0;

    m.m[4]  = 2*x*y - 2*w*z;
    m.m[5]  = 1 - 2*x*x - 2*z*z;
    m.m[6]  = 2*y*z + 2*w*x;
    m.m[7]  = 0;

    m.m[8]  = 2*x*z + 2*w*y;
    m.m[9]  = 2*y*z - 2*w*x;
    m.m[10] = 1 - 2*x*x - 2*y*y;
    m.m[11] = 0;

    m.m[12] = 0;
    m.m[13] = 0;
    m.m[14] = 0;
    m.m[15] = 1;

    return m;
}

Vec3 transformVec3(const Mat4& m, const Vec3& v)
{
    return {
        m.m[0]*v.x + m.m[4]*v.y + m.m[8]*v.z,
        m.m[1]*v.x + m.m[5]*v.y + m.m[9]*v.z,
        m.m[2]*v.x + m.m[6]*v.y + m.m[10]*v.z
    };
}

Mat4 inverseRigid(const Mat4& m)
{
    Mat4 r{};

    // transpose rotation (upper 3x3)
    r.m[0] = m.m[0];
    r.m[1] = m.m[4];
    r.m[2] = m.m[8];
    r.m[3] = 0;

    r.m[4] = m.m[1];
    r.m[5] = m.m[5];
    r.m[6] = m.m[9];
    r.m[7] = 0;

    r.m[8]  = m.m[2];
    r.m[9]  = m.m[6];
    r.m[10] = m.m[10];
    r.m[11] = 0;

    r.m[15] = 1;

    Vec3 t{m.m[12], m.m[13], m.m[14]};

    // corrected translation
    r.m[12] = -(r.m[0]*t.x + r.m[4]*t.y + r.m[8]*t.z);
    r.m[13] = -(r.m[1]*t.x + r.m[5]*t.y + r.m[9]*t.z);
    r.m[14] = -(r.m[2]*t.x + r.m[6]*t.y + r.m[10]*t.z);

    return r;
}

Mat4 rotationMatrix(const Vec3& axis, float deg)
{
    float a = deg * M_PI / 180.0f;
    float c = cos(a);
    float s = sin(a);
    Vec3 u = normalize(axis);

    Mat4 m{};

    m.m[0] = c + u.x*u.x*(1-c);
    m.m[1] = u.x*u.y*(1-c) + u.z*s;
    m.m[2] = u.x*u.z*(1-c) - u.y*s;
    m.m[3] = 0;

    m.m[4] = u.y*u.x*(1-c) - u.z*s;
    m.m[5] = c + u.y*u.y*(1-c);
    m.m[6] = u.y*u.z*(1-c) + u.x*s;
    m.m[7] = 0;

    m.m[8]  = u.z*u.x*(1-c) + u.y*s;
    m.m[9]  = u.z*u.y*(1-c) - u.x*s;
    m.m[10] = c + u.z*u.z*(1-c);
    m.m[11] = 0;

    m.m[12] = 0;
    m.m[13] = 0;
    m.m[14] = 0;
    m.m[15] = 1;

    return m;
}
