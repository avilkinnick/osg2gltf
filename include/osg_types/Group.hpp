#pragma once

#include "Data_variance.hpp"
#include "Node.hpp"

#include <optional>
#include <string>
#include <vector>

namespace osg_types
{

// TODO: UpdateCallback, EventCallback, CullCallback, Description, UserData, StateSet
struct Group : public Node
{
    std::optional<std::string> name;
    std::optional<Data_variance> data_variance;
    std::vector<Node> children;
};

}
