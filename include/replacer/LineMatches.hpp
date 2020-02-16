#ifndef LIBREPLACER_LINEMATCHES_HPP
#define LIBREPLACER_LINEMATCHES_HPP

#include <replacer/MatchSpan.hpp>
#include <replacer/NamedValue.hpp>
#include <variant>
#include <regex>

namespace replacer
{

using LineNumber = NamedValue<unsigned, class LineNumberTag>;

class NoMatches {};

class LineMatches
{
public:
    LineMatches(LineNumber number, const std::string &line, std::vector<MatchSpan> &&matches)
        : lineNumber_ {number}
        , line_ {line}
        , matches_ {std::move(matches)}
    {}

    using CreateResult = std::variant<NoMatches, LineMatches>;
    static CreateResult CreateMatches(LineNumber number, const std::string &line, std::regex &re)
    {
        auto m = std::sregex_iterator(line.begin(), line.end(), re);
        auto e = std::sregex_iterator();
        std::vector<MatchSpan> matches;

        for (; m != e; ++m) {
            matches.emplace_back(MatchPosition{m->position()}, MatchLength{m->length()});
        }

        if (matches.empty()) {
            return NoMatches{};
        } else {
            return LineMatches{number, line, std::move(matches)};
        }
    }

    const auto& getMatches() const { return matches_; }

private:
    unsigned                lineNumber_;
    std::string             line_;
    std::vector<MatchSpan>  matches_;
};

} // namespace replacer
#endif // LIBREPLACER_LINEMATCHES_HPP