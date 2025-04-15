#pragma once

#include "Object.hpp"
#include "State_set.hpp"

#include <memory>

namespace osg
{

struct Geometry : public Object
{
    Geometry() { struct_name = "osg::Geometry"; }

    void print_custom_content(int indentation) const override
    {
        if (state_set)
        {
            state_set->print(indentation);
        }
    }

    std::shared_ptr<State_set> state_set;
};

}
