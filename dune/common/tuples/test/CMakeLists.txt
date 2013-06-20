set(TESTPROGS
    test-densevectortuple
    test-indextuple
    test-pointertuple
    test-subtuple)

add_directory_test_target(_test_target)
add_dependencies(${_test_target} ${TESTPROGS})

add_executable("test-densevectortuple" test-densevectortuple.cc)
target_link_libraries(test-densevectortuple "dunecommon")

add_executable("test-indextuple" test-indextuple.cc)
target_link_libraries(test-indextuple "dunecommon")

add_executable("test-pointertuple" test-pointertuple.cc)
target_link_libraries(test-pointertuple "dunecommon")

add_executable("test-subtuple" test-subtuple.cc)
target_link_libraries(test-subtuple "dunecommon")

add_test(test-densevectortuple test-densevectortuple)
add_test(test-pointertuple     test-pointertuple)
add_test(test-subtuple         test-subtuple)
