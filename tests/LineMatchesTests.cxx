#include "catch.hpp"
#include <replacer/LineMatches.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;
using namespace replacer;

TEST_CASE("No matches in line", "[LineMatches]")
{
    auto input = "Lorem ipsum dolor sit amet"s;
    auto rex = std::regex{"foo"};
    auto result = LineMatches::CreateMatches(LineNumber{0}, input, rex);

    REQUIRE(std::holds_alternative<NoMatches>(result));
}

TEST_CASE("One match in line", "[LineMatches]")
{
    auto input = "Lorem ipsum dolor sit amet"s;
    auto rex = std::regex{"Lorem"};
    auto result = LineMatches::CreateMatches(LineNumber{0}, input, rex);

    REQUIRE(std::holds_alternative<NoMatches>(result) == false);
    auto matches = std::get<LineMatches>(result);

    REQUIRE(matches.getMatches().size() == 1);
    auto match = matches.getMatches()[0];

    REQUIRE(match.position() == 0);
    REQUIRE(match.length() == 5);

    REQUIRE("Lorem"sv == matches.matchedString(match));
}

TEST_CASE("Two matches in line", "[LineMatches]")
{
    auto input = "Lorem ipsum dolor sit amet"s;
    auto rex = std::regex{"Lorem|ipsum"};
    auto result = LineMatches::CreateMatches(LineNumber{0}, input, rex);

    REQUIRE(std::holds_alternative<NoMatches>(result) == false);
    auto matches = std::get<LineMatches>(result);

    REQUIRE(matches.getMatches().size() == 2);

    auto match0 = matches.getMatches()[0];
    REQUIRE(match0.position() == 0);
    REQUIRE(match0.length() == 5);
    REQUIRE("Lorem"sv == matches.matchedString(match0));

    auto match1 = matches.getMatches()[1];
    REQUIRE(match1.position() == 6);
    REQUIRE(match1.length() == 5);
    REQUIRE("ipsum"sv == matches.matchedString(match1));
}
