cmake_minimum_required(VERSION 3.1)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-Wall -Werror -fno-exceptions -pedantic -Wunused-parameter -fpermissive")
# create and separate lists of necessary sources:
# - OBJ_SOURCES, MAIN, TEST_SOURCES
include_directories("src")
set(MAIN "${CMAKE_SOURCE_DIR}/src/main.cpp")
file (GLOB OBJ_SOURCES "src/*.cpp")
file (GLOB TEST_SOURCES "src/*_test.cpp")

# filter OBJ_SOURCES
list(REMOVE_ITEM OBJ_SOURCES ${MAIN} ${TEST_SOURCES})

# create main executable 
set (SOURCES ${OBJ_SOURCES} ${MAIN})
add_executable(main ${SOURCES})

# create test executable
set (SOURCES ${OBJ_SOURCES} ${TEST_SOURCES})
# Now simply link against gtest or gtest_main as needed. Eg
add_executable(main_test ${SOURCES})
target_link_libraries(main_test gtest_main)
target_link_libraries(main_test gmock_main)
