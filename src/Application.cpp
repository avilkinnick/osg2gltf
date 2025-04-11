#include "Application.hpp"

#include "osg/Group.hpp"
#include "osg/Matrix_transform.hpp"
#include "osg/Object.hpp"
#include "osg/State_set.hpp"

#include <cstddef>
#include <fstream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

using std::string_literals::operator""s;

void Application::read_arguments(int argc, char* argv[])
{
    if (argc != 3)
    {
        throw std::runtime_error("Invalid number of arguments!\nValid usage:\nosg2gltf input_filename output_filename");
    }

    input_filename = argv[1];
    output_filename = argv[2];
}

void Application::open_input_file()
{
    input_file.open(input_filename);
    if (!input_file)
    {
        throw std::runtime_error("Failed to open "s + input_filename);
    }
}

void Application::read_data_from_input_file()
{
    unsigned int depth = 1;
    std::map<unsigned int, std::shared_ptr<osg::Object>> depth_map;
    std::map<unsigned int, std::shared_ptr<osg::Object>> id_map;

    root = std::make_shared<osg::Group>();
    depth_map[0] = root;

    std::string buffer;
    while (input_file >> buffer)
    {
        if (buffer == "osg::Group")
        {
            if (auto parent_group = dynamic_cast<osg::Group*>(depth_map[depth - 1].get()))
            {
                auto group = std::make_shared<osg::Group>();
                parent_group->children.emplace_back(group);
                depth_map[depth] = group;
            }
            else
            {
                throw std::runtime_error("Failed to cast parent");
            }
        }
        else if (buffer == "osg::StateSet")
        {
            if (auto parent_group = dynamic_cast<osg::Group*>(depth_map[depth - 2].get()))
            {
                auto state_set = std::make_shared<osg::State_set>();
                parent_group->state_set = state_set;
                depth_map[depth] = state_set;
            }
            else
            {
                throw std::runtime_error("Failed to cast parent");
            }
        }
        else if (buffer == "osg::MatrixTransform")
        {
            if (auto parent_group = dynamic_cast<osg::Group*>(depth_map[depth - 2].get()))
            {
                auto matrix_transform = std::make_shared<osg::Matrix_transform>();
                parent_group->children.emplace_back(matrix_transform);
                depth_map[depth] = matrix_transform;
            }
            else
            {
                throw std::runtime_error("Failed to cast parent");
            }
        }
        else if (buffer == "{")
        {
            ++depth;
        }
        else if (buffer == "}")
        {
            --depth;
        }
        else if (buffer == "UniqueID")
        {
            unsigned int id;
            input_file >> id;
            if (id_map.count(id))
            {
                // TODO:
            }
            else
            {
                depth_map[depth - 1]->unique_id = id;
                id_map[id] = depth_map[depth - 1];
            }
        }
        else if (buffer == "Name")
        {
            std::getline(input_file, buffer);
            std::size_t quote1_index = buffer.find('"');
            std::size_t quote2_index = buffer.find('"', quote1_index + 1);
            buffer = buffer.substr(quote1_index + 1, quote2_index - quote1_index - 1);
            depth_map[depth - 1]->name = buffer;
        }
        else if (buffer == "GL_BLEND")
        {
            if (auto state_set = dynamic_cast<osg::State_set*>(depth_map[depth - 2].get()))
            {
                input_file >> buffer;
                state_set->blend = (buffer == "ON");
            }
            else
            {
                throw std::runtime_error("GL_BLEND not inside osg::StateSet");
            }
        }
        else if (buffer == "Children")
        {
            if (auto parent_group = dynamic_cast<osg::Group*>(depth_map[depth - 1].get()))
            {
                unsigned int child_count;
                input_file >> child_count;
                parent_group->children.reserve(child_count);
            }
            else
            {
                throw std::runtime_error("Children not inside osg::Group");
            }
        }
    }

    int a = 1;
}
