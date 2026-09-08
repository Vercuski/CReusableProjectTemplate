# Changelog

All notable changes to this project are documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project intends to follow [Semantic Versioning](https://semver.org/).

## [Unreleased]

## [0.1.0] - 2026-09-08

### Added

- Initial project template: CMake build system with install/export support,
  compiler warning flags, optional sanitizers, and optional clang-tidy
  integration.
- Example `ctemplate` library (a small growable string buffer) demonstrating
  the template's conventions: opaque types, create/destroy ownership,
  enum status codes, const-correctness.
- Test suite wired into CTest via a minimal, dependency-free test header.
- GitHub Actions CI: multi-OS build and test matrix, a sanitizer job, and a
  static-analysis job (clang-format check + clang-tidy).
- `.clang-format`, `.clang-tidy`, `.editorconfig`, `.gitattributes`.
