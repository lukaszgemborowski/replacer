#include "view_item.hpp"
#include "theme.hpp"
#include <ncurses++/drawing.hpp>
#include <array>

namespace replacer
{

view_item::view_item(const fs::path &p)
    : path_ {p}
    , line_before_ {}
    , line_after_ {}
{}

view_item::view_item(const fs::path &p,
              const std::string &line,
              long position,
              long length,
              const std::regex &re,
              const std::string &with)
    : path_ {p}
{
    // TODO: remove duplicates
    line_before_[0] = line.substr(0, position);
    line_before_[1] = line.substr(position, length);
    line_before_[2] = line.substr(position + length);

    std::string res = std::regex_replace(line, re, with);
    auto change_len = line.size() - res.size();

    line_after_[0] = line.substr(0, position);
    line_after_[1] = res.substr(position, length - change_len);
    line_after_[2] = line.substr(position + length);
}

void view_item::draw(int line, bool selected, ncursespp::rect_i r) const
{
    using namespace ncursespp;

    auto val = [](auto i1, auto i2) { return std::array<int, 2>{i1.value, i2.value }; };
    std::array<int, 2> Colors[] = {
        val(LIST_FILENAME, LIST_FILENAME_HL),
        val(LIST_BEFORE_UNCHANGED, LIST_BEFORE_UNCHANGED_HL),
        val(LIST_BEFORE_CHANGED, LIST_BEFORE_CHANGED_HL),
        val(LIST_AFTER_UNCHANGED, LIST_AFTER_UNCHANGED_HL),
        val(LIST_AFTER_CHANGED, LIST_AFTER_CHANGED_HL)
    };


    if (line_before_[1].size() == 0 && line_after_[1].size() == 0) {
        ncursespp::draw::text(path_.string(), r, Colors[0][selected]);
    } else {
        auto make_rect = [&](int x, int y) {
            return ncursespp::rect_i {
                {r.left_top.x + x, r.left_top.y + y},
                {r.left_top.x + r.width(), r.left_top.y + y}
            };
        };

        int x = ncursespp::draw::text(line_before_[0], make_rect(0, 0), Colors[1][selected]);
        x    += ncursespp::draw::text(line_before_[1], make_rect(x, 0), Colors[2][selected]);
        ncursespp::draw::text(line_before_[2], make_rect(x, 0), Colors[1][selected]);

        x  = ncursespp::draw::text(line_after_[0], make_rect(0, 1), Colors[3][selected]);
        x += ncursespp::draw::text(line_after_[1], make_rect(x, 1), Colors[4][selected]);
        ncursespp::draw::text(line_after_[2], make_rect(x, 1), Colors[3][selected]);
    }
}

int view_item::height() const
{
    if (line_before_[1].size() == 0 && line_after_[1].size() == 0) {
        return 1;
    } else {
        return 2;
    }
}

} // namespace replacer
