cmake_minimum_required(VERSION 3.1)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
# execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
#   RESULT_VARIABLE result
#   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
# if(result)
#   message(FATAL_ERROR "CMake step for googletest failed: ${result}")
# endif()
# execute_process(COMMAND ${CMAKE_COMMAND} --build .
#   RESULT_VARIABLE result
#   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
# if(result)
#   message(FATAL_ERROR "Build step for googletest failed: ${result}")
# endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-Wall -fno-exceptions -pedantic ")
# create and separate lists of necessary sources:
# - OBJ_SOURCES, MAIN, TEST_SOURCES
include_directories("src")
set(EXPERIMENT "${CMAKE_SOURCE_DIR}/src/experiment.cpp")
set(MAIN "${CMAKE_SOURCE_DIR}/src/main.cpp")
set(EXTRACEAPP "${CMAKE_SOURCE_DIR}/src/extraceapp.cpp")
set(ATRACE "${CMAKE_SOURCE_DIR}/src/atrace.cpp")
set(SYSTEMTIME_IMPL "${CMAKE_SOURCE_DIR}/src/systemtime_impl.cpp")
set(ANDROIDSYSTEM_IMPL "${CMAKE_SOURCE_DIR}/src/androidsystem_impl.cpp")
set(KERNELSYSTEM_IMPL "${CMAKE_SOURCE_DIR}/src/kernelsystem_impl.cpp")
set(ANDROIDTOOLBOX "${CMAKE_SOURCE_DIR}/src/androidtoolbox.cpp")
set(ARGSPARSERTEST "${CMAKE_SOURCE_DIR}/src/argsparser_test.cpp")

file (GLOB OBJ_SOURCES "src/*.cpp")
file (GLOB TEST_SOURCES "src/*_test.cpp")

# filter OBJ_SOURCES
list(REMOVE_ITEM OBJ_SOURCES 
     ${MAIN}
     # following need some android libraries so we exclude them
     ${ATRACE}
     ${SYSTEMTIME_IMPL}
     ${ANDROIDSYSTEM_IMPL}
     ${KERNELSYSTEM_IMPL}
     ${ANDROIDTOOLBOX}
     ${TEST_SOURCES}
     ${EXTRACEAPP}
)

list(REMOVE_ITEM TEST_SOURCES
     ${ARGSPARSERTEST}
)

# create main executable 
set (SOURCES ${OBJ_SOURCES} ${MAIN})
# add_executable(main ${SOURCES})

# add_executable(experiment ${EXPERIMENT} ${OBJ_SOURCES})

# create experimental
set (SOURCES ${OBJ_SOURCES} ${EXPERIMENT})

# create test executable
set (SOURCES ${OBJ_SOURCES} ${TEST_SOURCES})
# Now simply link against gtest or gtest_main as needed. Eg
# add_executable(main_test ${SOURCES})
# target_link_libraries(main_test gtest_main)
# target_link_libraries(main_test gmock_main)

set (PARSER_TEST_SRC "src/parser/parser_test.cpp")
add_executable(parser_test ${PARSER_TEST_SRC})
target_link_libraries(parser_test gtest_main)
target_link_libraries(parser_test gmock_main)

set (ENTRYFILE_TEST "src/parser/entryfile_test.cpp")
add_executable(entryfile_test ${ENTRYFILE_TEST})
target_link_libraries(entryfile_test gtest_main)
target_link_libraries(entryfile_test gmock_main)

set (SCHEDSWITCH_TEST "src/parser/schedswitchentrycreator_test.cpp")
add_executable(schedswitch_test ${SCHEDSWITCH_TEST})
target_link_libraries(schedswitch_test gtest_main)
target_link_libraries(schedswitch_test gmock_main)