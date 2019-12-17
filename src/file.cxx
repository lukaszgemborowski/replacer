#include "file.hpp"

namespace replacer
{

file::file(const fs::path &p)
    : path_ {p}
{
}

file::~file()
{
    apply();
}

bool file::has_matches() const
{
    return !matches_.empty();
}

void file::fill_matches(const std::regex &re, const std::string& with)
{
    iterate([this, &re, &with](auto no, const auto &line) {
        auto m = std::sregex_iterator(line.begin(), line.end(), re);
        auto e = std::sregex_iterator();

        for (; m != e; ++m) {
            std::string w = std::regex_replace(
                line.substr(m->position(), m->length()),
                re,
                with
            );

            add_match(path_, no, line, w, m->position(), m->length());
        }
    });
}

void file::apply()
{
    auto f = load_file();

    for (const auto &m : matches_) {
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

