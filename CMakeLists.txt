cmake_minimum_required(VERSION 3.15)

project(
  ctemplate
  VERSION 0.1.0
  DESCRIPTION "A reusable, maintainable C project template"
  LANGUAGES C
)

# ---------------------------------------------------------------------------
# Project-wide settings
# ---------------------------------------------------------------------------

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Detect whether this project is being built standalone or pulled in via
# add_subdirectory()/FetchContent by another project. CMake >= 3.21 exposes
# this as PROJECT_IS_TOP_LEVEL automatically; we compute it manually so the
# template keeps working back to 3.15.
if(NOT DEFINED CTEMPLATE_IS_TOP_LEVEL)
  if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(CTEMPLATE_IS_TOP_LEVEL ON)
  else()
    set(CTEMPLATE_IS_TOP_LEVEL OFF)
  endif()
endif()

include(GNUInstallDirs)
include(CTest)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerWarnings)
include(Sanitizers)
include(StaticAnalysis)

# ---------------------------------------------------------------------------
# User-facing options
# ---------------------------------------------------------------------------

option(BUILD_SHARED_LIBS            "Build ctemplate as a shared library" OFF)
option(CTEMPLATE_BUILD_TESTS        "Build the test suite"                ${CTEMPLATE_IS_TOP_LEVEL})
option(CTEMPLATE_BUILD_EXAMPLES     "Build the example programs"          ${CTEMPLATE_IS_TOP_LEVEL})
option(CTEMPLATE_BUILD_DOCS         "Build Doxygen documentation"         OFF)
option(CTEMPLATE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors"   OFF)
option(CTEMPLATE_ENABLE_SANITIZERS  "Enable ASan/UBSan (Debug, GCC/Clang)" OFF)
option(CTEMPLATE_ENABLE_CLANG_TIDY  "Run clang-tidy as part of the build" OFF)
option(CTEMPLATE_INSTALL            "Generate install and packaging rules" ${CTEMPLATE_IS_TOP_LEVEL})

if(CTEMPLATE_ENABLE_CLANG_TIDY)
  ctemplate_enable_clang_tidy()
endif()

# ---------------------------------------------------------------------------
# Library target
# ---------------------------------------------------------------------------

add_library(ctemplate
  src/ctemplate.c
)
add_library(ctemplate::ctemplate ALIAS ctemplate)

set_target_properties(ctemplate PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  C_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN ON
)

include(GenerateExportHeader)
generate_export_header(ctemplate
  BASE_NAME CTEMPLATE
  EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/ctemplate/export.h"
)

target_include_directories(ctemplate
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(ctemplate PUBLIC c_std_11)

ctemplate_set_warnings(ctemplate ${CTEMPLATE_WARNINGS_AS_ERRORS})

if(CTEMPLATE_ENABLE_SANITIZERS)
  ctemplate_enable_sanitizers(ctemplate)
endif()

# ---------------------------------------------------------------------------
# Subdirectories
# ---------------------------------------------------------------------------

if(CTEMPLATE_BUILD_TESTS AND BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

if(CTEMPLATE_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(CTEMPLATE_BUILD_DOCS)
  add_subdirectory(docs)
endif()

# ---------------------------------------------------------------------------
# Install / packaging (only meaningful when this is the top-level project)
# ---------------------------------------------------------------------------

if(CTEMPLATE_INSTALL)
  include(CMakePackageConfigHelpers)

  install(
    TARGETS ctemplate
    EXPORT ctemplateTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(DIRECTORY include/ctemplate DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/ctemplate/export.h"
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ctemplate)

  install(
    EXPORT ctemplateTargets
    FILE ctemplateTargets.cmake
    NAMESPACE ctemplate::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ctemplate
  )

  configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ctemplateConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ctemplateConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ctemplate
  )

  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ctemplateConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
  )

  install(
    FILES
      "${CMAKE_CURRENT_BINARY_DIR}/ctemplateConfig.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/ctemplateConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ctemplate
  )

  export(
    EXPORT ctemplateTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/ctemplateTargets.cmake"
    NAMESPACE ctemplate::
  )
endif()
