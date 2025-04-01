#pragma once

#include "enums.hpp"
#include "Node.hpp"

#include <string>

namespace osg_types
{

struct State_set : public Node
{
    Data_variance data_variance = Data_variance::STATIC;
    Rendering_hint rendering_hint = Rendering_hint::DEFAULT_BIN;
    Render_bin_mode render_bin_mode = Render_bin_mode::INHERIT;
    int bin_number = 0;
    std::string bin_name = "";
};

}
