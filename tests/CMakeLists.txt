add_executable(
    tests
    main.cxx
    LineMatchesTests.cxx
    LineReplacerTests.cxx
)

target_link_libraries(
    tests
    replacer
)