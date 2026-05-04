#pragma once
#include "plane.h"
#include "math3d.h"

static const float epsilon =1e-5f; //NIEPOPRAWNA WARTOŚĆ: 10.0f - DO POKAZU

struct Triangle {
    Vec3 v0, v1, v2;
    Plane plane;

    void computePlane();
};

Side classifyTriangle(const Plane& p, const Triangle& t);
