#ifndef REPLACER_VIEW_ITEM_HPP
#define REPLACER_VIEW_ITEM_HPP

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
    view_item(const fs::path &p,
              const std::string &line,
              long position,
              long length,
              const std::regex &re,
              const std::string &with);

    void draw(int line, bool selected, ncursespp::rect_i r) const;
    int height() const;

private:
    fs::path path_;
    std::string line_before_[3];
    std::string line_after_[3];
};

} // namespace replacer

#endif // REPLACER_VIEW_ITEM_HPP
