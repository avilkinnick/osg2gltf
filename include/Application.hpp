#pragma once

#include "gltf/Buffer.hpp"
#include "gltf/Node.hpp"
#include "gltf/Scene.hpp"

#include "osg/Geometry.hpp"
#include "osg/Group.hpp"
#include "osg/Image.hpp"
#include "osg/Object.hpp"
#include "osg/State_set.hpp"
#include "osg/Texture2d.hpp"

#include <memory>
#include <fstream>
#include <string>

class Application final
{
using Unique_osg_id = unsigned int;

public:
    void read_arguments(int argc, char* argv[]);
    void open_input_file();
    void read_data_from_input_file();
    void convert_data();
    void write_data_to_output_files();
    
private:
    void convert_data(std::shared_ptr<osg::Object> object);
    void write_node(std::string& gltf_content, std::shared_ptr<gltf::Node> node);

private:
    std::string input_filename;
    std::string output_filename;
    std::ifstream input_file;
    std::shared_ptr<osg::Group> root;
    gltf::Scene scene;
    gltf::Buffer buffer;
    std::map<Unique_osg_id, std::pair<unsigned int, std::shared_ptr<osg::Geometry>>> meshes;
    std::map<Unique_osg_id, std::pair<unsigned int, std::shared_ptr<osg::State_set>>> materials;
    std::map<Unique_osg_id, std::pair<unsigned int, std::shared_ptr<osg::Texture2d>>> textures;
    std::map<Unique_osg_id, std::pair<unsigned int, std::shared_ptr<osg::Image>>> images;
};
