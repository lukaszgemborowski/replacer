add_executable(
    replacer
    main.cxx
    view_item.cxx
    file.cxx
    application.cxx
    search_results.cxx
)

target_link_libraries(
    replacer
    ncurses++
    args

    # clang 6.0.1 @ FreeBSD, needs to be fixed for GCC and other platforms
    c++experimental
)
