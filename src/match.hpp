#ifndef REPLACER_MATCH_HPP
#define REPLACER_MATCH_HPP

#include "filesystem.hpp"
#include <string>
#include <vector>

namespace replacer
{

class match
{
public:
    match() = default;
    match(const match &) = delete;
    match(match &&) = default;
    match& operator=(const match &) = delete;
    match& operator=(match &&) = delete;

    match(const fs::path &p, int line, const std::string &before, const std::string &with, long position, long length)
        : path_ {p}
        , line_ {line}
        , before_ {before}
        , with_ {with}
        , position_ {position}
        , length_ {length}
        , apply_ {false}
    {
    }

    auto prefix() const
    {
        return before_.substr(0, position_);
    }

    auto matched() const
    {
        return before_.substr(position_, length_);
    }

    auto suffix() const
    {
        return before_.substr(position_ + length_);
    }

    auto valid() const
    {
        return length_ != 0;
    }

    auto replaced() const
    {
        return with_;
    }

    auto for_apply() const
    {
        return apply_;
    }

    auto line() const { return line_; }

    void toggle_apply() { apply_ = !apply_; }

private:
    fs::path path_;
    int line_;
    std::string before_;
    std::string with_;
    long position_;
    long length_;
    bool apply_;
};

} // namespace replacer

#endif // REPLACER_MATCH_HPP
