#ifndef REPLACER_SEARCH_RESULTS_HPP
#define REPLACER_SEARCH_RESULTS_HPP

#include "filesystem.hpp"
#include "file.hpp"
#include <regex>
#include <functional>
#include <map>

namespace replacer
{

class search_results
{
    using Callback = std::function<void (const fs::path&, match &)>;
    using FileMap = std::map<fs::path, file>;

public:
    search_results(
        const fs::path &root,
        const std::regex &re,
        const std::string &with
    );

    void search(Callback callback);

private:
    void run_search();

private:
    fs::path                        root_;
    std::regex                      re_;
    std::string                     with_;
    Callback                        callback_;
    FileMap                         files_;
};

} // namespace replacer

#endif // REPLACER_SEARCH_RESULTS_HPP
