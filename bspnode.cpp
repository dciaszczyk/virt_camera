#include "bspnode.h"
#include "triangle.h"

static Vec3 lerp(const Vec3& a, const Vec3& b, float t)
{
    return {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        a.z + t * (b.z - a.z)
    };
}

static Vec3 intersect(const Plane& p, const Vec3& a, const Vec3& b)
{
    float da = p.distance(a);
    float db = p.distance(b);
    float t  = da / (da - db);
    return lerp(a, b, t);
}

static void emitTriangles(const std::vector<Vec3>& poly,
                          std::vector<Triangle>& out)
{
    if (poly.size() < 3) return;
    for (size_t i = 1; i + 1 < poly.size(); ++i)
    {
        Triangle tri{poly[0], poly[i], poly[i + 1]};
        tri.computePlane();
        out.push_back(tri);
    }
}

static void splitTriangle(const Triangle& t,
                          const Plane& p,
                          std::vector<Triangle>& front,
                          std::vector<Triangle>& back)
{
    std::vector<Vec3> frontPoly;
    std::vector<Vec3> backPoly;

    Vec3 verts[3] = {t.v0, t.v1, t.v2};

    for (int i = 0; i < 3; ++i)
    {
        Vec3 a = verts[i];
        Vec3 b = verts[(i + 1) % 3];

        float da = p.distance(a);
        float db = p.distance(b);

        bool aFront = da >  EPS;
        bool aBack  = da < -EPS;
        bool bFront = db >  EPS;
        bool bBack  = db < -EPS;

        if (!aBack) frontPoly.push_back(a);
        if (!aFront) backPoly.push_back(a);

        if ((aFront && bBack) || (aBack && bFront))
        {
            Vec3 ip = intersect(p, a, b);
            frontPoly.push_back(ip);
            backPoly.push_back(ip);
        }
    }

    emitTriangles(frontPoly, front);
    emitTriangles(backPoly, back);
}

void BSPNode::build(std::vector<Triangle>& tris)
{
    if (tris.empty()) return;

    plane = tris[0].plane;

    std::vector<Triangle> frontList, backList;

    for (auto& t : tris)
    {
        switch (classifyTriangle(plane, t))
        {
        case Side::Front:
            frontList.push_back(t); break;
        case Side::Back:
            backList.push_back(t); break;
        case Side::Coplanar:
            triangles.push_back(t); break;
        case Side::Spanning:
            splitTriangle(t, plane, frontList, backList); break;
        }
    }

    if (!frontList.empty())
    {
        front = new BSPNode();
        front->build(frontList);
    }
    if (!backList.empty())
    {
        back = new BSPNode();
        back->build(backList);
    }
}

void BSPNode::traverse(const Vec3& cam, std::vector<Triangle>& out) const
{
    float dist = plane.distance(cam);

    const BSPNode* first;
    const BSPNode* second;

    if (dist > 0)
    {
        first  = back;
        second = front;
    }
    else
    {
        first  = front;
        second = back;
    }

    if (first)
        first->traverse(cam, out);

    out.insert(out.end(), triangles.begin(), triangles.end());

    if (second)
        second->traverse(cam, out);
}