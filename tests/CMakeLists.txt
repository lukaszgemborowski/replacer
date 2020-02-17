add_executable(
    tests
    main.cxx
    LineMatchesTests.cxx
    LineReplacerTests.cxx
    FileMatchesTests.cxx
    FileMatchesIterationTests.cxx
    IterateFilesystemTests.cxx
)

target_link_libraries(
    tests
    replacer
)
