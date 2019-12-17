#ifndef REPLACER_APPLICATION_HPP
#define REPLACER_APPLICATION_HPP

#include "filesystem.hpp"
#include "view_item.hpp"
#include <ncurses++/session.hpp>
#include <ncurses++/item_list.hpp>
#include <regex>
#include <map>

namespace replacer
{

class application
{
public:
    application(ncursespp::session &ses, const fs::path &path, const std::regex &re, const std::string &with);

    // delete copy and move
    application() = delete;
    application(const application &) = delete;
    application(application &&) = delete;
    application& operator=(const application &) = delete;
    application& operator=(application &&) = delete;

    int run();
    void match_in_file(const fs::path &p);

private:
    using ItemList = ncursespp::item_list<view_item>;
    using FileMap = std::map<fs::path, file>;

    ncursespp::session& ses_;
    const fs::path      path_;
    const std::regex    &re_;
    const std::string&  with_;
    ItemList            list_;
    FileMap             files_;
};

} // namespace replacer

#endif // REPLACER_APPLICATION_HPP
