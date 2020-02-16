#ifndef LIBREPLACER_FILEMATCHES_HPP
#define LIBREPLACER_FILEMATCHES_HPP

#include <replacer/LineMatches.hpp>
#include <replacer/fs.hpp>
#include <regex>
#include <set>

namespace replacer
{

namespace detail
{

struct LineCompare
{
    bool operator()(const LineMatches &l1, const LineMatches &l2) {
        return *l1.lineNumber() < *l2.lineNumber();
    }
};

using LineMatchesSet = std::set<LineMatches, detail::LineCompare>;

} // namespace detail

class FileMatches
{
public:
    static FileMatches Create(fs::path filename, std::regex &re);
    static FileMatches Create(fs::path filename, std::regex &re, std::istream &is);

    std::size_t size() const;

private:
    FileMatches(fs::path filename, detail::LineMatchesSet &&matchesSet);

private:
    fs::path filename_;
    detail::LineMatchesSet matches_;
};

} // namespace replacer
#endif // LIBREPLACER_FILEMATCHES_HPP