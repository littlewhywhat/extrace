common_src_files := \
    src/abstractconsoleapp.cpp \
    src/argsparser.cpp \
    src/arguments.cpp \
    src/consoleappmain.cpp \
    src/extraceconsoleapp.cpp \
    src/extraceconsoleappbuilder.cpp \

common_test_files := \
    src/argsparser_test.cpp \
    src/arguments_test.cpp \
    src/consoleappmain_test.cpp \
    src/extrace_test.cpp \

common_cflags := \
    -Werror \
    -Wall \
    -std=c++11 \

LOCAL_PATH:= $(call my-dir)

# extrace
# ==========

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    $(common_src_files) \
    src/main.cpp \

LOCAL_MODULE:= extrace

LOCAL_CPPFLAGS := $(common_cflags)

include $(BUILD_EXECUTABLE)

# atrace refactored
# =================

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
		src/systemtime_impl.cpp \
		src/androidsystem_impl.cpp \
		src/atraceapp.cpp \
		src/atrace.cpp \
		src/filesystem_impl.cpp \
		src/kernelsystem_impl.cpp \
		src/androidtoolbox.cpp \
    src/trace_impl.cpp \
		

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
    $(common_test_files) \

LOCAL_STATIC_LIBRARIES := \
    libgmock \

include $(BUILD_NATIVE_TEST)

# extrace_test host
# =================

include $(CLEAR_VARS)

LOCAL_MODULE := extrace_test_host
LOCAL_MODULE_TAGS := tests
LOCAL_MODULE_HOST := linux

LOCAL_CPPFLAGS := $(common_cflags)

LOCAL_SRC_FILES := \
    $(common_src_files) \
    $(common_test_files) \

LOCAL_STATIC_LIBRARIES := \
    libgmock_host \

include $(BUILD_HOST_NATIVE_TEST)
