#pragma once

#include <fstream>
#include <string>

class Application
{
public:
    void read_arguments(int argc, char* argv[]);
    void open_input_file();
    void read_data_from_input_file();

private:
    std::string input_filename;
    std::string output_filename;
    std::ifstream input_file;
};
