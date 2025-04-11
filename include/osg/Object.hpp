#pragma once

#include <string>

namespace osg
{

struct Object
{
    virtual ~Object() = default;

    // virtual void print();

    unsigned int unique_id;
    std::string name;
};

}
