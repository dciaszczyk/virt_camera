#pragma once
#include "math3d.h"

enum class Side {
    Front,
    Back,
    Coplanar,
    Spanning
};

struct Plane {
    Vec3 normal;
    float d;

    float distance(const Vec3& p) const;
};