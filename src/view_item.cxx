#include "view_item.hpp"
#include <ncurses++/drawing.hpp>

namespace replacer
{

view_item::view_item(const fs::path &p)
    : path_ {p}
    , line_before_ {}
    , line_after_ {}
{}

view_item::view_item(const fs::path &p, const std::string &b, const std::string &a)
    : path_ {p}
    , line_before_ {b}
    , line_after_ {a}
{}

void view_item::draw(int line, bool selected, ncursespp::rect_i r) const
{
    using namespace ncursespp;

    if (line_before_.size() == 0 && line_after_.size() == 0) {
        ncursespp::draw::text(path_.string(), r, 1);
        //ncursespp::draw::text(path_.string(), r, color_pair{color::white, color::black});
    } else {
        auto max_width = r.width();
        auto b_r = ncursespp::rect_i {
            {r.left_top.x + 2, r.left_top.y},
            {max_width, r.left_top.y}
        };

        auto a_r = ncursespp::rect_i {
            {r.left_top.x + 2, r.left_top.y + 1},
            {max_width, r.left_top.y + 1}
        };

        ncursespp::draw::text(line_before_, b_r, 2);
        ncursespp::draw::text(line_after_, a_r, 3);
        //ncursespp::draw::text(line_before_, b_r, color_pair{color::red, color::black});
        //ncursespp::draw::text(line_after_, a_r, color_pair{color::green, color::black});
    }
}

int view_item::height() const
{
    if (line_before_.size() == 0 && line_after_.size() == 0) {
        return 1;
    } else {
        return 2;
    }
}

} // namespace replacer
