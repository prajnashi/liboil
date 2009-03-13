LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
        liboilcpu.c         \
        liboildebug.c       \
        liboilfault.c       \
        liboilfunction.c    \
        liboilmarshal.c     \
        liboilprofile.c     \
        liboilprototype.c   \
        liboilrandom.c      \
        liboiltest.c        \
        liboilutils.c       \
        liboilcpu-arm.c     \
        ./android/liboil/liboilarray.c       \
	./c/ag_clamp.c      \
	./c/composite.c     \
        ./c/copy.c          \
	./c/swab.c          \
	./c/wavelet.c       \
	./colorspace/composite.c    \
	./conv/conv_c.c     \
        ./conv/conv_bitstuff.c      \
        ./conv/conv_misc.c  \
        ./copy/copy.c       \
        ./copy/copy8x8.c    \
        ./copy/splat_ref.c  \
        ./copy/trans8x8_c.c \
        ./dct/dct12_f32.c   \
        ./dct/dct36_f32.c   \
        ./dct/fdct8_f64.c   \
        ./dct/fdct8x8_f64.c \
        ./dct/fdct8x8s_s16.c        \
        ./dct/fdct8x8theora.c       \
        ./dct/idct8_f64.c   \
        ./dct/idct8x8_c.c   \
        ./dct/imdct32_f32.c \
        ./dct/idct8x8theora_ref.c   \
        ./jpeg/convert8x8_c.c       \
        ./jpeg/jpeg_rgb_decoder.c   \
        ./jpeg/quantize8x8_c.c      \
        ./jpeg/yuv2rgb_c.c          \
        ./jpeg/zigzag8x8_c.c        \
        ./math/math.c       \
        ./math/ag_math.c    \
        ./md5/md5.c         \
        ./ref/addc.c        \
        ./ref/argb_paint.c  \
        ./ref/ayuv2argb.c   \
        ./ref/clamp.c       \
        ./ref/composite.c   \
        ./ref/convert.c     \
        ./ref/copy.c        \
        ./ref/copy8x8.c     \
        ./ref/diff8x8.c     \
        ./ref/diffsquaresum_f64.c \
        ./ref/diffsquaresum_f32.c \
        ./ref/error8x8.c    \
        ./ref/mas.c         \
        ./ref/math.c        \
        ./ref/mix_u8.c      \
        ./ref/mt19937ar.c   \
        ./ref/mult8x8_s16.c \
        ./ref/multsum.c     \
        ./ref/recon8x8.c    \
        ./ref/resample.c    \
        ./ref/rgb.c         \
        ./ref/rowcolsad8x8.c        \
        ./ref/sad8x8.c      \
        ./ref/sad8x8_broken.c       \
        ./ref/sad8x8avg.c   \
        ./ref/sincos_f64.c  \
        ./ref/splat.c       \
        ./ref/squaresum_f32.c       \
        ./ref/squaresum_f64.c       \
        ./ref/sum.c         \
        ./ref/sum_f64.c     \
        ./ref/swab.c        \
        ./ref/trans8x8.c    \
        ./ref/yuv.c         \
        ./ref/wavelet.c     \
        ./simdpack/diffsquaresum_f64.c \
        ./simdpack/mix_u8.c         \
        ./simdpack/multsum.c        \
        ./simdpack/sincos_f64.c     \
        ./simdpack/squaresum_f64.c  \
        ./simdpack/sum_f64.c        \
        ./simdpack/abs_misc.c       \
        ./simdpack/average2_u8.c    \
        ./simdpack/clip_fast.c      \
        ./simdpack/scalaradd.c      \
        ./simdpack/scalarmult.c     \
        ./utf8/utf8.c       \
        ./utf8/utf8_fast.c  \
        ./deprecated/abs.c  \
        ./deprecated/average2_u8.c  \
        ./deprecated/clip_ref.c     \
        ./deprecated/conv.c         \
        ./deprecated/permute.c      \
        ./deprecated/scalaradd.c    \
        ./deprecated/scalarmult.c   \
        ./deprecated/tablelookup.c  \
        ./deprecated/vectoradd_f64.c    \
        ./deprecated/vectoradd_s.c  \
        ./arm/math_vfp_asm.S        \
        ./arm/math_vfp.c            \

LOCAL_SHARED_LIBRARIES := libm

LOCAL_MODULE:= liboil

LOCAL_C_INCLUDES := 			\
	$(LIBOIL_TOP)			\
	$(LIBOIL_TOP)/android		\
	$(LOCAL_PATH)			\
	$(LOCAL_PATH)/android   	\
	$(LOCAL_PATH)/android/liboil   	\
	$(LOCAL_PATH)/c         	\
	$(LOCAL_PATH)/colorspace    	\
	$(LOCAL_PATH)/conv      	\
	$(LOCAL_PATH)/copy

LOCAL_CFLAGS := \
    -DHAVE_CONFIG_H		    \
    -D_BSD_SOURCE           \
    -D_GNU_SOURCE	        \
    -DOIL_ENABLE_UNSTABLE_API

include $(BUILD_SHARED_LIBRARY)
