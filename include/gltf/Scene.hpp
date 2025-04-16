#pragma once

#include "Node.hpp"

#include <map>
#include <memory>

namespace gltf
{

struct Scene
{
    std::map<unsigned int, std::shared_ptr<Node>> nodes;
};

}
