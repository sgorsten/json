#define JSON_H_IMPLEMENTATION
#include "../json.h"

#include <iostream>

int main() try
{
    std::cout << json::tabbed(json::object{
        {"name", "sterling"},
        {"age", 29},
        {"likes xml", false},
        {"public domain projects", json::array{"json.h", "linalg.h"}},
    }, 4) << std::endl;
	return EXIT_SUCCESS;
}
catch(const std::exception & e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}