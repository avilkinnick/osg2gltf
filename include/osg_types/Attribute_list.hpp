#pragma once

#include "Blend_func.hpp"
#include "osg_types/enums.hpp"

namespace osg_types
{

struct Attribute_list
{
    Blend_func blend_func = {
        .source_rgb = Blend_func_enum::SRC_ALPHA,
        .destination_rgb = Blend_func_enum::ONE_MINUS_SRC_ALPHA,
        .source_alpha = Blend_func_enum::ONE,
        .destination_alpha = Blend_func_enum::ONE
    };
};

};
