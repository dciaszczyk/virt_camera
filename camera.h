#pragma once
#include "math3d.h"

class Camera
{
public:
    Mat4 transform;

    Mat4 getViewMatrix() const;

    void rotate(const Vec3& axis, float angleDeg);
    void move(const Vec3& d);

    void snapPosition();
    void snapRotation();

    Vec3 getRight(const Mat4& m);
    Vec3 getUp(const Mat4& m);
    Vec3 getForward(const Mat4& m);

    float fov;
};
