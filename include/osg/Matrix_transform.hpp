#pragma once

#include "Group.hpp"

namespace osg
{

struct Matrix_transform : public Group
{
    float matrix[4][4];
};

}
