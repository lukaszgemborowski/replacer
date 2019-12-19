#include "application.hpp"
#include <set>

namespace replacer
{
application::application(ncursespp::session &ses, const fs::path &path, const std::regex &re, const std::string &with)
    : ses_ {ses}
    , path_ {path}
    , re_ {re}
    , with_ {with}
    , list_ {}
    , search_ {path, re, with}
{
}

int application::run()
{
    std::set<fs::path> found;
    search_.search(
        [this, &found](auto &p, auto &m) {
            if (!found.count(p)) {
                found.insert(p);
                list_.append(view_item{p});
            }

            list_.append(view_item{p, m});
        }
    );

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

} // namespace replacer
