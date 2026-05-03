#pragma once
#include <vector>
#include "triangle.h"

class BSPNode {
public:
    Plane plane;
    std::vector<Triangle> triangles;

    BSPNode* front = nullptr;
    BSPNode* back = nullptr;

    void build(std::vector<Triangle>& tris);
    void traverse(const Vec3& cameraPos, std::vector<Triangle>& out) const;
};