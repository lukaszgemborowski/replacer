#include "file.hpp"

namespace replacer
{

file::file(const fs::path &p)
    : path_ {p}
{
}

bool file::has_matches() const
{
    return !matches_.empty();
}

void file::apply(matches &all)
{
    auto f = load_file();

    for (const auto &m : all) {
        if (!m.for_apply())
            continue;

        f[m.line()] = m.prefix() + m.replaced() + m.suffix();
    }

    save(f);
}

std::vector<std::string> file::load_file()
{
    std::vector<std::string> res;
    iterate([&res](auto, auto &l) {
                res.push_back(l);
            });
    return res;
}

void file::save(const std::vector<std::string> &l)
{
    std::ofstream of {path_};
    for (auto i = 0u; i < l.size(); ++i) {
        of << l[i];

        if (i != l.size() - 1)
            of << '\n';
    }
}

} // namespace replacer

