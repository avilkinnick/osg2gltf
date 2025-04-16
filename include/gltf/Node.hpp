#pragma once

#include <map>
#include <memory>
#include <string>

namespace gltf
{

struct Node
{
    unsigned int id;
    std::string name;
    std::map<unsigned int, std::shared_ptr<Node>> children;
    // std::shared_ptr<Mesh> mesh;
    bool has_matrix = false;
    float matrix[4][4];
};

}
