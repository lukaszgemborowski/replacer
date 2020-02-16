#include "catch.hpp"
#include <replacer/LineReplacer.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;
using namespace replacer;

TEST_CASE("Replace parts of the line", "[LineReplacer]")
{
    auto input = "a bb ccc dddd eee ff g"s;
    auto rex = std::regex{"a|bb|ccc|dddd|eee|ff|g"};
    auto result = LineMatches::CreateMatches(LineNumber{0}, input, rex);
    auto matches = std::get<LineMatches>(result);

    auto replacer = LineReplacer(matches);

    SECTION("replace first match")
    {
        REQUIRE(replacer.replace("XXX", {0}) == "XXX bb ccc dddd eee ff g"s);
    }

    SECTION("replace first two")
    {
        REQUIRE(replacer.replace("XXX", {0, 1}) == "XXX XXX ccc dddd eee ff g"s);
    }

    SECTION("replace first and last")
    {
        REQUIRE(replacer.replace("XXX", {0, 6}) == "XXX bb ccc dddd eee ff XXX"s);
    }
}