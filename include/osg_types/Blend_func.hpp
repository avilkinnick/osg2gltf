#pragma once

#include "enums.hpp"

namespace osg_types
{

struct Blend_func
{
    Blend_func_enum source_rgb;
    Blend_func_enum destination_rgb;
    Blend_func_enum source_alpha;
    Blend_func_enum destination_alpha;
};

};
