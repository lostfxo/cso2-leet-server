cmake_minimum_required(VERSION 3.12.0)

project(LeetServerTests LANGUAGES CXX)

set(LEET_LIB_CATCH_DIR "${LEET_LIBS_DIR}/catch2")
set(LEET_LIB_CATCH_HEADER_DIR "${LEET_LIB_CATCH_DIR}/src")

set(LEET_TESTS_SOURCES "sources/clientconnection.cpp" "sources/test_serverinstance.cpp")

set(LEET_TESTS_HEADERS "headers/clientconnection.hpp" "headers/wait_util.hpp")

file(
  GLOB
  LEET_TESTS_ALL_SOURCES
  ${LEET_TESTS_SOURCES}
  ${LEET_TESTS_HEADERS}
)

source_group("Source Files" FILES ${LEET_TESTS_SOURCES})
source_group("Header Files" FILES ${LEET_TESTS_HEADERS})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

#
# setup dependencies
#
set(CATCH_INSTALL_DOCS FALSE CACHE BOOL "" FORCE)
set(CATCH_INSTALL_EXTRAS FALSE CACHE BOOL "" FORCE)
add_subdirectory(${LEET_LIB_CATCH_DIR} catch)

# threads, used by asio
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost 1.73.0 REQUIRED)

#
# Add executable to build.
#
add_executable(leet-server_tests ${LEET_TESTS_ALL_SOURCES})

#
# compiler flags
#

# enable warnings
if(MSVC)
  target_compile_options(leet-server_tests PRIVATE /W4)
else()
  target_compile_options(leet-server_tests PRIVATE -Wall -Wextra -pedantic)
endif()

# enable coroutines on gcc
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(leet-server_tests PRIVATE -fcoroutines)
endif()

#
# Setup include directories
#
target_include_directories(
  leet-server_tests
  PRIVATE "headers" "${LEET_ROOT_DIR}/headers" ${Boost_INCLUDE_DIRS}
)

#
# link libraries
#
target_link_libraries(leet-server_tests Catch2::Catch2WithMain leet-server_static)
