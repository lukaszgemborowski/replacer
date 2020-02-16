#ifndef LIBREPLACER_MATCHSPAN_HPP
#define LIBREPLACER_MATCHSPAN_HPP

#include <replacer/NamedValue.hpp>

namespace replacer
{

using MatchPosition = NamedValue<long int, class MatchPositionTag>;
using MatchLength = NamedValue<long int, class MatchLengthTag>;

class MatchSpan
{
public:
    MatchSpan(MatchPosition position, MatchLength length)
        : position_ {position}
        , length_ {length}
    {}

    auto position() const { return position_; }
    auto length() const { return length_; }

private:
    MatchPosition::type position_;
    MatchLength::type   length_;
};

} // namespace replacer

#endif // LIBREPLACER_MATCHSPAN_HPP