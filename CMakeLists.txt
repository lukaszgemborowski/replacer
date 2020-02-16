cmake_minimum_required (VERSION 3.2)
project (replacer)

# dependencies
add_subdirectory(3rd/ncursesplusplus)
add_subdirectory(3rd/args)

# replacer
add_subdirectory(src)
