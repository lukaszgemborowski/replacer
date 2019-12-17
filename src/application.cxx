#include "application.hpp"

namespace
{
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
}

namespace replacer
{
application::application(ncursespp::session &ses, const fs::path &path, const std::regex &re, const std::string &with)
    : ses_ {ses}
    , path_ {path}
    , re_ {re}
    , with_ {with}
    , list_ {}
{
}

int application::run()
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

void application::match_in_file(const fs::path &p)
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
} // namespace replacer
