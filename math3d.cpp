// math3d.cpp
#include "math3d.h"

Mat4 perspective(float fov, float aspect, float n, float f) {
    Mat4 m{};
    float t = 1.0f / tan(fov * M_PI / 360.0f);

    m.m[0] = t / aspect;
    m.m[5] = t;
    m.m[10] = -(f + n) / (f - n);
    m.m[11] = -1.0f;
    m.m[14] = -(2.0f * f * n) / (f - n);

    return m;
}

Quat normalizeQ(const Quat& q) {
    float len = sqrt(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
    return {q.w/len, q.x/len, q.y/len, q.z/len};
}

Vec3 rotateVec(const Quat& q, const Vec3& v) {
    Quat p{0, v.x, v.y, v.z};
    Quat qInv{q.w, -q.x, -q.y, -q.z};
    Quat r = q * p * qInv;
    return {r.x, r.y, r.z};
}

Mat4 quatToMat4(const Quat& q) {
    Mat4 m{};
    float x = q.x, y = q.y, z = q.z, w = q.w;

    m.m[0] = 1 - 2*y*y - 2*z*z;  m.m[4] = 2*x*y - 2*w*z;  m.m[8]  = 2*x*z + 2*w*y;
    m.m[1] = 2*x*y + 2*w*z;      m.m[5] = 1 - 2*x*x - 2*z*z; m.m[9]  = 2*y*z - 2*w*x;
    m.m[2] = 2*x*z - 2*w*y;      m.m[6] = 2*y*z + 2*w*x;    m.m[10] = 1 - 2*x*x - 2*y*y;
    m.m[15] = 1.0f;

    return m;
}

Quat mat4ToQuat(const Mat4& m) {
    float trace = m.m[0] + m.m[5] + m.m[10];
    Quat q;

    if (trace > 0.0f) {
        float s = sqrt(trace + 1.0f) * 2.0f;
        q.w = 0.25f * s;
        q.x = (m.m[6] - m.m[9]) / s;
        q.y = (m.m[8] - m.m[2]) / s;
        q.z = (m.m[1] - m.m[4]) / s;
    } else if (m.m[0] > m.m[5] && m.m[0] > m.m[10]) {
        float s = sqrt(1.0f + m.m[0] - m.m[5] - m.m[10]) * 2.0f;
        q.w = (m.m[6] - m.m[9]) / s;
        q.x = 0.25f * s;
        q.y = (m.m[4] + m.m[1]) / s;
        q.z = (m.m[8] + m.m[2]) / s;
    } else if (m.m[5] > m.m[10]) {
        float s = sqrt(1.0f + m.m[5] - m.m[0] - m.m[10]) * 2.0f;
        q.w = (m.m[8] - m.m[2]) / s;
        q.x = (m.m[4] + m.m[1]) / s;
        q.y = 0.25f * s;
        q.z = (m.m[9] + m.m[6]) / s;
    } else {
        float s = sqrt(1.0f + m.m[10] - m.m[0] - m.m[5]) * 2.0f;
        q.w = (m.m[1] - m.m[4]) / s;
        q.x = (m.m[8] + m.m[2]) / s;
        q.y = (m.m[9] + m.m[6]) / s;
        q.z = 0.25f * s;
    }

    return normalizeQ(q);
}

Quat quatFromBasis(const Vec3& right, const Vec3& up, const Vec3& forward)
{
    Mat4 m{};

    m.m[0]=right.x;   m.m[4]=up.x;   m.m[8]=forward.x;
    m.m[1]=right.y;   m.m[5]=up.y;   m.m[9]=forward.y;
    m.m[2]=right.z;   m.m[6]=up.z;   m.m[10]=forward.z;
    m.m[15]=1.0f;

    return normalizeQ(mat4ToQuat(m));
}

Mat4 inverseRigid(const Mat4& m)
{
    Mat4 r{};

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

    r.m[12] = -(r.m[0]*t.x + r.m[4]*t.y + r.m[8]*t.z);
    r.m[13] = -(r.m[1]*t.x + r.m[5]*t.y + r.m[9]*t.z);
    r.m[14] = -(r.m[2]*t.x + r.m[6]*t.y + r.m[10]*t.z);

    return r;
}
