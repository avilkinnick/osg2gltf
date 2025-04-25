#pragma once

#include "Mesh.hpp"

#include <map>
#include <memory>
#include <string>
#include <utility>

namespace gltf
{

struct Node
{
    using Unique_osg_id = unsigned int;
    using Id_node_pair = std::pair<unsigned int, std::shared_ptr<gltf::Node>>;

    unsigned int id;
    std::string name;
    std::map<Unique_osg_id, Id_node_pair> children;
    std::pair<unsigned int, std::shared_ptr<Mesh>> mesh;
    bool has_matrix = false;
    float matrix[4][4];
};

}
