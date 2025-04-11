#pragma once

#include <string>

namespace osg
{

struct Object
{
    virtual ~Object() = default;

    unsigned int unique_id;
    std::string name;
};

}
