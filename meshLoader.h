#pragma once
#include "mesh.h"
#include <fstream>
#include <sstream>
#include <string>

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
        }

        return mesh;
    }
};
