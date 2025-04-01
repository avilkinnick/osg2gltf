#pragma once

#include <fstream>
#include <memory>
#include <string>

namespace osg_types
{
    struct Group;
};

class Application
{
public:
    Application();
    ~Application();

    void read_arguments(int argc, char* argv[]);
    void open_input_file();
    void read_data_from_input_file();

private:
    std::string input_filename;
    std::string output_filename;
    std::ifstream input_file;

    // TODO: make unique
    std::shared_ptr<osg_types::Group> root;
};
