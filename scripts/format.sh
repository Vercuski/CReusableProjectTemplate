#!/usr/bin/env bash
# Format all C sources in the project with clang-format, in place.
set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")/.."

find include src tests examples \( -name '*.c' -o -name '*.h' \) -print0 \
  | xargs -0 clang-format -i

echo "Formatted all .c/.h files under include/, src/, tests/, examples/."
