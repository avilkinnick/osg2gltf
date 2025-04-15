#pragma once

#include "Object.hpp"
#include "State_set.hpp"

#include <memory>
#include <vector>

namespace osg
{

struct Vec2f
{
    float x;
    float y;
};

struct Vec3f
{
    float x;
    float y;
    float z;
};

struct Geometry : public Object
{
    Geometry() { struct_name = "osg::Geometry"; }

    void print_custom_content(int indentation) const override
    {
        if (state_set)
        {
            state_set->print(indentation);
        }

        indent(indentation);
        std::cout << "Vertices " << vertices.size() << " {\n";
        for (const auto& vertex : vertices)
        {
            indent(indentation + 1);
            std::cout << vertex.x << ' ' << vertex.y << ' ' << vertex.z << '\n';
        }
        indent(indentation);
        std::cout << "}\n";

        indent(indentation);
        std::cout << "Normals " << normals.size() << " {\n";
        for (const auto& normal : normals)
        {
            indent(indentation + 1);
            std::cout << normal.x << ' ' << normal.y << ' ' << normal.z << '\n';
        }
        indent(indentation);
        std::cout << "}\n";

        indent(indentation);
        std::cout << "Texcoords " << texcoords.size() << " {\n";
        for (const auto& texcoord : texcoords)
        {
            indent(indentation + 1);
            std::cout << texcoord.x << ' ' << texcoord.y << '\n';
        }
        indent(indentation);
        std::cout << "}\n";
    }

    std::shared_ptr<State_set> state_set;
    std::vector<Vec3f> vertices;
    std::vector<Vec3f> normals;
    std::vector<Vec2f> texcoords;
};

}
