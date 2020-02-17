add_executable(
    tests
    main.cxx
    LineMatchesTests.cxx
    LineReplacerTests.cxx
    FileMatchesTests.cxx
    FileMatchesIterationTests.cxx
    IterateFilesystemTests.cxx

    # sample use-cases tests
    UC_IterateFilesystemAndPrintMatches.cxx
)

target_link_libraries(
    tests
    replacer
)
