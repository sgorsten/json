#define JSON_H_IMPLEMENTATION
#include "../json.h"

#include <iostream>

int main()
{
    std::cout << cu::tabbed(cu::JsonObject{
        {"name", "sterling"},
        {"age", 29},
        {"likes xml", false},
        {"public domain projects", cu::JsonArray{"json.h", "linalg.h"}},
    }, 4) << std::endl;
	return 0;
}