#include "catch.hpp"
#include "mocks/FsIteratorMock.hpp"
#include "mocks/StreamOpenerMock.hpp"
#include <replacer/IterateFilesystem.hpp>
#include <replacer/FileMatches.hpp>
#include <sstream>
#include <iostream>

using namespace std::string_literals;
using namespace std::string_view_literals;
using namespace replacer;

TEST_CASE("UC_IterateFilesystemAndPrintMatches", "[]")
{
    auto foo1_file =
        "foo1 line1\n"
        "foo1\n"
        "foo1 line3"s;

    auto foo1_path = fs::path{"foo1.txt"};

    auto foo2_file =
        "foo2\n"
        "foo2 line3\n"s;

    auto foo2_path = fs::path{"foo2.txt"};

    auto files = std::map<fs::path, std::string>{
        {foo1_path, foo1_file},
        {foo2_path, foo2_file}
    };

    auto iterator = mocks::FsIteratorMock{{foo1_path, foo2_path}};
    auto acceptAll = [](auto &) { return true; };

    std::regex re{"line"};

    // in normal use case it would be: IterateFilesystem("./", re, []() {});
    IterateFilesystemMatches(
        mocks::StreamOpenerMock(files),
        iterator,
        acceptAll,
        re,
        [&](auto path, auto &lineMatches, auto &match) {
            REQUIRE(lineMatches.matchedString(match) == "line"s);
        }
    );
}
