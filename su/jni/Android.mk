LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
	/home/vasiliev/android_source/M4520/system/core/include

LOCAL_MODULE    := su
LOCAL_SRC_FILES += su.c
LOCAL_LDLIBS    += -llog
include $(BUILD_EXECUTABLE)
