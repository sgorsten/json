#define JSON_H_IMPLEMENTATION
#include "../json.h"

#define CATCH_CONFIG_MAIN
#include "thirdparty/catch.hpp"

TEST_CASE( "constructors" )
{
    // Default constructing or constructing with nullptr should produce null values
    REQUIRE( json::value{}.is_null() );
    REQUIRE( json::value{nullptr}.is_null() );

    // Constructing with booleans should produce true or false values
    REQUIRE( json::value{true}.is_true() );
    REQUIRE( json::value{false}.is_false() );

    // Constructing with a C-string should produce string values
    REQUIRE( json::value{"Hello"}.is_string() );
    REQUIRE( json::value{"Hello"}.contents() == "Hello" );
    REQUIRE( json::value{"Hello"}.string() == "Hello" );

    // Constructing with a std::string should produce string values
    REQUIRE( json::value{std::string{"Hello"}}.is_string() );
    REQUIRE( json::value{std::string{"Hello"}}.contents() == "Hello" );
    REQUIRE( json::value{std::string{"Hello"}}.string() == "Hello" );

    // Constructing with an int should produce number values
    REQUIRE( json::value{10}.is_number() );
    REQUIRE( json::value{10}.contents() == "10" );
    REQUIRE( json::value{10}.number<int>() == 10 );

    // Constructing with a double should produce number values
    REQUIRE( json::value{20.1}.is_number() );
    REQUIRE( json::value{20.1}.contents() == "20.1" );
    REQUIRE( json::value{20.1}.number<double>() == Approx(20.1) );

    // Constructing with a float should produce number values
    REQUIRE( json::value{33.2f}.is_number() );
    REQUIRE( json::value{33.2f}.contents() == "33.2" );
    REQUIRE( json::value{33.2f}.number<float>() == Approx(33.2f) );

    // Constructing with a char should produce number values (so that int8_t/uint8_t work correctly)
    REQUIRE( json::value{'A'}.is_number() );
    REQUIRE( json::value{'A'}.contents() == "65" );
    REQUIRE( json::value{'A'}.number<char>() == 'A' );

    // Constructing with a json::array should produce array values
    json::value array_value = json::array{2,3.14f,"foo",false,nullptr};
    REQUIRE( array_value.is_array() );
    REQUIRE( array_value.array().size() == 5 );
    REQUIRE( array_value[0] == 2 );
    REQUIRE( array_value[1] == 3.14f );
    REQUIRE( array_value[2] == "foo" );
    REQUIRE( array_value[3] == false );
    REQUIRE( array_value[4] == nullptr );

    // Constructing with a json::object should produce object values
    json::value object_value = json::object{{"a",2},{"b",3.14f},{"c","foo"},{"d",false},{"e",nullptr}};
    REQUIRE( object_value.is_object() );
    REQUIRE( object_value.object().size() == 5 );
    REQUIRE( object_value["a"] == 2 );
    REQUIRE( object_value["b"] == 3.14f );
    REQUIRE( object_value["c"] == "foo" );
    REQUIRE( object_value["d"] == false );
    REQUIRE( object_value["e"] == nullptr );
}

std::string packed_print(const json::value & v)
{
    std::ostringstream ss;
    ss << v;
    return ss.str();
}

TEST_CASE( "printing" )
{
    REQUIRE( packed_print(5) == "5" );
    REQUIRE( packed_print(3.14f) == "3.14" );
    REQUIRE( packed_print("foo") == "\"foo\"" );
    REQUIRE( packed_print(true) == "true" );
    REQUIRE( packed_print(false) == "false" );
    REQUIRE( packed_print(nullptr) == "null" );

    auto array = json::array{1,2,3,4,5};
    REQUIRE( packed_print(array) == "[1,2,3,4,5]" );

    auto object = json::object{{"a",1},{"c",3},{"b",2},{"a",4}};
    REQUIRE( object.size() == 3 );                                  // json::object should ignore duplicate keys
    REQUIRE( packed_print(object) == "{\"a\":1,\"b\":2,\"c\":3}" ); // json::object should print alphabetically by key order
}

// TODO: Test printing of escape sequences
// TODO: Test prettyprinting with tabbed(...)
// TODO: Test parsing
// TODO: Test parsing of escape sequences
// TODO: Test round-tripping of value -> json-encoded text -> value
// TODO: Test round-tripping of (well behaved) json-encoded text -> value -> json-encoded text
// TODO: Test equality comparison between parses of semantically identical but textually different JSON
