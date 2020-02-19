#include <replacer/IterateFilesystem.hpp>
#include <iostream>

namespace
{
const auto TEXT_GREEN = "\x1b[32m";
const auto TEXT_RED = "\x1b[31m";
const auto TEXT_RESET = "\x1b[0m";
}

class GrepPrinter
{
public:
    void operator()(const fs::path &path, const replacer::LineMatches &lm, replacer::MatchSpan span)
    {
        std::cout << path << ":" << lm.lineNumber() << ": "
            << lm.beforeMatch(span)
            << TEXT_GREEN << lm.matchedString(span) << TEXT_RESET
            << lm.afterMatch(span) << std::endl;
    }
};

class ReplacePrinter
{
public:
    ReplacePrinter(std::string_view replaceWith)
        : replaceWith_ {replaceWith}
    {}

    void operator()(const fs::path &path, const replacer::LineMatches &lm, replacer::MatchSpan span)
    {
        std::cout << path << ":" << lm.lineNumber()
                  << lm.beforeMatch(span)
                  << TEXT_RED << lm.matchedString(span)
                  << TEXT_GREEN << std::regex_replace(std::string{lm.matchedString(span)}, lm.regexp(), replaceWith_)
                  << TEXT_RESET << lm.afterMatch(span) << std::endl;
    }

private:
    std::string replaceWith_;
};

int main(int argc, char **argv)
{
    if (argc == 4) {
        std::regex re{argv[1]};
        replacer::IterateFilesystemMatches(fs::path{argv[3]}, re, ReplacePrinter{argv[2]});
    } else if (argc == 3) {
        std::regex re{argv[1]};
        replacer::IterateFilesystemMatches(fs::path{argv[2]}, re, GrepPrinter{});
    } else {
        std::cout << argv[0] << ": [search-pattern] [replace-pattern] [path]" << std::endl;
        std::cout << argv[0] << ": [search-pattern] [path]" << std::endl;
        return 0;
    }

    return 0;
}
