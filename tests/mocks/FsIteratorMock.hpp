#ifndef LIBREPLACER_TESTS_MOCKS_FSITERATORMOCK_HPP
#define LIBREPLACER_TESTS_MOCKS_FSITERATORMOCK_HPP

#include <replacer/fs.hpp>
#include <vector>
#include <algorithm>

namespace replacer::mocks
{

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

} // namespace replacer::mocks

#endif // LIBREPLACER_TESTS_MOCKS_FSITERATORMOCK_HPP