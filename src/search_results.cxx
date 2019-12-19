#include "search_results.hpp"
#include <algorithm>

namespace replacer
{
search_results::search_results(
        const fs::path &root,
        const std::regex &re,
        const std::string &with
    )
    : root_ {root}
    , re_ {re}
    , with_ {with}
{
}

void search_results::search(Callback callback)
{
    callback_ = callback;
    run_search();
}

void search_results::run_search()
{
    for (auto &it : fs::recursive_directory_iterator{root_}) {
        if (!fs::is_regular_file(it.status()))
            continue;

        auto &p = it.path();
        file &f = files_.emplace(p, p).first->second;
        f.fill_matches(re_, with_);

        for (auto &m : f.get_matches()) {
            callback_(p, m);
        }
    }
}

} // namespace replacer
