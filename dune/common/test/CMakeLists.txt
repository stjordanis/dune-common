# tests that should build and run successfully
set(TESTS
    arraylisttest
    arraytest
    bigunsignedinttest
    bitsetvectortest
    calloncetest
    check_fvector_size
    conversiontest
    diagonalmatrixtest
    dynmatrixtest
    dynvectortest
    eigenvaluestest
    enumsettest
    fmatrixtest
    fvectortest
    gcdlcmtest
    integersequence
    iteratorfacadetest
    iteratorfacadetest2
    lrutest
    mpicollectivecommunication
    mpiguardtest
    mpihelpertest
    mpihelpertest2
    nullptr_test
    pathtest
    parametertreelocaletest
    parametertreetest
    poolallocatortest
    shared_ptrtest_config
    singletontest
    streamtest
    testfloatcmp
    tuplestest_config
    tupleutilitytest
    utilitytest)

#test that should build but fail to run successfully
set(FAILTESTS
    testdebugallocator_fail1
    testdebugallocator_fail2
    testdebugallocator_fail3
    testdebugallocator_fail4
    testdebugallocator_fail5
   )

set(COMPILEFAILTESTS
    check_fvector_size_fail1
    check_fvector_size_fail2
    genericiterator_compile_fail
    nullptr-test-fail)

set(TESTPROGS ${TESTS} ${FAILTESTS})

# We do not want want to build the tests during make all,
# but just build them on demand
add_directory_test_target(_test_target)
add_dependencies(${_test_target} ${TESTPROGS})

# Add the executables needed for the tests
add_executable("arraylisttest" arraylisttest.cc)
add_executable("arraytest" arraytest.cc)

add_executable("bigunsignedinttest" bigunsignedinttest.cc)
target_link_libraries("bigunsignedinttest" "dunecommon")

add_executable("bitsetvectortest" bitsetvectortest.cc)
add_executable("calloncetest" calloncetest.cc)
add_executable("check_fvector_size" check_fvector_size.cc)
add_executable("check_fvector_size_fail1" EXCLUDE_FROM_ALL check_fvector_size_fail.cc)
set_target_properties(check_fvector_size_fail1 PROPERTIES COMPILE_FLAGS "-DDIM=1")
add_executable("check_fvector_size_fail2" EXCLUDE_FROM_ALL check_fvector_size_fail.cc)
set_target_properties(check_fvector_size_fail2 PROPERTIES COMPILE_FLAGS "-DDIM=3")
add_executable("conversiontest" conversiontest.cc)

add_executable("dynmatrixtest" dynmatrixtest.cc)

add_executable("dynvectortest" dynvectortest.cc)

add_executable("eigenvaluestest" eigenvaluestest.cc)

add_executable("diagonalmatrixtest" diagonalmatrixtest.cc)

add_executable("enumsettest" enumsettest.cc)

add_executable("testfloatcmp" testfloatcmp.cc)

# we provide an empty fortran file to force the linker
# to link to the fortran libraries (needed for static linking)
add_executable("fmatrixtest" fmatrixtest.cc dummy.f)
add_executable("fvectortest" fvectortest.cc)
add_executable("gcdlcmtest" gcdlcmtest.cc)
add_executable("genericiterator_compile_fail" EXCLUDE_FROM_ALL genericiterator_compile_fail.cc)
add_executable("integersequence" integersequence.cc)
add_executable("iteratorfacadetest2" iteratorfacadetest2.cc)
add_executable("iteratorfacadetest" iteratorfacadetest.cc)
add_executable("lrutest" lrutest.cc)
add_executable("mpiguardtest" mpiguardtest.cc)

add_executable("mpicollectivecommunication" mpicollectivecommunication.cc)

add_executable("mpihelpertest" mpihelpertest.cc)

add_executable("mpihelpertest2" mpihelpertest.cc)
set_target_properties(mpihelpertest2 PROPERTIES COMPILE_FLAGS "-DMPIHELPER_PREINITIALIZE")

add_executable("nullptr_test" nullptr-test.cc nullptr-test2.cc)
add_executable("nullptr_test_fail" EXCLUDE_FROM_ALL nullptr-test.cc)
set_target_properties(nullptr_test_fail PROPERTIES COMPILE_FLAGS "-DFAIL")

add_executable("parametertreelocaletest" parametertreelocaletest.cc)

add_executable("parametertreetest" parametertreetest.cc)

add_executable("pathtest" pathtest.cc)

add_executable("poolallocatortest" poolallocatortest.cc)
add_executable("shared_ptrtest_config" shared_ptrtest.cc)
add_executable("singletontest" singletontest.cc)
add_executable("sllisttest" EXCLUDE_FROM_ALL sllisttest.cc)

add_executable("streamtest" streamtest.cc)

add_executable("testdebugallocator" testdebugallocator.cc)
add_executable("testdebugallocator_fail1" testdebugallocator.cc)
set_target_properties(testdebugallocator_fail1 PROPERTIES COMPILE_DEFINITIONS "FAILURE1;EXPECTED_SIGNAL=SIGSEGV")
add_executable("testdebugallocator_fail2" testdebugallocator.cc)
set_target_properties(testdebugallocator_fail2 PROPERTIES COMPILE_DEFINITIONS "FAILURE2;EXPECTED_SIGNAL=SIGABRT")
add_executable("testdebugallocator_fail3" testdebugallocator.cc)
set_target_properties(testdebugallocator_fail3 PROPERTIES COMPILE_DEFINITIONS "FAILURE3;EXPECTED_SIGNAL=SIGABRT")
add_executable("testdebugallocator_fail4" testdebugallocator.cc)
set_target_properties(testdebugallocator_fail4 PROPERTIES COMPILE_DEFINITIONS "FAILURE4;DEBUG_ALLOCATOR_KEEP=1;EXPECTED_SIGNAL=SIGSEGV")
add_executable("testdebugallocator_fail5" testdebugallocator.cc)
set_target_properties(testdebugallocator_fail5 PROPERTIES COMPILE_DEFINITIONS "FAILURE5;EXPECTED_SIGNAL=SIGSEGV")

add_executable("tuplestest_config" tuplestest.cc)
add_executable("tupleutilitytest" tupleutilitytest.cc)
add_executable("utilitytest" utilitytest.cc)

# Add the tests to be executed
foreach(_TEST ${TESTPROGS})
  add_test(${_TEST} ${_TEST})
endforeach(_TEST)

# treat tests returning code 77 as skipped
set_tests_properties(
  eigenvaluestest
  mpicollectivecommunication
  PROPERTIES SKIP_RETURN_CODE 77)

# set properties for failing tests
set_tests_properties(
  ${FAILTESTS}
  PROPERTIES WILL_FAIL true)

# compile tests that should fail
foreach(_TEST ${COMPILEFAILTESTS})
  add_test(NAME ${_TEST}
  COMMAND ${CMAKE_COMMAND} --build . --target ${_TEST} --config $<CONFIGURATION>)
endforeach(_TEST)
set_tests_properties(
  ${COMPILEFAILTESTS}
  PROPERTIES WILL_FAIL true)
