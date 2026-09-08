# Sanitizers.cmake
#
# ctemplate_enable_sanitizers(<target>)
#
# Enables AddressSanitizer and UndefinedBehaviorSanitizer for <target> on
# GCC/Clang. Intended for local development and CI, not for release builds.
# No-op (with a warning) on MSVC, where these flags aren't available in the
# same form.

function(ctemplate_enable_sanitizers target)
  if(MSVC)
    message(WARNING "CTEMPLATE_ENABLE_SANITIZERS has no effect on MSVC; skipping for ${target}.")
    return()
  endif()

  set(sanitizer_flags -fsanitize=address,undefined -fno-omit-frame-pointer)

  target_compile_options(${target} PUBLIC ${sanitizer_flags})
  target_link_options(${target} PUBLIC -fsanitize=address,undefined)
endfunction()
