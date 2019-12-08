// args library
#include <args/args.hpp>

// ncurses++ library
#include <ncurses++/session.hpp>
#include <ncurses++/item_list.hpp>
#include <ncurses++/palette.hpp>

// internal
#include "view_item.hpp"

// std library
#include <iostream>
#include <regex>
#include <fstream>
#include <sstream>
#include <experimental/filesystem>

#include <ncurses.h>
#include <unistd.h>

namespace fs = std::experimental::filesystem;

struct options
{
    args::opt<std::string> expression{'e'};
    args::opt<std::string> with{'w'};
    std::vector<char *> args;

    options(int argc, char **argv)
    {
        args::parser p{expression, with};
        p.parse(argc, argv);
        args = std::move(p.posargs());
    }

    fs::path search_dir() const
    {
        if (args.size()) {
            return args[0];
        } else {
            return fs::current_path();
        }
    }
};

template<class NcList, class Match>
void print_match(NcList &list, const fs::path &p, const std::string &line, Match &match, const std::regex &re, const std::string &with)
{
    std::string replaced = std::regex_replace(
            line,
            re,
            with);
    list.append(replacer::view_item{p, line, replaced});

/* EXAMPLE: pretty print with crossed-out text and
 * highlited changed part assuming we can use CSI escape codes

    auto foundat = match.position();
    auto length = match.length();
    auto max_len = 40;

    std::stringstream before;
    before << line.substr(0, foundat);
    before << "\x1b[31m\x1b[9m";
    before << line.substr(foundat, length);
    before << "\x1b[0m";
    before << line.substr(foundat + length);

    std::stringstream after;

    std::string replaced = std::regex_replace(
            line,
            re,
            with);
    auto change_length = line.size() - replaced.size();

    after << line.substr(0, foundat);
    after << "\x1b[32m";
    after << replaced.substr(foundat, length - change_length);
    after << "\x1b[0m";
    after << line.substr(foundat + length) << std::endl;

    list.append(
        replacer::view_item{
            p,
            before.str(),
            after.str()
        }
    );*/
}

template<class NcList>
void match_in_file(
    NcList &list,
    const fs::path &p,
    const std::regex &re,
    const std::string &with)
{
    std::string line;
    std::ifstream fs{p};

    list.append(replacer::view_item{p});

    while (std::getline(fs, line)) {
        auto m = std::sregex_iterator(line.begin(), line.end(), re);
        auto e = std::sregex_iterator();

        for (auto i = m; i != e; ++i) {
            auto match = *i;
            print_match(list, p, line, match, re, with);
        }
    }
}

auto create_palette()
{
    using namespace ncursespp;
    return palette {
        color_pair{1_idx, color::white, color::black},
        color_pair{2_idx, color::green, color::black},
        color_pair{3_idx, color::red, color::black}
    };
}

int main(int argc, char **argv)
{
    options opt{argc, argv};

    std::regex re{*opt.expression.value(), std::regex::optimize};

    auto ses = ncursespp::session{};
    auto pal = create_palette();

    auto list = ncursespp::item_list<replacer::view_item>{};

    for (auto &p : fs::recursive_directory_iterator{opt.search_dir()}) {
        if (fs::is_regular_file(p.status())) {
            if (p.path().filename().string()[0] == '.')
                continue;
            match_in_file(list, p.path(), re, *opt.with.value());
        }
    }

    list.select(0);
    list.resize(ses.size());
    ses.refresh();

    pause();

    return 0;
}
