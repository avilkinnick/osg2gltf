#include "Application.hpp"

#include "osg/Geode.hpp"
#include "osg/Geometry.hpp"
#include "osg/Group.hpp"
#include "osg/Image.hpp"
#include "osg/Matrix_transform.hpp"
#include "osg/Object.hpp"
#include "osg/State_set.hpp"
#include "osg/Texture2d.hpp"

#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
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
    try
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
                if (auto parent_group = std::dynamic_pointer_cast<osg::Group>(depth_map[depth - 1]))
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
                if (auto parent_group = std::dynamic_pointer_cast<osg::Group>(depth_map[depth - 2]))
                {
                    auto state_set = std::make_shared<osg::State_set>();
                    parent_group->state_set = state_set;
                    depth_map[depth] = state_set;
                }
                else if (auto parent_geometry = std::dynamic_pointer_cast<osg::Geometry>(depth_map[depth - 2]))
                {
                    auto state_set = std::make_shared<osg::State_set>();
                    parent_geometry->state_set = state_set;
                    depth_map[depth] = state_set;
                }
                else
                {
                    throw std::runtime_error("Failed to cast parent");
                }
            }
            else if (buffer == "osg::MatrixTransform")
            {
                if (auto parent_group = std::dynamic_pointer_cast<osg::Group>(depth_map[depth - 2]))
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
            else if (buffer == "osg::Geode")
            {
                if (auto parent_group = std::dynamic_pointer_cast<osg::Group>(depth_map[depth - 2]))
                {
                    auto geode = std::make_shared<osg::Geode>();
                    parent_group->children.emplace_back(geode);
                    depth_map[depth] = geode;
                }
                else
                {
                    throw std::runtime_error("Failed to cast parent");
                }
            }
            else if (buffer == "osg::Geometry")
            {
                if (auto parent_group = std::dynamic_pointer_cast<osg::Group>(depth_map[depth - 2]))
                {
                    auto geometry = std::make_shared<osg::Geometry>();
                    parent_group->children.emplace_back(geometry);
                    depth_map[depth] = geometry;
                }
                else
                {
                    throw std::runtime_error("Failed to cast parent");
                }
            }
            else if (buffer == "osg::Material" || buffer == "osg::BlendFunc")
            {
                while (buffer != "}")
                {
                    input_file >> buffer;
                }
            }
            else if (buffer == "osg::Texture2D")
            {
                if (auto parent_state_set = std::dynamic_pointer_cast<osg::State_set>(depth_map[depth - 3]))
                {
                    auto texture = std::make_shared<osg::Texture2d>();
                    parent_state_set->texture = texture;
                    depth_map[depth] = texture;
                }
                else
                {
                    throw std::runtime_error("Failed to cast parent");
                }
            }
            else if (buffer == "Image")
            {
                if (auto parent_texture = std::dynamic_pointer_cast<osg::Texture2d>(depth_map[depth - 1]))
                {
                    auto image = std::make_shared<osg::Image>();
                    parent_texture->image = image;
                    depth_map[depth] = image;
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
                    depth_map[depth - 1] = id_map[id];

                    if (auto state_set = std::dynamic_pointer_cast<osg::State_set>(depth_map[depth - 1]))
                    {
                        if (auto parent_group = std::dynamic_pointer_cast<osg::Group>(depth_map[depth - 3]))
                        {
                            parent_group->state_set = state_set;
                        }
                        else if (auto parent_geometry = std::dynamic_pointer_cast<osg::Geometry>(depth_map[depth - 3]))
                        {
                            parent_geometry->state_set = state_set;
                        }
                    }
                    else if (auto image = std::dynamic_pointer_cast<osg::Image>(depth_map[depth - 1]))
                    {
                        if (auto parent_texture = std::dynamic_pointer_cast<osg::Texture2d>(depth_map[depth - 2]))
                        {
                            parent_texture->image = image;
                        }
                    }
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
            else if (buffer == "FileName")
            {
                if (auto parent_image = std::dynamic_pointer_cast<osg::Image>(depth_map[depth - 1]))
                {
                    std::getline(input_file, buffer);
                    std::size_t quote1_index = buffer.find('"');
                    std::size_t quote2_index = buffer.find('"', quote1_index + 1);
                    buffer = buffer.substr(quote1_index + 1, quote2_index - quote1_index - 1);
                    parent_image->filename = buffer;
                }
                else
                {
                    throw std::runtime_error("FileName not inside Image");
                }
            }
            else if (buffer == "GL_BLEND")
            {
                if (auto state_set = std::dynamic_pointer_cast<osg::State_set>(depth_map[depth - 2]))
                {
                    input_file >> buffer;
                    state_set->blend = (buffer == "ON");
                }
                else
                {
                    throw std::runtime_error("GL_BLEND not inside osg::StateSet");
                }
            }
            else if (buffer == "VertexData")
            {
                if (auto geometry = std::dynamic_pointer_cast<osg::Geometry>(depth_map[depth - 1]))
                {
                    while (buffer != "Vec3fArray")
                    {
                        input_file >> buffer;
                    }

                    std::size_t vertex_count;
                    input_file >> vertex_count >> buffer;
                    geometry->vertices.resize(vertex_count);

                    for (auto& vertex : geometry->vertices)
                    {
                        input_file >> vertex.x >> vertex.y >> vertex.z;
                    }

                    input_file >> buffer;

                    do
                    {
                        input_file >> buffer;
                    }
                    while (buffer != "}");
                }
            }
            else if (buffer == "NormalData")
            {
                if (auto geometry = std::dynamic_pointer_cast<osg::Geometry>(depth_map[depth - 1]))
                {
                    while (buffer != "Vec3fArray")
                    {
                        input_file >> buffer;
                    }

                    std::size_t normal_count;
                    input_file >> normal_count >> buffer;
                    geometry->normals.resize(normal_count);

                    for (auto& normal : geometry->normals)
                    {
                        input_file >> normal.x >> normal.y >> normal.z;
                    }

                    input_file >> buffer;

                    do
                    {
                        input_file >> buffer;
                    }
                    while (buffer != "}");
                }
            }
            else if (buffer == "TexCoordData")
            {
                if (auto geometry = std::dynamic_pointer_cast<osg::Geometry>(depth_map[depth - 1]))
                {
                    while (buffer != "Vec2fArray")
                    {
                        input_file >> buffer;
                    }

                    std::size_t texcoord_count;
                    input_file >> texcoord_count >> buffer;
                    geometry->texcoords.resize(texcoord_count);

                    for (auto& texcoord : geometry->texcoords)
                    {
                        input_file >> texcoord.x >> texcoord.y;
                    }

                    input_file >> buffer;

                    do
                    {
                        input_file >> buffer;
                    }
                    while (buffer != "}");

                    input_file >> buffer;
                }
            }
            else if (buffer == "Matrix")
            {
                if (auto matrix_transform = std::dynamic_pointer_cast<osg::Matrix_transform>(depth_map[depth - 1]))
                {
                    input_file >> buffer;

                    for (int i = 0; i < 4; ++i)
                    {
                        for (int j = 0; j < 4; ++j)
                        {
                            input_file >> matrix_transform->matrix[i][j];
                        }
                    }

                    input_file >> buffer;
                }
            }
            else if (buffer == "Children")
            {
                if (auto parent_group = std::dynamic_pointer_cast<osg::Group>(depth_map[depth - 1]))
                {
                    std::size_t child_count;
                    input_file >> child_count;
                    parent_group->children.reserve(child_count);
                }
                else
                {
                    throw std::runtime_error("Children not inside osg::Group");
                }
            }
            else if (buffer == "Drawables")
            {
                if (auto parent_geode = std::dynamic_pointer_cast<osg::Geode>(depth_map[depth - 1]))
                {
                    std::size_t drawable_count;
                    input_file >> drawable_count;
                    parent_geode->children.reserve(drawable_count);
                }
                else
                {
                    throw std::runtime_error("Drawables not inside osg::Geode");
                }
            }
        }
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << std::endl;
    }

    root->print();
}
