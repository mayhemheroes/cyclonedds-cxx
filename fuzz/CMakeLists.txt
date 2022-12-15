#
# Copyright(c) 2020 to 2022 ZettaScale Technology and others
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License v. 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
# v. 1.0 which is available at
# http://www.eclipse.org/org/documents/edl-v10.php.
#
# SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
#
set(CMAKE_INSTALL_FUZZDIR "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/fuzz")

# this is a work-around for cross-builds failing with:
#   CMake Error at examples/helloworld/CMakeLists.txt:19 (idlcxx_generate):
#   Unknown CMake command "idlcxx_generate".
include("${CMAKE_SOURCE_DIR}/src/idlcxx/Generate.cmake")

install(
  FILES fuzz_pub_topic/fuzz_pub_topic.cpp
        fuzz_pub_topic/FuzzData.idl
        fuzz_query/fuzz_query.cpp
  DESTINATION "${CMAKE_INSTALL_FUZZDIR}/"
  COMPONENT dev)

add_subdirectory(fuzz_pub_topic)
add_subdirectory(fuzz_query)


