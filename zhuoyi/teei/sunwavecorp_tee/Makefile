#include $(srctree)/drivers/misc/mediatek/Makefile.custom
ccflags-y += -I$(srctree)/drivers/spi/mediatek/$(MTK_PLATFORM)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include
ccflags-y += -I$(srctree)/drivers/spi/mediatek/$(MTK_PLATFORM)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/teei/V1.0/tz_vfs
ccflags-y += -I$(srctree)/drivers/misc/mediatek/teei/V1.0/tz_driver/include/
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/tkcore/core

obj-y += sunwave_fp.o
sunwave_fp-objs          := sf_ctl.o sf_spi.o

