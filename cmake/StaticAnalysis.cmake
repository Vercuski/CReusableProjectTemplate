# StaticAnalysis.cmake
#
# ctemplate_enable_clang_tidy()
#
# Wires clang-tidy into the build by setting CMAKE_C_CLANG_TIDY, so every
# target compiled afterwards in this directory scope is checked. Call this
# before adding targets. Silently (but visibly) does nothing if clang-tidy
# isn't installed, so CI/dev machines without it don't break the build.

function(ctemplate_enable_clang_tidy)
  find_program(CTEMPLATE_CLANG_TIDY_EXE NAMES "clang-tidy")

  if(NOT CTEMPLATE_CLANG_TIDY_EXE)
    message(WARNING "CTEMPLATE_ENABLE_CLANG_TIDY is ON but clang-tidy was not found; skipping.")
    return()
  endif()

  set(CMAKE_C_CLANG_TIDY "${CTEMPLATE_CLANG_TIDY_EXE}" PARENT_SCOPE)
  message(STATUS "clang-tidy enabled: ${CTEMPLATE_CLANG_TIDY_EXE}")
endfunction()
