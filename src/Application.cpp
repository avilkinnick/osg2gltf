#include "Application.hpp"

#include "gltf/Buffer.hpp"
#include "gltf/Mesh.hpp"
#include "gltf/Node.hpp"
#include "gltf/Scene.hpp"
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
#include <utility>

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
                        texcoord.y = (1.0f - texcoord.y);
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

    // root->print();
}

void Application::convert_data()
{
    convert_data(root->children[0]);
    int a = 10;
}

void Application::write_data_to_output_files()
{
    std::ofstream bin_file(output_filename + ".bin", std::ios::binary);
    for (const auto& [osg_id, subbuffer] : buffer.subbuffers)
    {
        bin_file.write(reinterpret_cast<const char*>(subbuffer.positions.data()), subbuffer.positions_byte_size);
        bin_file.write(reinterpret_cast<const char*>(subbuffer.normals.data()), subbuffer.normals_byte_size);
        bin_file.write(reinterpret_cast<const char*>(subbuffer.texcoords.data()), subbuffer.texcoords_byte_size);
        bin_file.write(reinterpret_cast<const char*>(subbuffer.indices.data()), subbuffer.indices_byte_size);
    }

    std::string gltf_content;

    gltf_content += "{\n"
        "    \"asset\": {\n"
        "        \"version\": \"2.0\"\n"
        "    },\n"
        "    \"scenes\": [\n"
        "        {\n"
        "            \"nodes\": [0]\n"
        "        }\n"
        "    ],\n"
        "    \"scene\": 0,\n"
        "    \"nodes\": [\n";

    write_node(gltf_content, scene.root_node);
    gltf_content.pop_back();
    gltf_content.pop_back();
    gltf_content += '\n';

    gltf_content += "    ],\n"
        "    \"buffers\": [\n"
        "        {\n"
        "            \"byteLength\": " + std::to_string(buffer.size) + ",\n"
        "            \"uri\": \"" + output_filename + ".bin\"\n" 
        "        }\n"
        "    ],\n"
        "    \"bufferViews\": [\n";

    for (const auto& [osg_id, subbuffer] : buffer.subbuffers)
    {
        gltf_content +=
            "        {\n"
            "            \"buffer\": 0,\n"
            "            \"byteLength\": " + std::to_string(subbuffer.indices_offset - subbuffer.positions_offset) + ",\n"
            "            \"byteOffset\": " + std::to_string(subbuffer.positions_offset) + ",\n"
            "            \"target\": 34962\n"
            "        },\n"
            "        {\n"
            "            \"buffer\": 0,\n"
            "            \"byteLength\": " + std::to_string(subbuffer.indices_byte_size) + ",\n"
            "            \"byteOffset\": " + std::to_string(subbuffer.indices_offset) + ",\n"
            "            \"target\": 34963\n"
            "        },\n";
    }
    gltf_content.pop_back();
    gltf_content.pop_back();
    gltf_content += '\n';

    gltf_content += "    ],\n"
        "    \"accessors\": [\n";

    int i = 0;
    for (const auto& [osg_id, subbuffer] : buffer.subbuffers)
    {
        osg::Vec3f min = subbuffer.positions[0];
        osg::Vec3f max = subbuffer.positions[0];

        for (const auto& pos : subbuffer.positions)
        {
            if (pos.x < min.x) min.x = pos.x;
            if (pos.y < min.y) min.y = pos.y;
            if (pos.z < min.z) min.z = pos.z;
            if (pos.x > max.x) max.x = pos.x;
            if (pos.y > max.y) max.y = pos.y;
            if (pos.z > max.z) max.z = pos.z;
        }

        gltf_content +=
            "        {\n"
            "            \"bufferView\": " + std::to_string(i) + ",\n"
            "            \"byteOffset\": 0,\n"
            "            \"componentType\": 5126,\n"
            "            \"count\": " + std::to_string(subbuffer.positions.size()) + ",\n"
            "            \"min\": [" + std::to_string(min.x) + ", " + std::to_string(min.y) + ", " + std::to_string(min.z) + "],\n"
            "            \"max\": [" + std::to_string(max.x) + ", " + std::to_string(max.y) + ", " + std::to_string(max.z) + "],\n"
            "            \"type\": \"VEC3\"\n"
            "        },\n"
            "        {\n"
            "            \"bufferView\": " + std::to_string(i) + ",\n"
            "            \"byteOffset\": " + std::to_string(subbuffer.normals_offset - subbuffer.positions_offset) + ",\n"
            "            \"componentType\": 5126,\n"
            "            \"count\": " + std::to_string(subbuffer.normals.size()) + ",\n"
            "            \"type\": \"VEC3\"\n"
            "        },\n"
            "        {\n"
            "            \"bufferView\": " + std::to_string(i) + ",\n"
            "            \"byteOffset\": " + std::to_string(subbuffer.texcoords_offset - subbuffer.positions_offset) + ",\n"
            "            \"componentType\": 5126,\n"
            "            \"count\": " + std::to_string(subbuffer.texcoords.size()) + ",\n"
            "            \"type\": \"VEC2\"\n"
            "        },\n"
            "        {\n"
            "            \"bufferView\": " + std::to_string(i + 1) + ",\n"
            "            \"byteOffset\": 0,\n"
            "            \"componentType\": 5125,\n"
            "            \"count\": " + std::to_string(subbuffer.indices.size()) + ",\n"
            "            \"type\": \"SCALAR\"\n"
            "        },\n";

            i += 2;
    }
    gltf_content.pop_back();
    gltf_content.pop_back();
    gltf_content += '\n';

    gltf_content += "    ],\n"
        "    \"images\": [\n";

    for (const auto& [id, image] : images)
    {
        gltf_content +=
            "        {\n"
            "            \"uri\": \"" + image.second->filename + "\"\n"
            "        },\n";
    }
    gltf_content.pop_back();
    gltf_content.pop_back();
    gltf_content += '\n';

    gltf_content += "    ],\n"
        "    \"samplers\": [\n"
        "        {\n"
        "            \"magFilter\": 9729,\n"
        "            \"minFilter\": 9987,\n"
        "            \"wrapS\": 10497,\n"
        "            \"wrapT\": 10497\n"
        "        }\n"
        "    ],\n"
        "    \"textures\": [\n";

    for (const auto& [id, texture] : textures)
    {
        gltf_content +=
            "        {\n"
            "            \"sampler\": 0,\n"
            "            \"source\": " + std::to_string(images[texture.second->image->unique_id].first) + "\n"
            "        },\n";
    }
    gltf_content.pop_back();
    gltf_content.pop_back();
    gltf_content += '\n';

    gltf_content += "    ],\n"
        "    \"materials\": [\n";

    for (const auto& [id, material] : materials)
    {
        gltf_content +=
            "        {\n"
            "            \"name\": \"" + material.second->name + "\",\n"
            "            \"pbrMetallicRoughness\": {\n"
            "                \"baseColorTexture\": {\n"
            "                    \"index\": " + std::to_string(textures[material.second->texture->unique_id].first) + ",\n"
            "                    \"texCoord\": 0\n"
            "                }\n"
            "            },\n"
            "            \"alphaMode\": \"" + (material.second->blend ? "OPAQUE" : "BLEND") + "\"\n"
            "        },\n";
    }
    gltf_content.pop_back();
    gltf_content.pop_back();
    gltf_content += '\n';

    gltf_content += "    ],\n"
        "    \"meshes\": [\n";
    
    for (const auto& [id, mesh] : meshes)
    {
        gltf_content +=
            "        {\n"
            "            \"name\": \"" + mesh.second->name + "\",\n"
            "            \"primitives\": [\n"
            "                {\n"
            "                    \"attributes\": {\n"
            "                        \"POSITION\": " + std::to_string(mesh.first * 4) + ",\n"
            "                        \"NORMAL\": " + std::to_string(mesh.first * 4 + 1) + ",\n"
            "                        \"TEXCOORD_0\": " + std::to_string(mesh.first * 4 + 2) + "\n"
            "                    },\n"
            "                    \"indices\": " + std::to_string(mesh.first * 4 + 3) + ",\n"
            "                    \"material\": " + std::to_string(materials[mesh.second->state_set->unique_id].first) + ",\n"
            "                    \"mode\": 4\n"
            "                }\n"
            "            ]\n"
            "        },\n";
    }
    gltf_content.pop_back();
    gltf_content.pop_back();
    gltf_content += '\n';

    gltf_content += "    ]\n"
        "}";

    std::ofstream gltf_file(output_filename + ".gltf");
    gltf_file << gltf_content;
}

void Application::write_node(std::string& gltf_content, std::shared_ptr<gltf::Node> node)
{
    gltf_content += "        {\n"
        "            \"name\": \"" + node->name + "\"";

    if (!node->children.empty())
    {
        gltf_content += ",\n"
            "            \"children\": [";

        for (const auto& [osg_id, child] : node->children)
        {
            gltf_content += std::to_string(child.first) + ", ";
        }
        gltf_content.pop_back();
        gltf_content.pop_back();
        gltf_content += ']';
    }

    if (node->mesh.second)
    {
        gltf_content += ",\n"
            "            \"mesh\": " + std::to_string(node->mesh.first);
    }

    if (node->has_matrix)
    {
        gltf_content += ",\n"
            "            \"matrix\": [\n";

        for (int i = 0; i < 4; ++i)
        {
            gltf_content += "                ";
            for (int j = 0; j < 4; ++j)
            {
                gltf_content += std::to_string(node->matrix[i][j]);
                if (j < 3)
                {
                    gltf_content += ", ";
                }
                else if (i < 3)
                {
                    gltf_content += ',';
                }
            }
            gltf_content += '\n';
        }

        gltf_content += "            ]";
    }

    gltf_content += "\n"
        "        },\n";

    for (const auto& [osg_id, child] : node->children)
    {
        write_node(gltf_content, child.second);
    }
}

void Application::convert_data(std::shared_ptr<osg::Object> object)
{
    static unsigned int free_node_id = 0;

    using Unique_osg_id = unsigned int;
    using Id_node_pair = std::pair<unsigned int, std::shared_ptr<gltf::Node>>;

    static std::map<Unique_osg_id, Id_node_pair> node_map;

    if (auto group = std::dynamic_pointer_cast<osg::Group>(object))
    {
        auto node = std::make_shared<gltf::Node>();
        node->id = free_node_id;
        ++free_node_id;
        node->name = group->name;
        node_map.insert({group->unique_id, {node->id, node}});

        for (const auto& child : group->children)
        {
            convert_data(child);
            node->children.insert({child->unique_id, node_map[child->unique_id]});
        }

        if (auto matrix_transform = std::dynamic_pointer_cast<osg::Matrix_transform>(group))
        {
            node->has_matrix = true;
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    node->matrix[i][j] = matrix_transform->matrix[i][j];
                }
            }
        }

        if (node->id == 0)
        {
            scene.root_node = node;
        }

        if (auto state_set = group->state_set)
        {
            if (auto texture = state_set->texture)
            {
                if (auto image = texture->image)
                {
                    if (!images.count(image->unique_id))
                    {
                        images.insert({image->unique_id, {images.size(), image}});
                    }
                }

                if (!textures.count(texture->unique_id))
                {
                    textures.insert({texture->unique_id, {textures.size(), texture}});
                }

                if (!materials.count(state_set->unique_id))
                {
                    materials.insert({state_set->unique_id, {materials.size(), state_set}});
                }
            }
        }
    }
    else if (auto geometry = std::dynamic_pointer_cast<osg::Geometry>(object))
    {
        auto node = std::make_shared<gltf::Node>();
        node->id = free_node_id;
        ++free_node_id;
        node->name = geometry->name;
        node_map.insert({geometry->unique_id, {node->id, node}});

        static std::size_t offset = 0;

        gltf::Subbuffer subbuffer;

        subbuffer.positions_offset = offset;
        subbuffer.positions_byte_size = geometry->vertices.size() * sizeof(osg::Vec3f);
        subbuffer.positions = geometry->vertices;

        subbuffer.normals_offset = subbuffer.positions_offset + subbuffer.positions_byte_size;
        subbuffer.normals_byte_size = geometry->normals.size() * sizeof(osg::Vec3f);
        subbuffer.normals = geometry->normals;

        subbuffer.texcoords_offset = subbuffer.normals_offset + subbuffer.normals_byte_size;
        subbuffer.texcoords_byte_size = geometry->texcoords.size() * sizeof(osg::Vec2f);
        subbuffer.texcoords = geometry->texcoords;

        subbuffer.indices_offset = subbuffer.texcoords_offset + subbuffer.texcoords_byte_size;
        subbuffer.indices_byte_size = geometry->vertices.size() * sizeof(std::uint32_t);
        subbuffer.indices.resize(geometry->vertices.size());
        for (std::size_t i = 0; i < geometry->vertices.size(); ++i)
        {
            subbuffer.indices[i] = i;
        }

        offset = subbuffer.indices_offset + subbuffer.indices_byte_size;

        buffer.subbuffers.insert({geometry->unique_id, subbuffer});

        buffer.size = offset;

        if (auto state_set = geometry->state_set)
        {
            if (auto texture = state_set->texture)
            {
                if (auto image = texture->image)
                {
                    if (!images.count(image->unique_id))
                    {
                        images.insert({image->unique_id, {images.size(), image}});
                    }
                }

                if (!textures.count(texture->unique_id))
                {
                    textures.insert({texture->unique_id, {textures.size(), texture}});
                }

                if (!materials.count(state_set->unique_id))
                {
                    materials.insert({state_set->unique_id, {materials.size(), state_set}});
                }
            }
        }

        if (!meshes.count(geometry->unique_id))
        {
            meshes.insert({geometry->unique_id, {meshes.size(), geometry}});
        }

        auto mesh_index = meshes[geometry->unique_id].first;
        
        auto mesh = std::make_shared<gltf::Mesh>();
        mesh->positions_index = mesh_index * 4;
        mesh->normals_index = mesh_index * 4 + 1;
        mesh->texcoords_index = mesh_index * 4 + 2;
        mesh->indices_index = mesh_index * 4 + 3;

        node->mesh = {mesh_index, mesh};
    }
}
