#pragma once

#include "Image.hpp"
#include "Object.hpp"

#include <memory>

namespace osg
{

struct Texture2d : public Object
{
    Texture2d() { struct_name = "osg::Texture2D"; }

    void print_custom_content(int indentation) const override
    {
        if (image)
        {
            image->print(indentation);
        }
    }

    std::shared_ptr<osg::Image> image;
};

}
