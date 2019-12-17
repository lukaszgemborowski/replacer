// ncurses++ library
#include <ncurses++/session.hpp>

// internal
#include "theme.hpp"
#include "file.hpp"
#include "match.hpp"
#include "options.hpp"
#include "application.hpp"

// std library
#include <iostream>
#include <regex>


int main(int argc, char **argv)
{
    replacer::options opt{argc, argv};

    if (!opt.expression || !opt.with) {
        std::cerr << "You need to specify -w and -e to search and replace" << std::endl;
        return 0;
    }

    std::regex re{*opt.expression.value(), std::regex::optimize};
    auto ses = ncursespp::session{};
    replacer::create_theme();

    replacer::application app{ses, opt.search_dir(), re, *opt.with.value()};
    return app.run();
}
