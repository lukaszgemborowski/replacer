#include <replacer/IterateFilesystem.hpp>
#include <iostream>

void print_entry(const fs::path &path, const replacer::LineMatches &lm, replacer::MatchSpan span)
{
    std::cout << path << ":" << lm.lineNumber() << ": "
        << lm.beforeMatch(span)
        << "\x1b[32m" << lm.matchedString(span) << "\x1b[0m"
        << lm.afterMatch(span) << std::endl;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cout << argv[0] << ": [pattern] [path]" << std::endl;
        return 0;
    }

    std::regex re{argv[1]};
    IterateFilesystemMatches(fs::path{argv[2]}, re, print_entry);

    return 0;
}
