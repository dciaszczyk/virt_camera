#pragma once
#include "math.h"

class Camera
{
public:
    Mat4 getViewMatrix() const;
    Mat4 getProjectionMatrix() const;

    // state
    Vec3 position;
    Vec3 rotation;
};
