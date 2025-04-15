#pragma once

#include "Object.hpp"
#include "Texture2d.hpp"

#include <iostream>
#include <memory>

namespace osg
{

struct State_set : public Object
{
    State_set() { struct_name = "osg::StateSet"; }

    void print_custom_content(int indentation) const override
    {
        indent(indentation);
        std::cout << "GL_BLEND " << (blend ? "ON" : "OFF") << '\n';

        if (texture)
        {
            texture->print(indentation);
        }
    }

    bool blend = false;
    std::shared_ptr<osg::Texture2d> texture;
};

}
