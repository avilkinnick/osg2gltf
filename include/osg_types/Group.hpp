#pragma once

#include "osg_types/Data_variance.hpp"
#include "osg_types/Node.hpp"

#include <string>

namespace osg_types
{

struct Group : public Node
{
protected:
    std::string name;
    Data_variance data_variance;
};

}
