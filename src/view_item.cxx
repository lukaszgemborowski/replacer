#include "view_item.hpp"
#include "theme.hpp"
#include <ncurses++/drawing.hpp>
#include <array>
#include <ncurses.h> // TODO: no attron/off support in ncurses++

namespace replacer
{

view_item::view_item(const fs::path &p)
    : path_ {p}
    , match_ {nullptr}
{}

view_item::view_item(const fs::path &p, match &m)
    : path_ {p}
    , match_ {&m}
{
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


    if (!match_) {
        ncursespp::draw::text(path_.string(), r, Colors[0][selected]);
    } else {
        auto make_rect = [&](int x, int y) {
            return ncursespp::rect_i {
                {r.left_top.x + x, r.left_top.y + y},
                {r.left_top.x + r.width(), r.left_top.y + y}
            };
        };

        auto prefix = match_->prefix();
        auto matched = match_->matched();
        auto suffix = match_->suffix();
        auto with = match_->replaced();

        if (match_->for_apply()) {
            attron(A_STANDOUT);
        }

        // text before the change
        int x = 3;
        x += ncursespp::draw::text(prefix, make_rect(x, 0), Colors[1][selected]);
        x += ncursespp::draw::text(matched, make_rect(x, 0), Colors[2][selected]);
             ncursespp::draw::text(suffix, make_rect(x, 0), Colors[1][selected]);

        // text after the change
        x = 3;
        x += ncursespp::draw::text(prefix, make_rect(x, 1), Colors[3][selected]);
        x += ncursespp::draw::text(with, make_rect(x, 1), Colors[4][selected]);
             ncursespp::draw::text(suffix, make_rect(x, 1), Colors[3][selected]);

        // fill reminign space on the left of the item
        auto space = r;
        space.right_bottom.x = 2;
        space.right_bottom.y --; // FIXME: this is a bug
        ncursespp::draw::fill_rect(space, Colors[1][selected]);

        if (match_->for_apply()) {
            attroff(A_STANDOUT);
        }
    }
}

int view_item::height() const
{
    if (!match_) {
        return 1;
    } else {
        return 2;
    }
}

void view_item::accept()
{
    if (match_) {
        match_->toggle_apply();
    }
}

} // namespace replacer
