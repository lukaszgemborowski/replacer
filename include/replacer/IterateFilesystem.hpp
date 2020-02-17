#ifndef LIBREPLACER_ITERATEFILESYSTEM_HPP
#define LIBREPLACER_ITERATEFILESYSTEM_HPP

#include <replacer/fs.hpp>
#include <regex>

namespace replacer
{

template<class FsIterator, class FileMatcher, class Callback>
void IterateFilesystem(FsIterator fsi, FileMatcher matcher, std::regex &re, Callback callback)
{
    for (auto &it : fsi) {
        if (!matcher(it)) {
            continue;
        }

        callback(it);
    }
}

template<class Callback>
void IterateFilesystem(fs::path root, std::regex &re, Callback callback)
{
    IterateFilesystem(
        fs::recursive_directory_iterator{root},
        [](auto &f) { return fs::is_regular_file(f.status()); },
        re,
        callback);
}

} // namespace replacer

#endif // LIBREPLACER_ITERATEFILESYSTEM_HPP
