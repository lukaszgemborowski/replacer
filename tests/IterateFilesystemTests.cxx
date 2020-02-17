#include "catch.hpp"
#include <replacer/IterateFilesystem.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;
using namespace replacer;

struct FsIteratorMock
{
    struct Item
    {
        Item(fs::path p) : path_ {p} {}
        fs::path path_;

        auto path() const { return path_; }
    };

    FsIteratorMock(std::vector<fs::path> items)
    {
        std::copy(items.begin(), items.end(), std::back_inserter(items_));
    }

    std::vector<Item> items_;

    auto begin() { return items_.begin(); }
    auto end() { return items_.end(); }
};

TEST_CASE("Can iterate over set of fles", "[IterateFilesystem]")
{
    auto raw = std::vector<fs::path> {{"foo/bar1", "foo/bar2", "foo3"}};
    auto items = FsIteratorMock {raw};
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
