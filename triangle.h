#pragma once
#include "plane.h"
#include "math3d.h"

static const float EPS = 1e-5f;

struct Triangle {
    Vec3 v0, v1, v2;
    Plane plane;

    void computePlane();
};

Side classifyTriangle(const Plane& p, const Triangle& t);
