add_library(
    replacer
    STATIC
    LineReplacer.cxx
    FileMatches.cxx
)

target_link_libraries(
    replacer

    # clang 6.0.1 @ FreeBSD, needs to be fixed for GCC and other platforms
    # c++experimental

    # GCC use -lstdc++fs
    stdc++fs
)

target_compile_features(
    replacer PUBLIC cxx_std_17
)

target_include_directories(
    replacer
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)