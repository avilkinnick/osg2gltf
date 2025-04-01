#include "Application.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        Application application;
        application.read_arguments(argc, argv);
        application.open_input_file();
        application.read_data_from_input_file();
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
