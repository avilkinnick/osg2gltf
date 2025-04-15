#pragma once

#include <iostream>
#include <string>

namespace osg
{

struct Object
{
    virtual ~Object() = default;

    void print(int indentation = 0) const
    {
        indent(indentation);
        std::cout << struct_name << ' ' << this << " {\n";

        indent(indentation + 1);
        std::cout << "UniqueID " << unique_id << '\n';

        indent(indentation + 1);
        std::cout << "Name \"" << name << "\"\n";

        print_custom_content(indentation + 1);
        
        indent(indentation);
        std::cout << '}' << std::endl;
    }

    void indent(int indentation) const
    {
        for (int i = 0; i < indentation; ++i)
        {
            std::cout << "  ";
        }
    }

    virtual void print_custom_content(int indentation) const = 0;

    const char* struct_name;
    unsigned int unique_id;
    std::string name;
};

}
