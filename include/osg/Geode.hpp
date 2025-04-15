#pragma once

#include "Group.hpp"

namespace osg
{

struct Geode : public Group 
{
    Geode() { struct_name = "osg::Geode"; }

    void print_custom_content(int indentation) const override
    {
        Group::print_custom_content(indentation);
    }
};

}
