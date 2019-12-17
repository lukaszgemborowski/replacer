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

class matches
{
public:
    template<class... Args>
    void add(Args&&... args)
    {
        matches_.emplace_back(std::forward<Args>(args)...);
    }

    template<class... Args>
    void emplace_back(Args&&... args)
    {
        matches_.emplace_back(std::forward<Args>(args)...);
    }

    auto empty() const { return matches_.empty(); }

    auto begin() { return matches_.begin(); }
    auto begin() const { return matches_.begin(); }
    auto end() { return matches_.end(); }
    auto end() const { return matches_.end(); }

private:
    std::vector<match> matches_;
};

} // namespace replacer

#endif // REPLACER_MATCH_HPP
