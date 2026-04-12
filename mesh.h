#pragma once
#include <vector>
#include "math3d.h"

struct Mesh {
    std::vector<Vec3> vertices;
    std::vector<unsigned int> indices;
};
