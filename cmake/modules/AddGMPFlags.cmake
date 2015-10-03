# add_dune_gmp_flags(target1 target2 ...)
# Function that adds GMP flags to the specified targets
#
function(add_dune_gmp_flags _targets)
  if(GMP_FOUND)
    foreach(_target ${_targets})
      target_link_libraries(${_target} ${GMP_LIBRARIES})
      foreach(_path ${GMP_INCLUDE_DIRS})
        set_property(TARGET ${_target}
          APPEND_STRING
          PROPERTY COMPILE_FLAGS "-I${_path}")
      endforeach(_path ${GMP_INCLUDE_DIRS})
    endforeach(_target ${_targets})
  endif(GMP_FOUND)
endfunction(add_dune_gmp_flags)
