add_executable(
    tests
    main.cxx
    LineMatchesTests.cxx
)

target_link_libraries(
    tests
    replacer
)