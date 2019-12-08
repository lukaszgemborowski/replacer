#ifndef REPLACER_THEME_HPP
#define REPLACER_THEME_HPP

#include <ncurses++/palette.hpp>
#include <ncurses++/color.hpp>

namespace replacer
{

using ncursespp::operator ""_idx;

constexpr auto MAIN_TEXT = 1_idx;

constexpr auto LIST_FILENAME = 2_idx;
constexpr auto LIST_BEFORE_UNCHANGED = 3_idx;
constexpr auto LIST_BEFORE_CHANGED = 4_idx;
constexpr auto LIST_AFTER_UNCHANGED = 5_idx;
constexpr auto LIST_AFTER_CHANGED = 6_idx;

constexpr auto LIST_FILENAME_HL = 7_idx;
constexpr auto LIST_BEFORE_UNCHANGED_HL = 8_idx;
constexpr auto LIST_BEFORE_CHANGED_HL = 9_idx;
constexpr auto LIST_AFTER_UNCHANGED_HL = 10_idx;
constexpr auto LIST_AFTER_CHANGED_HL = 11_idx;

inline auto create_theme()
{
    using namespace ncursespp;
    auto hl_bg = color::cyan;

    return palette {
        color_pair{MAIN_TEXT              , color::white, color::black},

        color_pair{LIST_FILENAME          , color::white, color::black},
        color_pair{LIST_BEFORE_UNCHANGED  , color::white, color::black},
        color_pair{LIST_BEFORE_CHANGED    , color::red  , color::black},
        color_pair{LIST_AFTER_UNCHANGED   , color::white, color::black},
        color_pair{LIST_AFTER_CHANGED     , color::green, color::black},

        color_pair{LIST_FILENAME_HL          , color::white, hl_bg},
        color_pair{LIST_BEFORE_UNCHANGED_HL  , color::white, hl_bg},
        color_pair{LIST_BEFORE_CHANGED_HL    , color::red  , hl_bg},
        color_pair{LIST_AFTER_UNCHANGED_HL   , color::white, hl_bg},
        color_pair{LIST_AFTER_CHANGED_HL     , color::green, hl_bg},
    };
}

} // namespace replacer

#endif // REPLACER_THEME_HPP
