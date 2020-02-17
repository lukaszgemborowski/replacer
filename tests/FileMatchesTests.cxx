#include "catch.hpp"
#include <replacer/FileMatches.hpp>
#include <sstream>

using namespace std::string_literals;
using namespace std::string_view_literals;
using namespace replacer;

TEST_CASE("File matcher", "[FileMatches]")
{
    std::stringstream ss{
        "first line\nsecond line\nthird line"s
    };

    SECTION("Match first line")
    {
        std::regex re{"first"};
        auto matches = FileMatches::Create("foo.cpp", re, ss);
        REQUIRE(matches.size() == 1);
    }

    SECTION("Match first two lines")
    {
        std::regex re{"first|second"};
        auto matches = FileMatches::Create("foo.cpp", re, ss);
        REQUIRE(matches.size() == 2);
    }

    SECTION("Match all lines")
    {
        std::regex re{"line"};
        auto matches = FileMatches::Create("foo.cpp", re, ss);
        REQUIRE(matches.size() == 3);
    }
}
