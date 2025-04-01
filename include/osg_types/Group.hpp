#pragma once

#include "enums.hpp"
#include "Node.hpp"
#include "State_set.hpp"

#include <memory>
#include <string>
#include <vector>

namespace osg_types
{

// TODO: UpdateCallback, EventCallback, CullCallback, Description, UserData, StateSet
struct Group : public Node
{
    std::string name = "";
    Data_variance data_variance = Data_variance::UNSPECIFIED;
    std::shared_ptr<State_set> state_set = nullptr;
    std::string description = "";
    std::vector<Node> children = {};
};

}
