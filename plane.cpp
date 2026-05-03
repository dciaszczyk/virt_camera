#include "plane.h"

float Plane::distance(const Vec3& p) const
{
    return dot(normal, p) + d;
}