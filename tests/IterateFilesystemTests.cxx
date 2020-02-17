#include "catch.hpp"
#include "mocks/FsIteratorMock.hpp"
#include <replacer/IterateFilesystem.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;
using namespace replacer;

TEST_CASE("Can iterate over set of fles", "[IterateFilesystem]")
{
    auto raw = std::vector<fs::path> {{"foo/bar1", "foo/bar2", "foo3"}};
    auto items = mocks::FsIteratorMock {raw};
    auto accept = [](auto &) { return true; };
    std::regex re;
    auto idx = 0u;

    IterateFilesystem(
        items,
        accept,
        re,
        [&idx, &raw](auto item) {
            REQUIRE(item.path() == raw[idx]);
            idx ++;
        }
    );
}
