LOCAL_PATH := $(call my-dir)  
  
include $(CLEAR_VARS)  
  
LOCAL_MODULE    := libspeexdsp  
LOCAL_CFLAGS = -DFIXED_POINT -DUSE_KISS_FFT -DEXPORT="" -UHAVE_CONFIG_H
LOCAL_LDLIBS:= -L$(SYSROOT)/usr/lib -llog
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
  
LOCAL_SRC_FILES :=	\
	./speexdsp_jni.cpp \
	./libspeexdsp/fftwrap.c \
	./libspeexdsp/filterbank.c \
	./libspeexdsp/kiss_fft.c \
	./libspeexdsp/kiss_fftr.c \
	./libspeexdsp/mdf.c \
	./libspeexdsp/preprocess.c \
  
include $(BUILD_SHARED_LIBRARY)  