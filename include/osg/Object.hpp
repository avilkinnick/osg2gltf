#pragma once

#include <string>

namespace osg
{

struct Object
{
    virtual ~Object() = default;

    // virtual void print(int indentation = 0) const;

    unsigned int unique_id;
    std::string name;
};

}
