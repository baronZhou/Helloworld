/*
 * Copyright (c) 2014-2015, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __PLATFORM_DEF_H__
#define __PLATFORM_DEF_H__

#include <arch.h>

#define PLAT_PRIMARY_CPU	0x0
#define PLAT_USE_EL1_AARCH32

/* Special value used to verify platform parameters from BL2 to BL3-1 */
#define MT_BL31_PLAT_PARAM_VAL	0x0f1e2d3c4b5a6978ULL

#define IO_PHYS          (0x10000000)
#define INFRACFG_AO_BASE (IO_PHYS + 0x1000)
#define PERI_BASE        (IO_PHYS + 0x3000)
#define GPIO_BASE        (IO_PHYS + 0x5000)
#define SPM_BASE         (IO_PHYS + 0x6000)
#define RGU_BASE         (IO_PHYS + 0x7000)
#define PMIC_WRAP_BASE   (IO_PHYS + 0xD000)
#define MCUCFG_BASE      (0x0C530000)
#define EMI_MPU_BASE     (IO_PHYS + 0x226000)
#define TRNG_base        (IO_PHYS + 0x20F000)
#define MT_GIC_BASE      (0x0C000000)
#define PLAT_MT_CCI_BASE (IO_PHYS + 0x390000)
#define EINT_BASE        (0x1000b000)
#define DVFSRC_BASE      (0x10012000)

#define SSPM_CFGREG_BASE (IO_PHYS + 0x440000)
#define SSPM_MBOX_3_BASE (IO_PHYS + 0x480000)

#define INFRACFG_AO_BASE (IO_PHYS + 0x1000)
#define SPM_BASE         (IO_PHYS + 0x6000)

#define APMIXEDSYS       (IO_PHYS + 0xC000)
#define ARMPLL_LL_CON0   (APMIXEDSYS+0x200)
#define ARMPLL_L_CON0    (APMIXEDSYS+0x210)
#define MAINPLL_CON0     (APMIXEDSYS+0x220)
#define CCIPLL_CON0      (APMIXEDSYS+0x290)

#define TOP_CKMUXSEL     (INFRACFG_AO_BASE+0x0)

#define armpll_mux1_sel_big_mask    (0xF<<4)
#define armpll_mux1_sel_big_ARMSPLL (0x1<<4)
#define armpll_mux1_sel_sml_mask    (0xF<<8)
#define armpll_mux1_sel_sml_ARMSPLL (0x1<<8)


/* Aggregate of all devices in the first GB */
#define MTK_DEV_RNG0_BASE IO_PHYS
#define MTK_DEV_RNG0_SIZE 0x490000
#define MTK_DEV_RNG1_BASE (IO_PHYS + 0x1000000)
#define MTK_DEV_RNG1_SIZE 0x4000000
#define MTK_DEV_RNG2_BASE (MT_GIC_BASE)
#define MTK_DEV_RNG2_SIZE 0x400000
#define MT_MCUSYS_SIZE    0x90000
#define RAM_CONSOLE_BASE  0x11C000
#define RAM_CONSOLE_SIZE  0x2000
/*******************************************************************************
 * Modem permission control hardware (PMS)
 ******************************************************************************/
#define PMS_EN_SET_CONFIG		(INFRACFG_AO_BASE+0x8ac)
#define PMS_EN_LOCK_CONFIG		(INFRACFG_AO_BASE+0x8a8)

/*******************************************************************************
 * MSDC
 ******************************************************************************/
#define MSDC0_BASE        (IO_PHYS + 0x01230000)

/*******************************************************************************
 * UFS
 ******************************************************************************/
#define PERICFG_BASE      (IO_PHYS + 0x00003000)

/*******************************************************************************
 * UFS
 ******************************************************************************/
#define UFSHCI_BASE       (IO_PHYS + 0x01270000)

/*******************************************************************************
 * MCUSYS related constants
 ******************************************************************************/
#define MT_L2_WRITE_ACCESS_RATE (MCUCFG_BASE + 0x604)
#define MP0_CA7L_CACHE_CONFIG   (MCUCFG_BASE + 0x7F0)
#define MP1_CA7L_CACHE_CONFIG   (MCUCFG_BASE + 0x7F4)

/*******************************************************************************
 * GIC related constants
 ******************************************************************************/
#define MT_POLARITY_LOW    0
#define MT_POLARITY_HIGH   1
#define MT_EDGE_SENSITIVE  1
#define MT_LEVEL_SENSITIVE 0

/*******************************************************************************
 * UART related constants
 ******************************************************************************/
#define UART0_BASE    (IO_PHYS + 0x01002000)

#define UART_BAUDRATE (921600)
#define UART_CLOCK    (10000000)

/*******************************************************************************
 * System counter frequency related constants
 ******************************************************************************/
#define SYS_COUNTER_FREQ_IN_TICKS 13000000
#define SYS_COUNTER_FREQ_IN_MHZ   13

/*******************************************************************************
 * GIC-400 & interrupt handling related constants
 ******************************************************************************/

/* Base MTK_platform compatible GIC memory map */
#define BASE_GICD_BASE    (MT_GIC_BASE)
#define BASE_GICC_BASE    (MT_GIC_BASE + 0x400000)
#define MT_GIC_RDIST_BASE (MT_GIC_BASE + 0x100000)
#define BASE_GICR_BASE    (MT_GIC_BASE + 0x100000)
#define BASE_GICH_BASE    (MT_GIC_BASE + 0x4000)
#define BASE_GICV_BASE    (MT_GIC_BASE + 0x6000)
#define INT_POL_CTL0      (MCUCFG_BASE + 0x620)
#define SEC_POL_CTL_EN0   (MCUCFG_BASE + 0xA00)

#define GIC_PRIVATE_SIGNALS (32)

/*******************************************************************************
 * CCI-400 related constants
 ******************************************************************************/
#define PLAT_MT_CCI_CLUSTER0_SL_IFACE_IX 4
#define PLAT_MT_CCI_CLUSTER1_SL_IFACE_IX 3

/*******************************************************************************
 * WDT Registers
 ******************************************************************************/
#define MTK_WDT_BASE        (IO_PHYS + 0x00007000)
#define MTK_WDT_SIZE        (0x1000)
#define MTK_WDT_MODE        (MTK_WDT_BASE+0x0000)
#define MTK_WDT_LENGTH      (MTK_WDT_BASE+0x0004)
#define MTK_WDT_RESTART     (MTK_WDT_BASE+0x0008)
#define MTK_WDT_STATUS      (MTK_WDT_BASE+0x000C)
#define MTK_WDT_INTERVAL    (MTK_WDT_BASE+0x0010)
#define MTK_WDT_SWRST       (MTK_WDT_BASE+0x0014)
#define MTK_WDT_SWSYSRST    (MTK_WDT_BASE+0x0018)
#define MTK_WDT_NONRST_REG  (MTK_WDT_BASE+0x0020)
#define MTK_WDT_NONRST_REG2 (MTK_WDT_BASE+0x0024)
#define MTK_WDT_REQ_MODE    (MTK_WDT_BASE+0x0030)
#define MTK_WDT_REQ_IRQ_EN  (MTK_WDT_BASE+0x0034)
#define MTK_WDT_EXT_REQ_CON (MTK_WDT_BASE+0x0038)
#define MTK_WDT_DEBUG_CTL   (MTK_WDT_BASE+0x0040)
#define MTK_WDT_LATCH_CTL   (MTK_WDT_BASE+0x0044)
#define MTK_WDT_DEBUG_CTL2  (MTK_WDT_BASE+0x00A0)
#define MTK_WDT_COUNTER     (MTK_WDT_BASE+0x0514)

/* WDT_STATUS */
#define MTK_WDT_STATUS_SPM_THERMAL_RST      (1 << 0)
#define MTK_WDT_STATUS_SPM_RST              (1 << 1)
#define MTK_WDT_STATUS_EINT_RST             (1 << 2)
#define MTK_WDT_STATUS_SYSRST_RST           (1 << 3) /* from PMIC */
#define MTK_WDT_STATUS_DVFSP_RST            (1 << 4)
#define MTK_WDT_STATUS_PMCU_RST             (1 << 16)
#define MTK_WDT_STATUS_MDDBG_RST            (1 << 17)
#define MTK_WDT_STATUS_THERMAL_DIRECT_RST   (1 << 18)
#define MTK_WDT_STATUS_DEBUG_RST            (1 << 19)
#define MTK_WDT_STATUS_SECURITY_RST         (1 << 28)
#define MTK_WDT_STATUS_IRQ_ASSERT           (1 << 29)
#define MTK_WDT_STATUS_SW_WDT_RST           (1 << 30)
#define MTK_WDT_STATUS_HW_WDT_RST           (1 << 31)

/* RGU other related */
#define MTK_WDT_MODE_DUAL_MODE 0x0040
#define MTK_WDT_MODE_IRQ       0x0008
#define MTK_WDT_MODE_KEY       0x22000000
#define MTK_WDT_MODE_EXTEN     0x0004
#define MTK_WDT_SWRST_KEY      0x1209
#define MTK_WDT_RESTART_KEY    (0x1971)

/* FIQ platform related define */
#define MT_IRQ_SEC_SGI_0 8
#define MT_IRQ_SEC_SGI_1 9
#define MT_IRQ_SEC_SGI_2 10
#define MT_IRQ_SEC_SGI_3 11
#define MT_IRQ_SEC_SGI_4 12
#define MT_IRQ_SEC_SGI_5 13
#define MT_IRQ_SEC_SGI_6 14
#define MT_IRQ_SEC_SGI_7 15

#define FIQ_SMP_CALL_SGI 13
#define WDT_IRQ_BIT_ID   (171)
#define ATF_LOG_IRQ_ID   155

#define DEBUG_XLAT_TABLE 0

/*******************************************************************************
 * Platform binary types for linking
 ******************************************************************************/
#define PLATFORM_LINKER_FORMAT "elf64-littleaarch64"
#define PLATFORM_LINKER_ARCH   aarch64

/*******************************************************************************
 * Generic platform constants
 ******************************************************************************/

/* Size of cacheable stacks */
#if     DEBUG_XLAT_TABLE
#define PLATFORM_STACK_SIZE 0x800
#elif   IMAGE_BL1
#define PLATFORM_STACK_SIZE 0x440
#elif   IMAGE_BL2
#define PLATFORM_STACK_SIZE 0x400
#elif   IMAGE_BL31
#define PLATFORM_STACK_SIZE 0x800
#elif   IMAGE_BL32
#define PLATFORM_STACK_SIZE 0x440
#endif

#define FIRMWARE_WELCOME_STR "Booting Trusted Firmware\n"
#if     ENABLE_PLAT_COMPAT
#define PLATFORM_MAX_AFFLVL  MPIDR_AFFLVL1
#else
#define PLAT_MAX_PWR_LVL     MPIDR_AFFLVL1
#endif

#define PLATFORM_CACHE_LINE_SIZE      64
#define PLATFORM_SYSTEM_COUNT         0
#define PLATFORM_CLUSTER_COUNT        2
#define PLATFORM_CLUSTER0_CORE_COUNT  4
#define PLATFORM_CLUSTER1_CORE_COUNT  4
#define PLATFORM_CORE_COUNT           (PLATFORM_CLUSTER1_CORE_COUNT + PLATFORM_CLUSTER0_CORE_COUNT)
#define PLATFORM_MAX_CPUS_PER_CLUSTER 4
#define PLATFORM_NUM_AFFS             (PLATFORM_SYSTEM_COUNT + PLATFORM_CLUSTER_COUNT + PLATFORM_CORE_COUNT)

/*******************************************************************************
 * Platform memory map related constants
 ******************************************************************************/
/* ATF Argument */
#define ATF_ARG_SIZE  (0x800)

/* TF txet, ro, rw, internal SRAM, Size: release: 80KB, debug: 92KB */
#ifdef DRAM_EXTENSION_SUPPORT
#define TZRAM_BASE		(0x100000)
#else //DRAM_EXTENSION_SUPPORT
#define TZRAM_BASE		(0x54600000)
#endif //DRAM_EXTENSION_SUPPORT

#if DEBUG
#ifdef DRAM_EXTENSION_SUPPORT
#define TZRAM_SIZE (0x1B000)
#else //DRAM_EXTENSION_SUPPORT
#define TZRAM_SIZE (0x28000)
#endif //DRAM_EXTENSION_SUPPORT
#else //DEBUG
#ifdef DRAM_EXTENSION_SUPPORT
#define TZRAM_SIZE		(0x1B000)
#else
#define TZRAM_SIZE		(0x28000)
#endif

#endif
#define ATF_DRAM_TOTAL_SIZE 0x40000

/* xlat_table , coherence ram, 64KB */
#ifdef DRAM_EXTENSION_SUPPORT
#define TZRAM2_BASE		(0x54600000)
#define TZRAM2_SIZE		(ATF_DRAM_TOTAL_SIZE)
#else
#define TZRAM2_BASE		(TZRAM_BASE + TZRAM_SIZE)
#define TZRAM2_SIZE		(ATF_DRAM_TOTAL_SIZE - TZRAM_SIZE)
#endif
#define DRAM_EMI_BASE 0x40000000

/*******************************************************************************
 * BL31 specific defines.
 ******************************************************************************/
/*
 * Put BL3-1 at the top of the Trusted SRAM (just below the shared memory, if
 * present). BL31_BASE is calculated using the current BL3-1 debug size plus a
 * little space for growth.
 */
#ifdef DRAM_EXTENSION_SUPPORT
#define BL31_IMG_SIZE  0x13A00
#else //DRAM_EXTENSION_SUPPORT
#define BL31_IMG_SIZE 0x20000
#endif //DRAM_EXTENSION_SUPPORT
#define BL31_BASE       (TZRAM_BASE  + ATF_ARG_SIZE)
#define BL31_IMG_LIMIT  (TZRAM_BASE  + BL31_IMG_SIZE)
#define BL31_LIMIT      (TZRAM_BASE  + TZRAM_SIZE)
#define TZRAM2_LIMIT    (TZRAM2_BASE + TZRAM2_SIZE)
#define BL31_TZRAM_SIZE (TZRAM_SIZE  - ATF_ARG_SIZE)

/*******************************************************************************
 * Platform specific page table and MMU setup constants
 ******************************************************************************/
#define ADDR_SPACE_SIZE  (1ull << 32)
#define MAX_XLAT_TABLES  40
#define MAX_MMAP_REGIONS 30

/*******************************************************************************
 * Declarations and constants to access the mailboxes safely. Each mailbox is
 * aligned on the biggest cache line size in the platform. This is known only
 * to the platform as it might have a combination of integrated and external
 * caches. Such alignment ensures that two maiboxes do not sit on the same cache
 * line at any cache level. They could belong to different cpus/clusters &
 * get written while being protected by different locks causing corruption of
 * a valid mailbox address.
 ******************************************************************************/
#define CACHE_WRITEBACK_SHIFT   6
#define CACHE_WRITEBACK_GRANULE (1 << CACHE_WRITEBACK_SHIFT)

#define BL32_BASE (0x0)

/******************************************************************************
* MODEM REGISTERs
******************************************************************************/
#define MDPERI_MDSBC_KEY_CONFIG (0x201d7000)
#define MDPERI_MDSBC_KEY_SIZE   (0x1000)
#define LTE_SBC_PUBK_HASH0 (MDPERI_MDSBC_KEY_CONFIG + 0x000)
#define LTE_SBC_PUBK_HASH1 (MDPERI_MDSBC_KEY_CONFIG + 0x004)
#define LTE_SBC_PUBK_HASH2 (MDPERI_MDSBC_KEY_CONFIG + 0x008)
#define LTE_SBC_PUBK_HASH3 (MDPERI_MDSBC_KEY_CONFIG + 0x00c)
#define LTE_SBC_PUBK_HASH4 (MDPERI_MDSBC_KEY_CONFIG + 0x010)
#define LTE_SBC_PUBK_HASH5 (MDPERI_MDSBC_KEY_CONFIG + 0x014)
#define LTE_SBC_PUBK_HASH6 (MDPERI_MDSBC_KEY_CONFIG + 0x018)
#define LTE_SBC_PUBK_HASH7 (MDPERI_MDSBC_KEY_CONFIG + 0x01c)
#define LTE_SBC_LOCK       (MDPERI_MDSBC_KEY_CONFIG + 0x020)

/*******************************************************************************
 * TRNG Registers
 ******************************************************************************/
#define TRNG_BASE_ADDR     TRNG_base
#define TRNG_BASE_SIZE     (0x1000)
#define TRNG_CTRL          (TRNG_base + 0x0000)
#define TRNG_TIME          (TRNG_base + 0x0004)
#define TRNG_DATA          (TRNG_base + 0x0008)
#define TRNG_PDN_base      (0x10001000)
#define TRNG_PDN_BASE_ADDR TRNG_PDN_BASE_ADDR
#define TRNG_PDN_BASE_SIZE (0x1000)
#define TRNG_PDN_SET       (TRNG_PDN_base + 0x0088)
#define TRNG_PDN_CLR       (TRNG_PDN_base + 0x008C)
#define TRNG_PDN_STATUS    (TRNG_PDN_base + 0x0094)
#define TRNG_CTRL_RDY      0x80000000
#define TRNG_CTRL_START    0x00000001
#define TRNG_PDN_VALUE     0x200


#define PLAT_MTK_DFD_SETUP_MAGIC (0x99716150)
#define PLAT_MTK_DFD_READ_MAGIC  (0x99716151)
#define PLAT_MTK_DFD_WRITE_MAGIC (0x99716152)
#endif /* __PLATFORM_DEF_H__ */
