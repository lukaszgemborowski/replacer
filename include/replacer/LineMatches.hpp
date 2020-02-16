#ifndef LIBREPLACER_LINEMATCHES_HPP
#define LIBREPLACER_LINEMATCHES_HPP

#include <replacer/MatchSpan.hpp>
#include <replacer/NamedValue.hpp>
#include <variant>
#include <regex>
#include <string_view>

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
    auto matchedString(const MatchSpan &span) const
    {
        return std::string_view{
            line_.c_str() + span.position(),
            static_cast<std::string_view::size_type>(span.length())};
    }

private:
    LineNumber::type        lineNumber_;
    std::string             line_;
    std::vector<MatchSpan>  matches_;
};

} // namespace replacer
#endif // LIBREPLACER_LINEMATCHES_HPP