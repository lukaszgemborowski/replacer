#ifndef LIBREPLACER_UTILS_HPP
#define LIBREPLACER_UTILS_HPP

#include <fstream>
#include <variant>
#include <replacer/fs.hpp>

namespace replacer::utils
{

struct OpenFileStream
{
    std::ifstream operator()(fs::path &path) const
    {
        return std::ifstream {path};
    }
};

} // namespace replacer::utils

#endif // LIBREPLACER_UTILS_HPP