#ifndef REPLACER_FILE_HPP
#define REPLACER_FILE_HPP

#include "match.hpp"
#include "filesystem.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <regex>

namespace replacer
{

struct file
{
    file(const fs::path &p);
    ~file();

    file() = delete;
    file(const file &) = delete;
    file(file &&) = delete;
    file& operator=(const file&) = delete;
    file& operator=(file&&) = delete;

    template<class... Args>
    void add_match(Args&&... args)
    {
        matches_.add(std::forward<Args>(args)...);
    }

    bool has_matches() const;
    auto& get_matches() { return matches_; }
    const auto& path() const { return path_; }

    template<class Fun>
    void iterate(Fun f) const
    {
        std::ifstream is{path_};
        std::string temp;
        int line = 0;

        while (std::getline(is, temp)) {
            f (line, temp);
            line ++;
        }
    }


private:
    void apply();
    std::vector<std::string> load_file();
    void save(const std::vector<std::string> &l);

private:
    fs::path path_;
    matches matches_;
};

} // namespace replacer

#endif // REPLACER_FILE_HPP
