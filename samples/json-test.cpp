#define JSON_H_IMPLEMENTATION
#include "../json.h"

#include <iostream>

int main()
{
    std::cout << json::tabbed(json::JsonObject{
        {"name", "sterling"},
        {"age", 29},
        {"likes xml", false},
        {"public domain projects", json::JsonArray{"json.h", "linalg.h"}},
    }, 4) << std::endl;
	return 0;
}