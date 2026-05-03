#pragma once
#include "mesh.h"
#include <fstream>
#include <sstream>
#include <string>
#include "triangle.h"

class MeshLoader {
public:
    static Mesh load(const std::string& path)
    {
        Mesh mesh;
        std::ifstream file(path);

        std::string line;

        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string type;
            ss >> type;

            if (type == "v")
            {
                float x,y,z;
                ss >> x >> y >> z;
                mesh.vertices.push_back({x,y,z});
            }
            else if (type == "e")
            {
                unsigned int a,b;
                ss >> a >> b;
                mesh.indices.push_back(a);
                mesh.indices.push_back(b);
            }
            else if (type == "f")
            {
                unsigned int a, b, c;
                ss >> a >> b >> c;

                mesh.indices.insert(mesh.indices.end(), {a, b, c});
            }
        }

        std::vector<Triangle> triangles;
        for (size_t i = 0; i < mesh.indices.size(); i += 3)
        {
            Triangle t;
            t.v0 = mesh.vertices[mesh.indices[i]];
            t.v1 = mesh.vertices[mesh.indices[i+1]];
            t.v2 = mesh.vertices[mesh.indices[i+2]];
            t.computePlane();

            triangles.push_back(t);
        }

        return mesh;
    }
};
