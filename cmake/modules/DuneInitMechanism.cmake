# Macros that implement the Dune init mechanism.
#
# dune_register_init_function(INCLUDE include1 [, include2...]
#                             FUNCTION function1 [, function2...])
#
# Registers a function to be called during the init
# procedure of every executable in the module and in
# all modules that depend on this module.
#
# This function needs to be called from the DuneFooMacros.cmake
# module (given a dune module dune-foo).
#
# Any registered function is expected to have the signature to
# take int, char** as arguments.

function(dune_register_init_function)
  # Parse Arguments
  set(OPTION)
  set(SINGLE)
  set(MULTI INCLUDE FUNCTION)
  include(CMakeParseArguments)
  cmake_parse_arguments(DUNE_INIT "${OPTION}" "${SINGLE}" "${MULTI}" ${ARGN})
  if(DUNE_INIT_UNPARSED_ARGUMENTS)
    message(WARNING "Unparsed arguments in dune_register_init_function: This often indicates typos!")
  endif()

  set(${CMAKE_PROJECT_NAME}_INIT TRUE PARENT_SCOPE)
  set(${CMAKE_PROJECT_NAME}_INCLUDE ${DUNE_INIT_INCLUDE} PARENT_SCOPE)
  set(${CMAKE_PROJECT_NAME}_FUNCTION ${DUNE_INIT_FUNCTION} PARENT_SCOPE)
endfunction()
