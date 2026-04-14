#pragma once
#include "math3d.h"// camera.h
#pragma once
#include "math3d.h"

class Camera {
public:
    //Mat4 transform;

    Vec3 position{0, 0, 10};  // Vec3 is sufficient
    Quat orientation{1, 0, 0, 0};
    float fov = 60.0f;

    Mat4 getViewMatrix() const;
    Mat4 getViewProjectionMatrix(int width, int height) const;

    void rotate(const Vec3& worldAxis, float angleDeg);
    void move(const Vec3& localDelta);

    void snapPosition();
    void snapRotation();

    Vec3 getRight() const   { return rotateVec(orientation, {1,0,0}); }
    Vec3 getUp() const      { return rotateVec(orientation, {0,1,0}); }
    Vec3 getForward() const { return rotateVec(orientation, {0,0,1}); }
};
