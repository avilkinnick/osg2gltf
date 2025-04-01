#pragma once

#include "Node.hpp"

#include <string>

namespace osg_types
{

struct Geometry : public Node
{
protected:
    std::string name;
};

}
