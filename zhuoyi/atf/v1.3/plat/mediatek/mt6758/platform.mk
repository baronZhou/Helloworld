#
# Copyright (c) 2015, ARM Limited and Contributors. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# Neither the name of ARM nor the names of its contributors may be used
# to endorse or promote products derived from this software without specific
# prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

MTK_PLAT		:=	plat/mediatek
MTK_PLAT_SOC		:=	${MTK_PLAT}/${PLAT}

# Add OEM customized codes
OEMS				:= true
MTK_SIP_KERNEL_BOOT_ENABLE := 1


ifneq (${OEMS},none)
  OEMS_INCLUDES		:= -I${MTK_PLAT}/common/custom/
  OEMS_SOURCES		:=	${MTK_PLAT}/common/custom/oem_svc.c
endif

PLAT_INCLUDES		:=	-I${MTK_PLAT}/common/		\
				-I${MTK_PLAT}/common/drivers/uart		\
				-I${MTK_PLAT_SOC}/									\
				-I${MTK_PLAT_SOC}/drivers/devapc/		\
				-I${MTK_PLAT_SOC}/drivers/emi/			\
				-I${MTK_PLAT_SOC}/drivers/gpio/			\
				-I${MTK_PLAT_SOC}/drivers/spm/			\
				-I${MTK_PLAT_SOC}/drivers/spmc/			\
				-I${MTK_PLAT_SOC}/drivers/mcdi/			\
				-I${MTK_PLAT_SOC}/drivers/pmic/			\
				-I${MTK_PLAT_SOC}/drivers/rtc/			\
				-I${MTK_PLAT_SOC}/drivers/timer/		\
				-I${MTK_PLAT_SOC}/drivers/l2c/			\
				-I${MTK_PLAT_SOC}/drivers/wfifo/                \
				-I${MTK_PLAT_SOC}/drivers/ocp/			\
				-I${MTK_PLAT_SOC}/drivers/dbgtool/	\
				-I${MTK_PLAT_SOC}/drivers/md/ \
				-I${MTK_PLAT_SOC}/drivers/sec/			\
				-I${MTK_PLAT_SOC}/drivers/udi/			\
				-I${MTK_PLAT_SOC}/drivers/crypto/		\
				-I${MTK_PLAT_SOC}/drivers/scp/			\
				-I${MTK_PLAT_SOC}/include/					\
				-Iinclude/plat/arm/common/					\
				-Iinclude/common/tbbr/							\
				-Idrivers/arm/gic/v3/								\
				${OEMS_INCLUDES}

PLAT_BL_COMMON_SOURCES	:=	lib/aarch64/xlat_tables.c			\
				plat/common/aarch64/plat_common.c		\
				plat/common/plat_gic.c

BL31_SOURCES		+=	drivers/arm/cci/cci.c				\
				drivers/arm/gic/arm_gic.c								\
				drivers/arm/gic/gic_v2.c								\
				drivers/arm/gic/gic_v3.c								\
				drivers/arm/gic/v3/gicv3_helpers.c			\
				drivers/mediatek/fiq_cache/fiq_cache_v1.c		\
				drivers/console/console.S								\
				lib/cpus/aarch64/aem_generic.S					\
				lib/cpus/aarch64/cortex_a35.S						\
				lib/cpus/aarch64/cortex_a53.S						\
				lib/cpus/aarch64/cortex_a57.S						\
				lib/cpus/aarch64/cortex_a72.S						\
				plat/common/aarch64/platform_mp_stack.S	\
				${MTK_PLAT}/common/mtk_sip_svc.c				\
				${MTK_PLAT}/common/fiq_smp_call.c       \
				${MTK_PLAT}/common/plat_aee_debug.c     \
				${MTK_PLAT}/common/mtk_plat_common.c		\
				${MTK_PLAT}/common/drivers/uart/8250_console.S	\
				${MTK_PLAT}/common/log.c								\
				${MTK_PLAT}/common/bl31_fiq_handler.c		\
				${MTK_PLAT_SOC}/aarch64/plat_helpers.S	\
				${MTK_PLAT_SOC}/bl31_plat_setup.c				\
				${MTK_PLAT_SOC}/plat_sip_svc.c					\
				${MTK_PLAT_SOC}/plat_mt_gic.c						\
				${MTK_PLAT_SOC}/plat_lastpc.c						\
				${MTK_PLAT_SOC}/plat_topology.c					\
				${MTK_PLAT_SOC}/plat_pm.c											\
				${MTK_PLAT_SOC}/power_tracer.c					\
				${MTK_PLAT_SOC}/scu.c

ifeq ($(ATF_BYPASS_DRAM), yes)
BL31_SOURCE_DRAM :=
else
BL31_SOURCE_DRAM := drivers/mediatek/mcsi/mcsi.c			\
				drivers/delay_timer/delay_timer.c				\
				${MTK_PLAT_SOC}/drivers/dbgtool/latch.c				\
				${MTK_PLAT_SOC}/drivers/devapc/devapc.c				\
				${MTK_PLAT_SOC}/drivers/emi/emi_mpu.c					\
				${MTK_PLAT_SOC}/drivers/gpio/gpio.c						\
				${MTK_PLAT_SOC}/drivers/md/md.c								\
				${MTK_PLAT_SOC}/drivers/pmic/pmic_wrap_init.c	\
				${MTK_PLAT_SOC}/drivers/sec/rpmb_hmac.c				\
				${MTK_PLAT_SOC}/drivers/sec/rng.c \
				${MTK_PLAT_SOC}/drivers/rtc/rtc.c							\
				${MTK_PLAT_SOC}/drivers/spmc/mtspmc.c					\
				${MTK_PLAT_SOC}/drivers/timer/mt_cpuxgpt.c		\
				${MTK_PLAT_SOC}/drivers/timer/mt_systimer.c	\
				${MTK_PLAT_SOC}/drivers/l2c/l2c.c							\
				${MTK_PLAT_SOC}/drivers/wfifo/wfifo.c				\
				${MTK_PLAT_SOC}/drivers/ocp/mtk_ocp_api.c			\
				${MTK_PLAT_SOC}/drivers/udi/mtk_udi_api.c			\
				${MTK_PLAT_SOC}/drivers/scp/scp_dvfs.c				\
				${MTK_PLAT_SOC}/drivers/crypto/crypto.c 			\
				${MTK_PLAT_SOC}/drivers/crypto/msdc_crypto.c	\
				${MTK_PLAT_SOC}/drivers/spm/mt_spm.c					\
				${MTK_PLAT_SOC}/drivers/spm/mt_spm_sleep.c		\
				${MTK_PLAT_SOC}/drivers/spm/mt_spm_dpidle.c		\
				${MTK_PLAT_SOC}/drivers/spm/mt_spm_sodi.c			\
				${MTK_PLAT_SOC}/drivers/spm/mt_spm_idle.c			\
				${MTK_PLAT_SOC}/drivers/spm/mt_spm_internal.c	\
				${MTK_PLAT_SOC}/drivers/spm/mt_spm_vcorefs.c	\
				${MTK_PLAT_SOC}/drivers/mcdi/mtk_mcdi.c       \
				${MTK_PLAT_SOC}/plat_dcm.c										\
				${MTK_PLAT_SOC}/plat_delay_timer.c			\
				${MTK_PLAT_SOC}/plat_dfd.c										\
				${OEMS_SOURCES}
endif

STATIC_LIBS += ${MTK_PLAT_SOC}/drivers/sec/lib/sha2.a

ifeq (${SPD}, tbase)
BL31_SOURCE_DRAM += ${MTK_PLAT_SOC}/plat_tbase.c
endif

ifeq (${SPD}, teeid)
BL31_SOURCE_DRAM += ${MTK_PLAT_SOC}/plat_teei.c
endif

# Flag used by the MTK_platform port to determine the version of ARM GIC
# architecture to use for interrupt management in EL3.
ARM_GIC_ARCH		:=	2
$(eval $(call add_define,ARM_GIC_ARCH))

# Enable workarounds for selected Cortex-A53 erratas.
ERRATA_A53_826319	:=	0
ERRATA_A53_836870	:=	1
ERRATA_A53_855873	:=	1

# indicate the reset vector address can be programmed
PROGRAMMABLE_RESET_ADDRESS	:=	1

# Disable the PSCI platform compatibility layer
ENABLE_PLAT_COMPAT	:=	1

# Enable SRAM_FLAG support for DFD/LASTPC
CFG_PLAT_SRAM_FLAG	:=	1

ifeq (${MACH_TYPE},mt6758)
CONFIG_MACH_MT6758	:=	1
$(eval $(call add_define,CONFIG_MACH_MT6758))
endif

ifeq (${ATF_BYPASS_DRAM},yes)
$(eval $(call add_define,ATF_BYPASS_DRAM))
endif

#Full ATF ram dump
ifeq (${MTK_ATF_RAM_DUMP},yes)
MTK_ATF_RAM_DUMP := 1
$(eval $(call add_define,MTK_ATF_RAM_DUMP))
endif

# Enable ACAO support
ifeq (${MTK_ACAO_SUPPORT},yes)
$(eval $(call add_define,MTK_ACAO_SUPPORT))
endif

ifeq (${CFG_PLAT_SRAM_FLAG},1)
$(eval $(call add_define,CFG_PLAT_SRAM_FLAG))
endif

ifeq (${DRAM_EXTENSION_SUPPORT},yes)
$(eval $(call add_define,DRAM_EXTENSION_SUPPORT))
endif

ifeq (${MTK_MSDC_HW_FDE},yes)
$(eval $(call add_define,MTK_MSDC_HW_FDE))
endif

$(eval $(call add_define,MTK_SIP_KERNEL_BOOT_ENABLE))

ifeq (${MTK_FIQ_CACHE_SUPPORT},yes)
$(eval $(call add_define,MTK_FIQ_CACHE_SUPPORT))
endif
