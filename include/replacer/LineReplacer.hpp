#ifndef LIBREPLACER_LINEREPLACER_HPP
#define LIBREPLACER_LINEREPLACER_HPP

#include <replacer/LineMatches.hpp>

namespace replacer
{
class LineReplacer
{
public:
    explicit LineReplacer(LineMatches &matches);
    std::string replace(const std::string &with, std::vector<std::size_t> &&indices) const;

private:
    LineMatches &lineMatches_;
};

} // namespace replacer
#endif // LIBREPLACER_LINEREPLACER_HPP