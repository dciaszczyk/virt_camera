#include "triangle.h"

void Triangle::computePlane()
{
    Vec3 u{v1.x - v0.x, v1.y - v0.y, v1.z - v0.z};
    Vec3 v{v2.x - v0.x, v2.y - v0.y, v2.z - v0.z};

    plane.normal = normalize(cross(u, v));
    plane.d = -dot(plane.normal, v0);
}


Side classifyTriangle(const Plane& p, const Triangle& t)
{
    float d0 = p.distance(t.v0);
    float d1 = p.distance(t.v1);
    float d2 = p.distance(t.v2);

    bool anyFront = (d0 > epsilon) || (d1 > epsilon) || (d2 > epsilon);
    bool anyBack  = (d0 < -epsilon) || (d1 < -epsilon) || (d2 < -epsilon);

    if (anyFront && anyBack) return Side::Spanning;
    if (anyFront)            return Side::Front;
    if (anyBack)             return Side::Back;
    return Side::Coplanar;
}