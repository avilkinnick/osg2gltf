#pragma once

#include "Object.hpp"

#include <iostream>

namespace osg
{

struct Image : public Object
{
    Image() { struct_name = "Image"; }

    void print_custom_content(int indentation) const override
    {
        indent(indentation);
        std::cout << "FileName \"" << filename << "\"\n";
    }

    std::string filename;
};

}
