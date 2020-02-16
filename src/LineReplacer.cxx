#include <replacer/LineReplacer.hpp>

namespace replacer
{
LineReplacer::LineReplacer(LineMatches &matches)
    : lineMatches_ {matches}
{}

std::string LineReplacer::replace(const std::string &with, std::vector<std::size_t> &&indices) const
{
    int offset = 0;
    auto matches = lineMatches_.getMatches();
    auto line = lineMatches_.line();
    std::sort(indices.begin(), indices.end());

    for (auto idx : indices) {
        auto match = matches[idx];
        auto substr = line.substr(match.position() + offset, match.length());
        auto replaced = std::regex_replace(
            substr,
            lineMatches_.regexp(),
            with
        );

        line.replace(match.position() + offset, match.length(), replaced);
        offset += (replaced.size() - substr.size());
    }

    return line;
}

} // namespace replacer