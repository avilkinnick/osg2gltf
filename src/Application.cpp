#include "Application.hpp"

#include "osg/Group.hpp"
#include "osg/Object.hpp"

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
            auto group = std::make_shared<osg::Group>();
            auto parent = depth_map[depth - 1];
            if (auto parent_group = dynamic_cast<osg::Group*>(parent.get()))
            {
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
        else if (buffer == "UniqueUD")
        {
            input_file >> buffer;
        }
        else if (buffer == "Name")
        {

        }
    }
}
