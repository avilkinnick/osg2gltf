#pragma once

#include "Group.hpp"

namespace osg
{

struct Matrix_transform final : public Group
{
    float matrix[4][4];
};

}
