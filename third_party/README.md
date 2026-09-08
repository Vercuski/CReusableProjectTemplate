# third_party/

This directory is where you vendor external C dependencies that don't ship
a CMake package config of their own (a single-header library, a small
library you build from source alongside this project, etc.).

Two common patterns:

1. **Vendor the source** directly into a subdirectory here and add it with
   `add_subdirectory(third_party/<name>)` from the top-level `CMakeLists.txt`.

2. **Fetch it at configure time** with `FetchContent` instead of committing
   the source:

   ```cmake
   include(FetchContent)
   FetchContent_Declare(
     somelib
     GIT_REPOSITORY https://example.com/somelib.git
     GIT_TAG        v1.2.3
   )
   FetchContent_MakeAvailable(somelib)
   ```

Prefer a real package manager (vcpkg, Conan) or `find_package()` for
anything with a proper release process; reserve this folder for the rare
dependency that needs to be vendored directly.
