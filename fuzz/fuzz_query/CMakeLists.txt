#
# Copyright(c) 2006 to 2022 ZettaScale Technology and others
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License v. 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
# v. 1.0 which is available at
# http://www.eclipse.org/org/documents/edl-v10.php.
#
# SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
#
project(fuzz_query LANGUAGES C CXX)
cmake_minimum_required(VERSION 3.16)

set(CC Clang)
set(CXX Clang++)
if(NOT TARGET CycloneDDS-CXX::ddscxx)
  find_package(CycloneDDS-CXX REQUIRED)
endif()

idlcxx_generate(TARGET ddscxx_test_types_fuzz FILES
  ../../src/ddscxx/tests/data/Space.idl
  WARNINGS no-implicit-extensibility)

add_executable(fuzz_query fuzz_query.cpp)
add_link_options(fuzz_query PRIVATE -fsantitize=fuzzer,address)

# Link both executables to idl data type library and ddscxx.
target_link_libraries(fuzz_query CycloneDDS-CXX::ddscxx ddscxx_test_types_fuzz $ENV{LIB_FUZZING_ENGINE})
target_compile_options(fuzz_query PRIVATE -g -fsanitize=fuzzer,address)

# Disable the static analyzer in GCC to avoid crashing the GNU C++ compiler
# on Azure Pipelines
if(DEFINED ENV{SYSTEM_TEAMFOUNDATIONSERVERURI})
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND ANALYZER STREQUAL "on")
    target_compile_options(fuzz_query PRIVATE -fno-analyzer)
  endif()
endif()

set_property(TARGET fuzz_query PROPERTY CXX_STANDARD ${cyclonedds_cpp_std_to_use})