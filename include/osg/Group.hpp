#pragma once

#include "Object.hpp"
#include "State_set.hpp"

#include <memory>
#include <vector>

namespace osg
{

struct Group : public Object
{
    virtual ~Group() = default;

    std::shared_ptr<State_set> state_set;
    std::vector<std::shared_ptr<osg::Object>> children;
};

}
