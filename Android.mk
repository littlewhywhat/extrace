LOCAL_PATH:= $(call my-dir)

# extrace
# ==========

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
   src/greeter.cpp \
   src/main.cpp \

LOCAL_MODULE:= extrace

LOCAL_CPPFLAGS := -Werror -Wall -std=c++11

include $(BUILD_EXECUTABLE)

# extrace_test target
# ===================

include $(CLEAR_VARS)

LOCAL_MODULE := extrace_test
LOCAL_MODULE_TAGS := tests

LOCAL_CPPFLAGS := -Werror -Wall -std=c++11

LOCAL_SRC_FILES := \
    src/greeter.cpp \
    src/greeter_test.cpp \
    src/extrace_test.cpp \

LOCAL_STATIC_LIBRARIES := \
    libgmock \

include $(BUILD_NATIVE_TEST)

# extrace_test
# =================

include $(CLEAR_VARS)

LOCAL_MODULE := extrace_test_host
LOCAL_MODULE_TAGS := tests
LOCAL_MODULE_HOST := linux

LOCAL_CPPFLAGS := -Werror -Wall -std=c++11

LOCAL_SRC_FILES := \
    src/greeter.cpp \
    src/greeter_test.cpp \
    src/extrace_test.cpp \

LOCAL_STATIC_LIBRARIES := \
    libgmock_host \

include $(BUILD_HOST_NATIVE_TEST)
