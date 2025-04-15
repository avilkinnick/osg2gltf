#pragma once

#include "Group.hpp"

namespace osg
{

struct Geode : public Group 
{
    Geode() { struct_name = "osg::Geode"; }
};

}
