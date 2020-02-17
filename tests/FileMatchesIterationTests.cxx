#include "catch.hpp"
#include <replacer/FileMatches.hpp>
#include <map>

using namespace std::string_literals;
using namespace std::string_view_literals;
using namespace replacer;

TEST_CASE("Match iteration", "[FileMatches]")
{
    std::stringstream ss{
        "first line\nsecond line\nthird line"s
    };

    std::regex re{"first|line"};
    auto matches = FileMatches::Create("foo.cpp", re, ss);

    auto matchCountPerLine = std::map<int, std::vector<std::string>> {
        {0, {"first", "line"}}, // 0: "first" and "line"
        {1, {"line"}}, // 1: "line"
        {2, {"line"}}  // 2: "line"
    };

    for (auto &line : matches)
    {
        auto ref = matchCountPerLine[*line.lineNumber()];

        // matched count is the same
        REQUIRE(ref.size() == line.size());

        auto idx = 0u;
        for (auto &m : line)
        {
            // compare every match with a reference match map
            REQUIRE(line.matchedString(m) == ref[idx]);
            idx ++;
        }
    }
}