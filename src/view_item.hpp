#ifndef REPLACER_VIEW_ITEM_HPP
#define REPLACER_VIEW_ITEM_HPP

#include "file.hpp"
#include "match.hpp"

#include <ncurses++/rect.hpp>
#include <regex>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

namespace replacer
{

class view_item
{
public:
    view_item(const fs::path &p);
    view_item(const fs::path &p, match &m);

    void draw(int line, bool selected, ncursespp::rect_i r) const;
    int height() const;
    void accept();

private:
    fs::path path_;
    match *match_; // TODO: try to make it a reference
};

} // namespace replacer

#endif // REPLACER_VIEW_ITEM_HPP
