#pragma once

#include <memory>
#include <fstream>
#include <string>

namespace osg
{
    struct Group;
}

class Application final
{
public:
    void read_arguments(int argc, char* argv[]);
    void open_input_file();
    void read_data_from_input_file();
    void convert_data();
    void write_data_to_output_files();

private:
    std::string input_filename;
    std::string output_filename;
    std::ifstream input_file;
    std::shared_ptr<osg::Group> root;
};
