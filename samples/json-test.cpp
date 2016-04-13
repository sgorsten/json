#define JSON_H_IMPLEMENTATION
#include "../json.h"

#define CATCH_CONFIG_MAIN
#include "thirdparty/catch.hpp"

TEST_CASE( "constructors" )
{
    // Default constructing or constructing with nullptr should produce null values
    REQUIRE( json::value{}.is_null() );
    REQUIRE( json::value{}.get_kind() == json::kind::null );
    REQUIRE( json::value{nullptr}.is_null() );
    REQUIRE( json::value{nullptr}.get_kind() == json::kind::null );

    // Constructing with booleans should produce true or false values
    REQUIRE( json::value{true}.is_true() );
    REQUIRE( json::value{true}.get_kind() == json::kind::true_ );
    REQUIRE( json::value{false}.is_false() );
    REQUIRE( json::value{false}.get_kind() == json::kind::false_ );

    // Constructing with a C-string should produce string values
    REQUIRE( json::value{"Hello"}.is_string() );
    REQUIRE( json::value{"Hello"}.string() == "Hello" );
    REQUIRE( json::value{"Hello"}.get_kind() == json::kind::string );
    REQUIRE( json::value{"Hello"}.get_contents() == "Hello" );

    // Constructing with a std::string should produce string values
    REQUIRE( json::value{std::string{"Hello"}}.is_string() );
    REQUIRE( json::value{std::string{"Hello"}}.string() == "Hello" );
    REQUIRE( json::value{std::string{"Hello"}}.get_kind() == json::kind::string );
    REQUIRE( json::value{std::string{"Hello"}}.get_contents() == "Hello" );

    // Constructing with an int should produce number values
    REQUIRE( json::value{10}.is_number() );
    REQUIRE( json::value{10}.number<int>() == 10 );
    REQUIRE( json::value{10}.get_kind() == json::kind::number );
    REQUIRE( json::value{10}.get_contents() == "10" );

    // Constructing with a double should produce number values
    REQUIRE( json::value{20.1}.is_number() );
    REQUIRE( json::value{20.1}.number<double>() == Approx(20.1) );
    REQUIRE( json::value{20.1}.get_kind() == json::kind::number );
    REQUIRE( json::value{20.1}.get_contents() == "20.1" );

    // Constructing with a float should produce number values
    REQUIRE( json::value{33.2f}.is_number() );
    REQUIRE( json::value{33.2f}.number<float>() == Approx(33.2f) );
    REQUIRE( json::value{33.2f}.get_kind() == json::kind::number );
    REQUIRE( json::value{33.2f}.get_contents() == "33.2" );

    // Constructing with a char should produce number values (so that int8_t/uint8_t work correctly)
    REQUIRE( json::value{'A'}.is_number() );
    REQUIRE( json::value{'A'}.number<char>() == 'A' );
    REQUIRE( json::value{'A'}.get_kind() == json::kind::number );
    REQUIRE( json::value{'A'}.get_contents() == "65" );

    // Constructing with a json::array should produce array values
    json::value array_value = json::array{2,3.14f,"foo",false,nullptr};
    REQUIRE( array_value.is_array() );
    REQUIRE( array_value.get_array().size() == 5 );
    REQUIRE( array_value[0] == 2 );
    REQUIRE( array_value[1] == 3.14f );
    REQUIRE( array_value[2] == "foo" );
    REQUIRE( array_value[3] == false );
    REQUIRE( array_value[4] == nullptr );
    REQUIRE( array_value.get_kind() == json::kind::array );

    // Constructing with a json::object should produce object values
    json::value object_value = json::object{{"a",2},{"b",3.14f},{"c","foo"},{"d",false},{"e",nullptr}};
    REQUIRE( object_value.is_object() );
    REQUIRE( object_value.get_object().size() == 5 );
    REQUIRE( object_value["a"] == 2 );
    REQUIRE( object_value["b"] == 3.14f );
    REQUIRE( object_value["c"] == "foo" );
    REQUIRE( object_value["d"] == false );
    REQUIRE( object_value["e"] == nullptr );
    REQUIRE( object_value.get_kind() == json::kind::object );
}

std::string packed_print(const json::value & v) { std::ostringstream ss; ss << v; return ss.str(); }
std::string pretty_print(const json::value & v) { std::ostringstream ss; ss << tabbed(v, 4); return ss.str(); }

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

struct vertex { float position[3], normal[3], texcoord[2]; };
struct mesh { std::vector<vertex> vertices; std::vector<std::array<int,3>> triangles; };

template<class F> void visit_fields(vertex & o, F f) { f("v", o.position); f("n", o.normal); f("tc", o.texcoord); }
template<class F> void visit_fields(mesh & o, F f) { f("vertices", o.vertices); f("triangles", o.triangles); }

TEST_CASE( "test json::encode and json::decode" )
{
    const mesh quad_mesh = {
        {
            {{-0.5f,-0.5f,0}, {0,0,1}, {0,0}},
            {{+0.5f,-0.5f,0}, {0,0,1}, {1,0}},
            {{+0.5f,+0.5f,0}, {0,0,1}, {1,1}},
            {{-0.5f,+0.5f,0}, {0,0,1}, {0,1}}
        },
        {
            {{0,1,2}}, {{0,2,3}}
        }
    };
    const char * desired_output = R"({
    "triangles": [
        [0,1,2],
        [0,2,3]
    ],
    "vertices": [
        {
            "n": [0,0,1],
            "tc": [0,0],
            "v": [-0.5,-0.5,0]
        },
        {
            "n": [0,0,1],
            "tc": [1,0],
            "v": [0.5,-0.5,0]
        },
        {
            "n": [0,0,1],
            "tc": [1,1],
            "v": [0.5,0.5,0]
        },
        {
            "n": [0,0,1],
            "tc": [0,1],
            "v": [-0.5,0.5,0]
        }
    ]
})";

    // If we encode the data structure and print it, we should get the desired output
    REQUIRE( pretty_print(to_json(quad_mesh)) == desired_output );

    // If we encode the data structure and parse the desired output, we should get equivalent JSON values
    REQUIRE( to_json(quad_mesh) == json::parse(desired_output) );

    // If we parse the desired output and decode it to a mesh, it should match the original
    mesh decoded_mesh;
    from_json(decoded_mesh, json::parse(desired_output));
    REQUIRE( quad_mesh.vertices.size() == decoded_mesh.vertices.size() );
    REQUIRE( memcmp(quad_mesh.vertices.data(), decoded_mesh.vertices.data(), sizeof(vertex)*quad_mesh.vertices.size()) == 0 );
    REQUIRE( quad_mesh.triangles.size() == decoded_mesh.triangles.size() );
    REQUIRE( memcmp(quad_mesh.triangles.data(), decoded_mesh.triangles.data(), sizeof(int)*3*quad_mesh.triangles.size()) == 0 );
}

// TODO: Test printing of escape sequences
// TODO: Test prettyprinting with tabbed(...)
// TODO: Test parsing
// TODO: Test parsing of escape sequences
// TODO: Test round-tripping of value -> json-encoded text -> value
// TODO: Test round-tripping of (well behaved) json-encoded text -> value -> json-encoded text
// TODO: Test equality comparison between parses of semantically identical but textually different JSON
