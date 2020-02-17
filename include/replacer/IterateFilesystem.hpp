#ifndef LIBREPLACER_ITERATEFILESYSTEM_HPP
#define LIBREPLACER_ITERATEFILESYSTEM_HPP

#include <replacer/fs.hpp>
#include <replacer/FileMatches.hpp>
#include <replacer/Utils.hpp>
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

template<class StreamOpener, class FsIterator, class FileMatcher, class Callback>
void IterateFilesystemMatches(StreamOpener opener, FsIterator fsi, FileMatcher matcher, std::regex &re, Callback callback)
{
    IterateFilesystem(
        fsi,
        matcher,
        re,
        [&](auto &entry) {
            auto stream = opener(entry.path());
            auto fm = FileMatches::Create(entry.path(), re, stream);

            for (auto &lineMatches : fm) {
                for (auto &match : lineMatches) {
                    callback(entry.path(), lineMatches, match);
                }
            }
        }
    );
}

template<class Callback>
void IterateFilesystemMatches(fs::path root, std::regex &re, Callback callback)
{
    IterateFilesystemMatches(
        utils::OpenFileStream{},
        fs::recursive_directory_iterator{root},
        [](auto &f) { return fs::is_regular_file(f.status()); },
        re,
        callback
    );
}

} // namespace replacer

#endif // LIBREPLACER_ITERATEFILESYSTEM_HPP
