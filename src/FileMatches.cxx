#include <replacer/FileMatches.hpp>
#include <fstream>

namespace replacer
{

FileMatches::FileMatches(fs::path filename, detail::LineMatchesSet &&matchesSet)
    : filename_ {filename}
    , matches_ {std::move(matchesSet)}
{}

FileMatches FileMatches::Create(fs::path filename, std::regex &re)
{
    std::ifstream ifs{filename};
    return Create(filename, re, ifs);
}

FileMatches FileMatches::Create(fs::path filename, std::regex &re, std::istream &is)
{
    LineNumber::type lineNumber = 0;
    std::string line;
    detail::LineMatchesSet matches;

    while (std::getline(is, line)) {
        auto lineMatches = LineMatches::CreateMatches(LineNumber{lineNumber}, line, re);

        if (std::holds_alternative<LineMatches>(lineMatches)) {
            matches.emplace(std::get<LineMatches>(lineMatches));
        }

        lineNumber ++;
    }

    return FileMatches{filename, std::move(matches)};
}

std::size_t FileMatches::size() const
{
    return matches_.size();
}

} // namespace replacer