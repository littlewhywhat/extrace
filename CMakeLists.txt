cmake_minimum_required(VERSION 3.1)

# Download and unpack googletest at configure time
# configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
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

include_directories("src")

list(APPEND OBJ_SOURCES 
    "src/ftraceentry"
    "src/processchange.cpp"
    "src/schedswitchentrycreator.cpp"
    "src/schedwakeupentrycreator.cpp"
    "src/simpleftraceentrybynamecreator.cpp"
    "src/simpleftraceentryfile.cpp"
    "src/simpleprocesschangefile.cpp"
    "src/simpleprocess.cpp"
    "src/simpleprocessrecordfile.cpp"
    "src/tracingmarkentrycreator.cpp"
    "src/signal.cpp"
    "src/cmdlineapp.cpp"
    "src/environment.cpp"
    "src/listsupportedcategories.cpp"
    "src/addandroidcoretotrace.cpp"
    "src/sleepaction.cpp"
    "src/startaction.cpp"
    "src/streamaction.cpp"
    "src/cleanupaction.cpp"
    "src/stopaction.cpp"
    "src/dumpaction.cpp"
    "src/showhelpaction.cpp"
    "src/interpretdumpfileaction.cpp"
    "src/actionsrunner.cpp"
    "src/extracearguments.cpp"
    "src/arguments.cpp"
    "src/cmdlineargsparser.cpp"
    "src/cmdlineargs.cpp"
    "src/wire.cpp"
    "src/filesystem_impl.cpp"
    "src/trace_impl.cpp"
    "src/ftrace.cpp"
    "src/ftrace_test.cpp"
    "src/kerneltracesystem.cpp"
    "src/simplefiledata.cpp"
    "src/filedataslice.cpp"
    "src/simplefiledatamaker.cpp"
    "src/kerneltracecategory.cpp"
    "src/androidtracecategory.cpp"
    "src/arguments_test.cpp"
    "src/schedswitchentrycreator_test.cpp"
    "src/schedwakeupentrycreator_test.cpp"
    "src/extrace_test.cpp"
    "src/processrecordfile_test.cpp"
    "src/simpleprocesschangefile_test.cpp"
    "src/processrecord_test.cpp"
    "src/memoryentrycreator_test.cpp"
    "src/simpleftraceentryfile_test.cpp"
    "src/emptyentrycreator.cpp"
    "src/androidentrycreator.cpp"
    "src/memoryentrycreator.cpp"
    "src/cpuguard.cpp"
    "src/cpuguard_test.cpp"
    "src/rssguard.cpp"
    "src/rssguard_test.cpp"
    ### String8, ... google libraries
    # "src/androidtoolbox.cpp"
    ### android toolbox dependencies
    # "src/cmdlineargsparser_test.cpp"
    # "src/extraceargumentsbuilder.cpp"
    # "src/simpleandroidtracesystem.cpp"
    # "src/simpleandroidtracesystem_test.cpp"
    ### systemtime library
    # "src/systemtime_impl.cpp"
    ### systemtime dependencies
    # "src/ftracebufferfile.cpp"
    ### android Binder, properties
    # "src/android_impl.cpp"
    ### android_impl dependencies
    # "src/environmentbuilder.cpp"
    # "src/extraceactioncmdlinebuilder.cpp"
    # "src/extraceactionsrunnerbuilder.cpp"
    ### libz
    # "src/compressedfiledata.cpp"
    ### compressedfiledata dependencies
    # "src/compressedfiledatamaker.cpp"
    ### pagemap google library
    # "src/pm_kernelbuilder.cpp"
    # "src/pm_kernel.cpp"
    # "src/pm_process.cpp"
    # "src/pm_abstractmemoryusage.cpp"
    # "src/pm_memoryusage.cpp"
    # "src/pm_workingset.cpp"
    # "src/memorysampleaction.cpp"
)

add_executable(main_test ${OBJ_SOURCES})
target_link_libraries(main_test gtest_main)
target_link_libraries(main_test gmock_main)
