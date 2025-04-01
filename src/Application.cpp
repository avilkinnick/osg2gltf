#include "Application.hpp"

#include "osg_types/Group.hpp"
#include "osg_types/Node.hpp"

#include <fstream>
#include <map>
#include <memory>
#include <stack>
#include <stdexcept>
#include <string>

using std::string_literals::operator""s;

Application::Application() = default;
Application::~Application() = default;

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
    std::map<unsigned int, std::shared_ptr<osg_types::Node>> node_map;
    std::stack<std::shared_ptr<osg_types::Node>> node_stack;

    root = std::make_shared<osg_types::Group>();
    node_stack.emplace(root);

    std::string buffer;
    while (input_file >> buffer)
    {
        if (buffer == "osg::Group")
        {
            auto group = std::make_shared<osg_types::Group>();

        }
    }
}
