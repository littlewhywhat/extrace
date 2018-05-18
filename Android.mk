processrecordfile_src_files := \
		src/ftraceentry.cpp                    \
		src/processchange.cpp                  \
		src/schedswitchentrycreator.cpp        \
		src/schedwakeupentrycreator.cpp        \
		src/simpleftraceentrybynamecreator.cpp \
		src/simpleftraceentryfile.cpp          \
		src/simpleprocesschangefile.cpp        \
		src/simpleprocess.cpp                  \
		src/simpleprocessrecordfile.cpp        \
		src/memoryentrycreator.cpp        \
		src/androidentrycreator.cpp       \
		src/tracingmarkentrycreator.cpp   \
		src/emptyentrycreator.cpp \

common_src_files :=  \
		src/signal.cpp   \
		src/cmdlineapp.cpp \
		src/environment.cpp \
		src/listsupportedcategories.cpp \
		src/addandroidcoretotrace.cpp \
		src/sleepaction.cpp \
		src/startaction.cpp \
		src/streamaction.cpp \
		src/cleanupaction.cpp \
		src/stopaction.cpp \
		src/dumpaction.cpp \
		src/showhelpaction.cpp \
		src/interpretdumpfileaction.cpp \
		src/actionsrunner.cpp \
		src/extracearguments.cpp \
		src/extraceargumentsbuilder.cpp \
	  src/androidtoolbox.cpp \
		src/arguments.cpp \
		src/cmdlineargsparser.cpp \
		src/cmdlineargs.cpp \
		src/wire.cpp \
		src/systemtime_impl.cpp \
		src/filesystem_impl.cpp \
		src/trace_impl.cpp \
		src/simpleftrace.cpp \
		src/kerneltracesystem.cpp \
		src/simplefiledata.cpp \
		src/filedataslice.cpp \
		src/ftracebufferfile.cpp \
		src/simplefiledatamaker.cpp \
		src/simpleandroidtracesystem.cpp \
		src/kerneltracecategory.cpp \
		src/androidtracecategory.cpp \
		src/cpuguard.cpp \
    src/rssguard.cpp \

libbinder_files := \
		src/android_impl.cpp \
		src/environmentbuilder.cpp \
		src/extraceactionsrunnerbuilder.cpp \
		src/extraceactioncmdlinebuilder.cpp \

libz_files := \
    src/compressedfiledata.cpp \
		src/compressedfiledatamaker.cpp \

pagemap_files := \
		src/pm_kernelbuilder.cpp \
		src/pm_kernel.cpp \
		src/pm_process.cpp \
		src/pm_abstractmemoryusage.cpp \
		src/pm_memoryusage.cpp \
		src/pm_workingset.cpp \
		src/memorysampleaction.cpp \

common_test_files := \
		src/arguments_test.cpp          \
		src/schedswitchentrycreator_test.cpp \
    src/cmdlineargsparser_test.cpp  \
    src/schedwakeupentrycreator_test.cpp \
    src/extrace_test.cpp            \
    src/processrecordfile_test.cpp  \
    src/simpleprocesschangefile_test.cpp \
    src/processrecord_test.cpp      \
    src/memoryentrycreator_test.cpp \
    src/androidentrycreator_test.cpp \
    "src/cpuguard_test.cpp" \
    "src/rssguard_test.cpp" \

local_test_files := \
    src/simpleftraceentryfile_test.cpp \

common_cflags := \
    -Wall \
    -std=c++11 \
    -pedantic \

LOCAL_PATH:= $(call my-dir)

# extrace
# ==========

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	  $(libbinder_files) \
	  $(libz_files) \
	  $(common_src_files) \
	  $(processrecordfile_src_files) \
	  $(pagemap_files) \
	  src/extrace.cpp \

LOCAL_MODULE:= extrace

LOCAL_CPPFLAGS := $(common_cflags)

LOCAL_C_INCLUDES += external/zlib

LOCAL_MODULE_TAGS:= optional

LOCAL_SHARED_LIBRARIES := \
    libbinder \
    libcutils \
    libutils \
    libz \
    libpagemap \

include $(BUILD_EXECUTABLE)

# extrace_test target
# ===================

include $(CLEAR_VARS)

LOCAL_MODULE := extrace_test
LOCAL_MODULE_TAGS := tests

LOCAL_CPPFLAGS := $(common_cflags)

LOCAL_SRC_FILES := \
	  $(libbinder_files) \
		$(libz_files) \
		$(common_src_files) \
	  $(processrecordfile_src_files) \
    ${common_test_files} \
		src/extrace_test.cpp \

LOCAL_STATIC_LIBRARIES := \
    libgmock \

LOCAL_SHARED_LIBRARIES := \
    libbinder \
    libcutils \
    libutils \
    libz \

include $(BUILD_NATIVE_TEST)

# extrace_test host
# =================

include $(CLEAR_VARS)

LOCAL_MODULE := extrace_test_host
LOCAL_MODULE_TAGS := tests
LOCAL_MODULE_HOST := linux

LOCAL_CPPFLAGS := $(common_cflags)
LOCAL_C_INCLUDES := $(TOP)/system/core/cutils
LOCAL_SRC_FILES := \
		$(common_src_files) \
	  $(processrecordfile_src_files) \
    ${common_test_files} \
		src/extrace_test.cpp \

LOCAL_STATIC_LIBRARIES := \
    libgmock_host \

LOCAL_SHARED_LIBRARIES := \
    libutils \
    libbacktrace \
    libcutils \

include $(BUILD_HOST_NATIVE_TEST)
