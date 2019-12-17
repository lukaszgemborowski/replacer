// args library
#include <args/args.hpp>

// ncurses++ library
#include <ncurses++/session.hpp>
#include <ncurses++/item_list.hpp>
#include <ncurses++/palette.hpp>

// internal
#include "view_item.hpp"
#include "theme.hpp"
#include "file.hpp"
#include "match.hpp"

// std library
#include <iostream>
#include <regex>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <experimental/filesystem>

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

class file_collector
{
public:
    file_collector() {}

    template<class Fun>
    void walk(const fs::path &path, Fun f)
    {
        for (auto &p : fs::recursive_directory_iterator{path}) {
            if (fs::is_regular_file(p.status())) {
                if (p.path().filename().string()[0] == '.')
                    continue;

                f (p);
            }
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
        file_collector collector;
        collector.walk(path_, [this](auto &e) { match_in_file(e.path()); });

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
            } else if (c == 'a') {
                list_.selected().accept();
            }
        }

        return 0;
    }

    void match_in_file(const fs::path &p)
    {
        replacer::file &f = files_.emplace(p, p).first->second;

        f.iterate(
            [this, &f, &p](int no, auto &line) {
                auto m = std::sregex_iterator(
                    line.begin(),
                    line.end(),
                    re_);
                auto e = std::sregex_iterator();

                for (; m != e; ++m) {
                    std::string w = std::regex_replace(
                        line.substr(m->position(), m->length()),
                        re_,
                        with_
                    );

                    // TODO: this is ok here but needs some thinking.
                    // matches are stored in file's internal vector and also as references
                    // in view_items. As match can be moved it can make a view_item
                    // reference dangling. I.e calling add_match after creating
                    // view_item can break the app!
                    f.add_match(p, no, line, w, m->position(), m->length());
                }
            }
        );

        if (!f.has_matches())
            return;

       list_.append(replacer::view_item{p});
       for (auto &m : f.get_matches()) {
           list_.append(replacer::view_item{p, m});
       }
    }

    ncursespp::session &ses_;
    const fs::path path_;
    const std::regex &re_;
    const std::string &with_;
    ncursespp::item_list<replacer::view_item> list_;
    std::map<fs::path, replacer::file> files_;
};


int main(int argc, char **argv)
{
    options opt{argc, argv};

    if (!opt.expression || !opt.with) {
        std::cerr << "You need to specify -w and -e to search and replace" << std::endl;
        return 0;
    }

    std::regex re{*opt.expression.value(), std::regex::optimize};
    auto ses = ncursespp::session{};
    replacer::create_theme();

    application app{ses, opt.search_dir(), re, *opt.with.value()};
    return app.run();
}
