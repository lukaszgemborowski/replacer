// args library
#include <args/args.hpp>

// ncurses++ library
#include <ncurses++/session.hpp>
#include <ncurses++/item_list.hpp>
#include <ncurses++/palette.hpp>

// internal
#include "view_item.hpp"
#include "theme.hpp"

// std library
#include <iostream>
#include <regex>
#include <fstream>
#include <sstream>
#include <experimental/filesystem>

#include <ncurses.h>

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


struct application
{
    application(ncursespp::session &ses, const fs::path &path, const std::regex &re, const std::string &with)
        : ses_ {ses}
        , path_ {path}
        , re_ {re}
        , with_ {with}
        , list_ {}
    {
    }

    int run()
    {
        for (auto &p : fs::recursive_directory_iterator{path_}) {
            if (fs::is_regular_file(p.status())) {
                if (p.path().filename().string()[0] == '.')
                    continue;
                match_in_file(p.path());
            }
        }

        list_.resize(ses_.size());
        list_.select(0);
        list_.resize(ses_.size());
        ses_.refresh();

        int c = 0;

        while (c != 'q') {
            c = ses_.getkey();

            if (c == 'k') {
                list_.select_up();
            } else if (c == 'j') {
                list_.select_down();
            }
        }

        return 0;
    }

    void match_in_file(const fs::path &p)
    {
        std::string line;
        std::ifstream fs{p};

        list_.append(replacer::view_item{p});

        while (std::getline(fs, line)) {
            auto m = std::sregex_iterator(line.begin(), line.end(), re_);
            auto e = std::sregex_iterator();

            for (auto i = m; i != e; ++i) {
                auto match = *i;

                list_.append(
                    replacer::view_item{
                        p,
                        line,
                        match.position(),
                        match.length(),
                        re_,
                        with_
                    }
                );
            }
        }
    }

    ncursespp::session &ses_;
    const fs::path path_;
    const std::regex &re_;
    const std::string &with_;
    ncursespp::item_list<replacer::view_item> list_;
};


int main(int argc, char **argv)
{
    options opt{argc, argv};
    std::regex re{*opt.expression.value(), std::regex::optimize};
    auto ses = ncursespp::session{};
    replacer::create_theme();

    application app{ses, opt.search_dir(), re, *opt.with.value()};
    return app.run();
}
