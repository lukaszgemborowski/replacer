cmake_minimum_required (VERSION 3.2)
project (ncurses++)

# dependencies
add_subdirectory(3rd/ncurses++)
add_subdirectory(3rd/args)

# replacer
add_subdirectory(src)
