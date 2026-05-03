#include "bsp.h"

void BSPTree::build(const std::vector<Triangle>& tris)
{
    if (root) delete root;

    root = new BSPNode();

    std::vector<Triangle> copy = tris;
    root->build(copy);
}

std::vector<Triangle> BSPTree::getSorted(const Vec3& cameraPos) const
{
    std::vector<Triangle> result;

    if (root)
        root->traverse(cameraPos, result);

    return result;
}