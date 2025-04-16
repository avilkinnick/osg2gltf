#pragma once

#include "osg/Group.hpp"
#include "osg/Object.hpp"

// #include <map>
#include <memory>
#include <fstream>
#include <string>

class Application final
{
public:
    void read_arguments(int argc, char* argv[]);
    void open_input_file();
    void read_data_from_input_file();
    void convert_data();
    void write_data_to_output_files();
    
private:
    void convert_data(std::shared_ptr<osg::Object> object);
    // void write_node(std::string& gltf_content, std::shared_ptr<osg::Object> node);

private:
    std::string input_filename;
    std::string output_filename;
    std::ifstream input_file;
    std::shared_ptr<osg::Group> root;
    // std::map<unsigned int, unsigned int> node_map;
    // std::map<unsigned int, unsigned int> mesh_map;
};
