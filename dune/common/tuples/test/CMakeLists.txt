set(TESTPROGS
    test-densevectortuple
    test-pointertuple
    test-subtuple)

set(SOURCES printtuple.hh)

add_directory_test_target(_test_target)
add_dependencies(${_test_target} ${TESTPROGS})

add_executable("test-densevectortuple" test-densevectortuple.cc)

add_executable("test-pointertuple" test-pointertuple.cc)

add_executable("test-subtuple" test-subtuple.cc)

add_test(test-densevectortuple test-densevectortuple)
add_test(test-pointertuple     test-pointertuple)
add_test(test-subtuple         test-subtuple)
