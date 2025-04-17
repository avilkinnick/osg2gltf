#pragma once

#include "Node.hpp"

#include <memory>

namespace gltf
{

struct Scene
{
    std::shared_ptr<gltf::Node> root_node;
};

}
