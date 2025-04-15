#pragma once

#include "Object.hpp"
#include "State_set.hpp"

#include <iostream>
#include <memory>
#include <vector>

namespace osg
{

struct Group : public Object
{
    Group() { struct_name = "osg::Group"; }
    virtual ~Group() = default;

    virtual void print_custom_content(int indentation) const override
    {
        if (state_set)
        {
            state_set->print(indentation);
        }

        indent(indentation);
        std::cout << "Children " << children.size() << " {\n";
        for (const auto& child : children)
        {
            child->print(indentation + 1);
        }
        indent(indentation);
        std::cout << "}\n";
    }

    std::shared_ptr<State_set> state_set;
    std::vector<std::shared_ptr<osg::Object>> children;
};

}
