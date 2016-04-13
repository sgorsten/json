#define JSON_H_IMPLEMENTATION
#include "../json.h"

#include <iostream>

int main() try
{
    std::cout << json::tabbed(json::JsonObject{
        {"name", "sterling"},
        {"age", 29},
        {"likes xml", false},
        {"public domain projects", json::JsonArray{"json.h", "linalg.h"}},
    }, 4) << std::endl;
	return EXIT_SUCCESS;
}
catch(const std::exception & e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}