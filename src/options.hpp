#ifndef REPLACER_OPTIONS_HPP
#define REPLACER_OPTIONS_HPP

#include "filesystem.hpp"
#include <args/args.hpp>
#include <string>
#include <vector>

namespace replacer
{
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
} // namespace replacer

#endif // REPLACER_OPTIONS_HPP
