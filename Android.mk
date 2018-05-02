common_src_files := \
    src/abstractconsoleapp.cpp \
    src/argsparser.cpp \
    src/arguments.cpp \
    src/consoleappmain.cpp \
    src/extraceconsoleapp.cpp \
    src/extraceconsoleappbuilder.cpp \
    src/extracearguments.cpp \
    src/androidtoolbox.cpp \
    src/systemcore.cpp \

common_test_files := \
    src/argsparser_test.cpp \
    src/arguments_test.cpp \
    src/consoleappmain_test.cpp \
    src/extrace_test.cpp \

common_cflags := \
    -Wall \
    -std=c++11 \

LOCAL_PATH:= $(call my-dir)

# extrace
# ==========

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    $(common_src_files) \
    src/systemtime_impl.cpp \
		src/androidsystem_impl.cpp \
		src/filesystem_impl.cpp \
		src/kernelsystem_impl.cpp \
    src/trace_impl.cpp \
    src/listsupportedcategories.cpp \
		src/addkernelcategoriesfromfiletotrace.cpp \
		src/addandroidcoretotrace.cpp \
		src/actionrunner_impl.cpp \
		src/main.cpp \
		src/sleepaction.cpp \
		src/startaction.cpp \
		src/streamaction.cpp \
		src/stopaction.cpp \
		src/cleanupaction.cpp \
		src/dumpaction.cpp \
		src/actionrunnerbuilder.cpp \

LOCAL_MODULE:= extrace

LOCAL_CPPFLAGS := $(common_cflags)

LOCAL_C_INCLUDES += external/zlib

LOCAL_MODULE_TAGS:= optional

LOCAL_SHARED_LIBRARIES := \
    libbinder \
    libcutils \
    libutils \
    libz \

include $(BUILD_EXECUTABLE)

# extrace
# ==========

include $(CLEAR_VARS)

# LOCAL_SRC_FILES:= \
#     src/systemtime_impl.cpp \
# 		src/androidsystem_impl.cpp \
# 		src/filesystem_impl.cpp \
# 		src/kernelsystem_impl.cpp \
# 		src/androidtoolbox.cpp \
#     src/trace_impl.cpp \
# 		src/startaction.cpp \
# 		src/stopaction.cpp \
# 		src/dumpaction.cpp \
# 		src/streamaction.cpp \
# 		src/sleepaction.cpp \
# 		src/cleanupaction.cpp \
# 		src/addandroidcoretotrace.cpp \
# 		src/addkernelcategoriesfromfiletotrace.cpp \
# 		src/listsupportedcategories.cpp \
# 		src/signal.cpp \
# 		src/actionrunner_impl.cpp \
# 		src/argsparser.cpp \
#     src/arguments.cpp \
#     src/extraceapp.cpp \

LOCAL_SRC_FILES:= \
			src/signal.cpp \
			src/cmdlineapp.cpp \
			src/actionrunner.cpp \
			src/environment.cpp \
			src/kernelsystembuilder.cpp \
			src/androidsystembuilder.cpp \
			src/tracesystembuilder.cpp \
			src/tracebuilder.cpp \
			src/listsupportedcategories.cpp \
			src/addandroidcoretotrace.cpp \
			src/addkernelcategoriesfromfiletotrace.cpp \
			src/sleepaction.cpp \
			src/startaction.cpp \
			src/streamaction.cpp \
			src/cleanupaction.cpp \
			src/stopaction.cpp \
			src/dumpaction.cpp \
			src/traceactionsrunner.cpp \
			src/traceactionsrunnerbuilder.cpp \
			src/extraceactioncmdlinebuilder.cpp \
			src/tracesystem.cpp \
			src/extracearguments.cpp \
			src/extraceargumentsbuilder.cpp \
	    src/androidtoolbox.cpp \
			src/arguments.cpp \
			src/cmdlineargsparser.cpp \
			src/cmdlineargs.cpp \
			src/wire.cpp \
			src/systemtime_impl.cpp \
			src/kernelsystem_impl.cpp \
			src/androidsystem_impl.cpp \
			src/trace_impl.cpp \
	    src/experiment.cpp \

LOCAL_MODULE:= experiment

LOCAL_CPPFLAGS := $(common_cflags)

LOCAL_C_INCLUDES += external/zlib

LOCAL_MODULE_TAGS:= optional

LOCAL_SHARED_LIBRARIES := \
    libbinder \
    libcutils \
    libutils \
    libz \

include $(BUILD_EXECUTABLE)

# atrace refactored
# =================

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
		src/systemtime_impl.cpp \
		src/androidsystem_impl.cpp \
		src/atrace.cpp \
		src/filesystem_impl.cpp \
		src/kernelsystem_impl.cpp \
		src/androidtoolbox.cpp \
    src/trace_impl.cpp \
		src/startaction.cpp \
		src/stopaction.cpp \
		src/dumpaction.cpp \
		src/streamaction.cpp \
		src/sleepaction.cpp \
		src/cleanupaction.cpp \
		src/addandroidcoretotrace.cpp \
		src/addkernelcategoriesfromfiletotrace.cpp \
		src/listsupportedcategories.cpp \
		src/signal.cpp \
		src/actionrunner_impl.cpp \
		src/argsparser.cpp \
    src/arguments.cpp \
    src/extraceapp.cpp

LOCAL_MODULE:= atrace_ref

LOCAL_CPPFLAGS := $(common_cflags)

LOCAL_C_INCLUDES += external/zlib

LOCAL_MODULE_TAGS:= optional

LOCAL_SHARED_LIBRARIES := \
    libbinder \
    libcutils \
    libutils \
    libz \

include $(BUILD_EXECUTABLE)

LOCAL_INIT_RC := atrace_ref.rc

# extrace_test target
# ===================

include $(CLEAR_VARS)

LOCAL_MODULE := extrace_test
LOCAL_MODULE_TAGS := tests

LOCAL_CPPFLAGS := $(common_cflags)

LOCAL_SRC_FILES := \
    $(common_src_files) \
		src/systemtime_impl.cpp \
		src/androidsystem_impl.cpp \
		src/filesystem_impl.cpp \
		src/kernelsystem_impl.cpp \
    src/trace_impl.cpp \
		src/startaction.cpp \
		src/stopaction.cpp \
		src/dumpaction.cpp \
		src/streamaction.cpp \
		src/sleepaction.cpp \
		src/cleanupaction.cpp \
		src/addandroidcoretotrace.cpp \
		src/addkernelcategoriesfromfiletotrace.cpp \
		src/listsupportedcategories.cpp \
		src/signal.cpp \
		src/actionrunner_impl.cpp \
    src/extraceapp.cpp \
    $(common_test_files) \
    src/extraceapp_test.cpp \

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

# include $(CLEAR_VARS)

# LOCAL_MODULE := extrace_test_host
# LOCAL_MODULE_TAGS := tests
# LOCAL_MODULE_HOST := linux

# LOCAL_CPPFLAGS := $(common_cflags)
# LOCAL_C_INCLUDES := $(TOP)/system/core/cutils
# LOCAL_SRC_FILES := \
#     $(common_src_files) \
# 		src/systemtime_impl.cpp \
# 		src/filesystem_impl.cpp \
# 		src/kernelsystem_impl.cpp \
#     src/trace_impl.cpp \
# 		src/startaction.cpp \
# 		src/stopaction.cpp \
# 		src/dumpaction.cpp \
# 		src/streamaction.cpp \
# 		src/sleepaction.cpp \
# 		src/cleanupaction.cpp \
# 		src/signal.cpp \
# 		src/actionrunner_impl.cpp \
#     src/extraceapp.cpp \
#     $(common_test_files) \
#     src/extraceapp_test.cpp \

# LOCAL_STATIC_LIBRARIES := \
#     libgmock_host \
#     libcutils \
#     libz \

# LOCAL_SHARED_LIBRARIES := \
#     libutils \
#     libbacktrace \

# include $(BUILD_HOST_NATIVE_TEST)
