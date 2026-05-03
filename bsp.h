#pragma once
#include "bspnode.h"

class BSPTree {
public:
    BSPNode* root = nullptr;

    void build(const std::vector<Triangle>& tris);
    std::vector<Triangle> getSorted(const Vec3& cameraPos) const;
};