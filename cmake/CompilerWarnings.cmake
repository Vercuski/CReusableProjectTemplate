# CompilerWarnings.cmake
#
# ctemplate_set_warnings(<target> <as_errors>)
#
# Applies a solid baseline of compiler warnings to <target>. When
# <as_errors> is truthy, warnings are promoted to errors -- meant for local
# development and CI, kept off by default so the template builds cleanly
# out of the box on toolchains with different default warning sets.

function(ctemplate_set_warnings target as_errors)
  set(MSVC_WARNINGS
    /W4
    /permissive-
  )

  set(CLANG_GCC_WARNINGS
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wsign-conversion
    -Wcast-qual
    -Wformat=2
    -Wundef
    -Wnull-dereference
    -Wdouble-promotion
  )

  if(MSVC)
    set(project_warnings ${MSVC_WARNINGS})
  else()
    set(project_warnings ${CLANG_GCC_WARNINGS})
  endif()

  if(as_errors)
    if(MSVC)
      list(APPEND project_warnings /WX)
    else()
      list(APPEND project_warnings -Werror)
    endif()
  endif()

  target_compile_options(${target} PRIVATE ${project_warnings})
endfunction()
