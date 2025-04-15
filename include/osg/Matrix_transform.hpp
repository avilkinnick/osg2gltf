#pragma once

#include "Group.hpp"

namespace osg
{

struct Matrix_transform : public Group
{
    Matrix_transform() { struct_name = "osg::MatrixTransform"; }

    void print_custom_content(int indentation) const override
    {
        Group::print_custom_content(indentation);

        indent(indentation);
        std::cout << "Matrix {\n";
        for (int i = 0; i < 4; ++i)
        {
            indent(indentation + 1);
            for (int j = 0; j < 4; ++j)
            {
                std::cout << matrix[i][j] << ' ';
            }
            std::cout << '\n';
        }
        indent(indentation);
        std::cout << "}\n";
    }

    float matrix[4][4];
};

}
