cmake_minimum_required(VERSION 3.14...3.31)

project(fastrange
  VERSION 1.0.0
  DESCRIPTION "Fair maps to intervals without division: a fast alternative to the modulo reduction"
  HOMEPAGE_URL "https://github.com/lemire/fastrange"
  LANGUAGES NONE)

# fastrange is a single header with no compiled component, so no language is
# enabled here: a project that merely consumes the header does not need a
# working C or C++ compiler at configure time. The tests enable C and C++
# themselves.

# PROJECT_IS_TOP_LEVEL is only defined by CMake 3.21 and better.
if(NOT DEFINED PROJECT_IS_TOP_LEVEL)
  if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(PROJECT_IS_TOP_LEVEL ON)
  else()
    set(PROJECT_IS_TOP_LEVEL OFF)
  endif()
endif()

option(FASTRANGE_BUILD_TESTS "Build the fastrange tests" ${PROJECT_IS_TOP_LEVEL})
option(FASTRANGE_INSTALL "Generate the fastrange install and package-config rules" ${PROJECT_IS_TOP_LEVEL})

# We deliberately do not include(GNUInstallDirs): it emits a warning when it is
# included before any language is enabled (it cannot tell "lib" from "lib64"),
# and fastrange installs nothing that is architecture dependent. The two
# directories we do need are defined exactly as GNUInstallDirs would define
# them, so overriding them on the command line works as usual, and a parent
# project that includes GNUInstallDirs later sees the same values.
if(NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
  set(CMAKE_INSTALL_INCLUDEDIR "include"
    CACHE PATH "C header files (include)")
endif()
if(NOT DEFINED CMAKE_INSTALL_DATAROOTDIR)
  set(CMAKE_INSTALL_DATAROOTDIR "share"
    CACHE PATH "read-only architecture-independent data root (share)")
endif()

add_library(fastrange INTERFACE)
# Consumers should always use the namespaced name; it works the same whether
# fastrange was found with find_package() or added with
# add_subdirectory()/FetchContent.
add_library(fastrange::fastrange ALIAS fastrange)

target_include_directories(fastrange INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

if(FASTRANGE_BUILD_TESTS)
  include(CTest)
  add_subdirectory(tests)
endif()

if(FASTRANGE_INSTALL)
  include(CMakePackageConfigHelpers)

  # share/cmake/fastrange rather than lib/cmake/fastrange: the package is
  # architecture independent, and this path is part of the documented
  # find_package() search order on every platform.
  set(FASTRANGE_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_DATAROOTDIR}/cmake/fastrange"
    CACHE STRING "Install directory for the fastrange CMake package files")

  install(FILES "${PROJECT_SOURCE_DIR}/fastrange.h"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

  install(TARGETS fastrange EXPORT fastrangeTargets)

  install(EXPORT fastrangeTargets
    FILE fastrangeTargets.cmake
    NAMESPACE fastrange::
    DESTINATION "${FASTRANGE_CMAKE_INSTALL_DIR}")

  configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/fastrangeConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/fastrangeConfig.cmake"
    INSTALL_DESTINATION "${FASTRANGE_CMAKE_INSTALL_DIR}")

  # ARCH_INDEPENDENT: a header-only package installed by a 64-bit build is
  # still usable from a 32-bit one.
  write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/fastrangeConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT)

  install(FILES
    "${PROJECT_BINARY_DIR}/fastrangeConfig.cmake"
    "${PROJECT_BINARY_DIR}/fastrangeConfigVersion.cmake"
    DESTINATION "${FASTRANGE_CMAKE_INSTALL_DIR}")

  # pkg-config file, for consumers that do not use CMake.
  configure_file(
    "${PROJECT_SOURCE_DIR}/cmake/fastrange.pc.in"
    "${PROJECT_BINARY_DIR}/fastrange.pc"
    @ONLY)

  install(FILES "${PROJECT_BINARY_DIR}/fastrange.pc"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig")

  install(FILES "${PROJECT_SOURCE_DIR}/LICENSE"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/licenses/fastrange")
endif()
