/*
 * Copyright (c) 2015,  ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms,  with or without
 * modification,  are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice,  this
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
 * AND ANY EXPRESS OR IMPLIED WARRANTIES,  INCLUDING,  BUT NOT LIMITED TO,  THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT,  INDIRECT,  INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR
 * CONSEQUENTIAL DAMAGES (INCLUDING,  BUT NOT LIMITED TO,  PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,  DATA,  OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,  WHETHER IN
 * CONTRACT,  STRICT LIABILITY,  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <assert.h>
#include <console.h>
#include <debug.h>
#include <delay_timer.h>
#include <mcucfg.h>
#include <mmio.h>
#include <mt_spm_reg.h>
#include <mtspmc.h>
#include <plat_private.h>
#include <platform.h>
#include <platform_def.h>
#include <psci.h>

#define MP0_CPUTOP_PWR_CON              MCU2_PWR_CON
#define MP0_CPU0_PWR_CON                MCU3_PWR_CON
#define MP0_CPU1_PWR_CON                MCU4_PWR_CON
#define MP0_CPU2_PWR_CON                MCU5_PWR_CON
#define MP0_CPU3_PWR_CON                MCU6_PWR_CON
#define MP1_CPUTOP_PWR_CON              MCU7_PWR_CON
#define MP1_CPU0_PWR_CON                MCU8_PWR_CON
#define MP1_CPU1_PWR_CON                MCU9_PWR_CON
#define MP1_CPU2_PWR_CON                MCU10_PWR_CON
#define MP1_CPU3_PWR_CON                MCU11_PWR_CON
#define MP2_CPUTOP_PWR_CON              MCU12_PWR_CON
#define MP2_CPU0_PWR_CON                MCU13_PWR_CON
#define MP2_CPU1_PWR_CON                MCU14_PWR_CON

#define SPM_CPU_RET_STATUS				CPU_RET_STATUS

#define MCUSYS_CPU_PWR_STA_MASK      (0x1 << 1)
#define MP_TOP_CPU_PWR_STA_MASK      (0x1 << 1)
#define MP0_CPUTOP_PWR_STA_MASK      (0x1 << 2)
#define MP0_CPU0_PWR_STA_MASK        (0x1 << 3)
#define MP0_CPU1_PWR_STA_MASK        (0x1 << 4)
#define MP0_CPU2_PWR_STA_MASK        (0x1 << 5)
#define MP0_CPU3_PWR_STA_MASK        (0x1 << 6)
#define MP1_CPUTOP_PWR_STA_MASK      (0x1 << 7)
#define MP1_CPU0_PWR_STA_MASK        (0x1 << 8)
#define MP1_CPU1_PWR_STA_MASK        (0x1 << 9)
#define MP1_CPU2_PWR_STA_MASK        (0x1 << 10)
#define MP1_CPU3_PWR_STA_MASK        (0x1 << 11)
#define MP2_CPUTOP_PWR_STA_MASK      (0x1 << 12)
#define MP2_CPU0_PWR_STA_MASK        (0x1 << 13)
#define MP2_CPU1_PWR_STA_MASK        (0x1 << 14)

#define MP0_STANDBYWFI                  (1U << 2)
#define MP0_CPU0_STANDBYWFI             (1U << 3)
#define MP0_CPU1_STANDBYWFI             (1U << 4)
#define MP0_CPU2_STANDBYWFI             (1U << 5)
#define MP0_CPU3_STANDBYWFI             (1U << 6)
#define MP1_STANDBYWFI                  (1U << 7)
#define MP1_CPU0_STANDBYWFI             (1U << 8)
#define MP1_CPU1_STANDBYWFI             (1U << 9)
#define MP1_CPU2_STANDBYWFI             (1U << 10)
#define MP1_CPU3_STANDBYWFI             (1U << 11)
#define MP2_STANDBYWFI                  (1U << 12)
#define MP2_CPU0_STANDBYWFI             (1U << 13)
#define MP2_CPU1_STANDBYWFI             (1U << 14)

#define MP0_SPMC_SRAM_DORMANT_EN            (1<<0)
#define MP1_SPMC_SRAM_DORMANT_EN            (1<<1)
#define MP2_SPMC_SRAM_DORMANT_EN            (1<<2)


#define MCUSYS_PROTECTEN_SET		(INFRACFG_AO_BASE + 0x2B4)
#define MCUSYS_PROTECTEN_CLR		(INFRACFG_AO_BASE + 0x2B8)
#define MCUSYS_PROTECTEN_STA1		(INFRACFG_AO_BASE + 0x2C0)

#define PWR_RST_B                        (0x1 << 0)
#define PWR_ISO                          (0x1 << 1)
#define PWR_ON                           (0x1 << 2)
#define PWR_ON_2ND                       (0x1 << 3)
#define PWR_CLK_DIS                      (0x1 << 4)
#define SRAM_CKISO                       (0x1 << 5)
#define SRAM_ISOINT_B                    (0x1 << 6)
#define SLPB_CLAMP                       (0x1 << 7)


#define IDX_PROTECT_MP0_CACTIVE		0
#define IDX_PROTECT_ICC0_CACTIVE	1
#define IDX_PROTECT_ICD0_CACTIVE	2
#define IDX_PROTECT_L2C0_CACTIVE	3

#define IDX_PROTECT_MP1_CACTIVE		4
#define IDX_PROTECT_ICC1_CACTIVE	5
#define IDX_PROTECT_ICD1_CACTIVE	6
#define IDX_PROTECT_L2C1_CACTIVE	7

#define IDX_PROTECT_MP2_CACTIVE		8
#define IDX_PROTECT_ICC2_CACTIVE	9
#define IDX_PROTECT_ICD2_CACTIVE	10
#define IDX_PROTECT_L2C2_CACTIVE	11

#if SPMC_DVT
#define MP0_SNOOP_CTRL				(0x10395000)
#define MP1_SNOOP_CTRL				(0x10394000)
#define MP2_SNOOP_CTRL				(0x10393000)
#define MPx_SNOOP_STATUS			(0x1039000C)
#define MPx_SNOOP_ENABLE			(0x3)
#endif

static unsigned int cpu_bitmask = 1;

unsigned long read_cpuectlr(void);
void write_cpuectlr(unsigned long);

void set_cpu_retention_control(int retention_value)
{
	uint64_t cpuectlr;

	cpuectlr = read_cpuectlr();
	cpuectlr = ((cpuectlr >> 3) << 3);
	cpuectlr |= retention_value;
	write_cpuectlr(cpuectlr);
}

/*
 * SPMC Mode
 */
int spmc_init(void)
{
	int err = 0;

	/* TINFO="enable SPM register control" */
	/*mmio_write_32(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));*/

#if CONFIG_SPMC_MODE != 0
	mmio_write_32(BYPASS_SPMC, 0x0);	/* de-assert Bypass SPMC  0: SPMC mode  1: Legacy mode */
	/* udelay(200); */
	PRINTF_SPMC("[%s]change to SPMC mode !!!\n", __func__);
#endif
/* MP0 SPMC power Ctrl signals */
	mmio_write_32(MP0_CPU0_PWR_CON, mmio_read_32(MP0_CPU0_PWR_CON) & ~PWR_ON_2ND);
	mmio_write_32(MP0_CPU0_PWR_CON, mmio_read_32(MP0_CPU0_PWR_CON) | PWR_RST_B);
	mmio_write_32(MP0_CPU1_PWR_CON, mmio_read_32(MP0_CPU1_PWR_CON) & ~PWR_ON_2ND);
	mmio_write_32(MP0_CPU1_PWR_CON, mmio_read_32(MP0_CPU1_PWR_CON) | PWR_RST_B);
	mmio_write_32(MP0_CPU2_PWR_CON, mmio_read_32(MP0_CPU2_PWR_CON) & ~PWR_ON_2ND);
	mmio_write_32(MP0_CPU2_PWR_CON, mmio_read_32(MP0_CPU2_PWR_CON) | PWR_RST_B);
	mmio_write_32(MP0_CPU3_PWR_CON, mmio_read_32(MP0_CPU3_PWR_CON) & ~PWR_ON_2ND);
	mmio_write_32(MP0_CPU3_PWR_CON, mmio_read_32(MP0_CPU3_PWR_CON) | PWR_RST_B);

	mmio_write_32(MP0_CPUTOP_PWR_CON, mmio_read_32(MP0_CPUTOP_PWR_CON) & ~PWR_ON_2ND);
	mmio_write_32(MP0_CPUTOP_PWR_CON, mmio_read_32(MP0_CPUTOP_PWR_CON) | PWR_RST_B);
	mmio_write_32(MP0_CPUTOP_PWR_CON, mmio_read_32(MP0_CPUTOP_PWR_CON) & ~PWR_CLK_DIS);
/* MP1 SPMC power Ctrl signals */
	mmio_write_32(MP1_CPU0_PWR_CON, mmio_read_32(MP1_CPU0_PWR_CON) & ~PWR_ON_2ND);
	mmio_write_32(MP1_CPU0_PWR_CON, mmio_read_32(MP1_CPU0_PWR_CON) | PWR_RST_B);
	mmio_write_32(MP1_CPU1_PWR_CON, mmio_read_32(MP1_CPU1_PWR_CON) & ~PWR_ON_2ND);
	mmio_write_32(MP1_CPU1_PWR_CON, mmio_read_32(MP1_CPU1_PWR_CON) | PWR_RST_B);
	mmio_write_32(MP1_CPU2_PWR_CON, mmio_read_32(MP1_CPU2_PWR_CON) & ~PWR_ON_2ND);
	mmio_write_32(MP1_CPU2_PWR_CON, mmio_read_32(MP1_CPU2_PWR_CON) | PWR_RST_B);
	mmio_write_32(MP1_CPU3_PWR_CON, mmio_read_32(MP1_CPU3_PWR_CON) & ~PWR_ON_2ND);
	mmio_write_32(MP1_CPU3_PWR_CON, mmio_read_32(MP1_CPU3_PWR_CON) | PWR_RST_B);

	mmio_write_32(MP1_CPUTOP_PWR_CON, mmio_read_32(MP1_CPUTOP_PWR_CON) & ~PWR_ON_2ND);
	mmio_write_32(MP1_CPUTOP_PWR_CON, mmio_read_32(MP1_CPUTOP_PWR_CON) | PWR_RST_B);
	mmio_write_32(MP1_CPUTOP_PWR_CON, mmio_read_32(MP1_CPUTOP_PWR_CON) & ~PWR_CLK_DIS);

	return err;
}

int spmc_cputop_mpx_onoff(int cputop_mpx, int state, int mode)
{
	int err = 0;
	unsigned int mpx_mask = 0;
#if SPMC_DVT
	unsigned int MPx_AXI_CONFIG = 0, MPx_SNOOP_CTRL = 0;
#endif
	unsigned int MPx_CPUTOP_SPMC = 0, MPx_CPUTOP_PWR_CON = 0;
	unsigned int MPx_CPUTOP_PWR_STA_MASK = 0, MPx_CPUx_STANDBYWFI = 0;

	if (cpu_bitmask & (0xF << (cputop_mpx << 2)))
		return err;

	PRINTF_SPMC("######## %s cputop_mpx=%d state=%d mode=%d\n", __func__, cputop_mpx, state, mode);

	if (cputop_mpx == CPUTOP_MP0) {
		mpx_mask = (1 << IDX_PROTECT_ICC0_CACTIVE) |
			   (1 << IDX_PROTECT_ICD0_CACTIVE) |
			   (1 << IDX_PROTECT_MP0_CACTIVE) |
			   (1 << IDX_PROTECT_L2C0_CACTIVE);
#if SPMC_DVT
		MPx_AXI_CONFIG = MP0_AXI_CONFIG;
		MPx_SNOOP_CTRL = MP0_SNOOP_CTRL;
#endif
		MPx_CPUTOP_SPMC = MP0_CPUTOP_SPMC_CTL;
		MPx_CPUTOP_PWR_CON = MP0_CPUTOP_PWR_CON;
		MPx_CPUTOP_PWR_STA_MASK = MP0_CPUTOP_PWR_STA_MASK;
		MPx_CPUx_STANDBYWFI = MP0_STANDBYWFI;
	} else if (cputop_mpx == CPUTOP_MP1) {
		mpx_mask = (1 << IDX_PROTECT_ICC1_CACTIVE) |
			   (1 << IDX_PROTECT_ICD1_CACTIVE) |
			   (1 << IDX_PROTECT_MP1_CACTIVE) |
			   (1 << IDX_PROTECT_L2C1_CACTIVE);
#if SPMC_DVT
		MPx_AXI_CONFIG = MP1_AXI_CONFIG;
		MPx_SNOOP_CTRL = MP1_SNOOP_CTRL;
#endif
		MPx_CPUTOP_SPMC = MP1_CPUTOP_SPMC_CTL;
		MPx_CPUTOP_PWR_CON = MP1_CPUTOP_PWR_CON;
		MPx_CPUTOP_PWR_STA_MASK = MP1_CPUTOP_PWR_STA_MASK;
		MPx_CPUx_STANDBYWFI = MP1_STANDBYWFI;
	}

	/*PRINTF_SPMC("MPx_CPUTOP_PWR_STA_MASK=%x\n", MPx_CPUTOP_PWR_STA_MASK);*/
	/*PRINTF_SPMC("MPx_AXI_CONFIG=%x\n", MPx_AXI_CONFIG);*/
	/*PRINTF_SPMC("mpx_mask=%x\n", mpx_mask);*/
	/*PRINTF_SPMC("MPx_SNOOP_CTRL=%x\n", MPx_SNOOP_CTRL);*/


	if (state == STA_POWER_DOWN) {

#if SPMC_DVT			/* by callee in plat_affinst_off() */
/* ################ BUS PROTECT ENABLE and SNOOP/DVM DISABLE ################ */
		mmio_write_32(MPx_SNOOP_CTRL, mmio_read_32(MPx_SNOOP_CTRL) & ~MPx_SNOOP_ENABLE);
		PRINTF_SPMC("MPx_SNOOP_CTRL_0x%x=0x%x\n", MPx_SNOOP_CTRL, mmio_read_32(MPx_SNOOP_CTRL));
		while ((mmio_read_32(MPx_SNOOP_STATUS) & (1 << 31)) == (1 << 31))
			;
		PRINTF_SPMC("MPx_SNOOP_STATUS_0x%x=0x%x\n", MPx_SNOOP_STATUS, mmio_read_32(MPx_SNOOP_STATUS));
		/* mcsib_sw_workaround_main(); */
		/*      Program MP<n>_AXI_CONFIG acinactm to 1 */
		/*      Wait mp<n>_STANDBYWFIL2 to high */
		/* TINFO="Set acinactm = 1" */
		mmio_write_32(MPx_AXI_CONFIG, mmio_read_32(MPx_AXI_CONFIG) | MPx_AXI_CONFIG_ACINACTM);
		PRINTF_SPMC("MPx_AXI_CONFIG_0x%x=0x%x\n", MPx_AXI_CONFIG, mmio_read_32(MPx_AXI_CONFIG));
#endif
		if (mode == MODE_AUTO_SHUT_OFF) {
			/* Cluster<n>Core0 set cluster<n>.SPMC.MP<n>.sw_spark_en (mcucfg_reg) 1 */
			/*mmio_write_32(MPx_CPUTOP_SPMC, mmio_read_32(MPx_CPUTOP_SPMC) | sw_spark_en);*/
			/* Cluster<n>Core0 set cluster<n>.SPMC.MP<n>.sw sw_no_wait_for_q_channel (mcucfg_reg) 0 */
			mmio_write_32(MPx_CPUTOP_SPMC, mmio_read_32(MPx_CPUTOP_SPMC) & ~sw_no_wait_for_q_channel);
			/* Cluster<n>Core0 set cluster<n>.SPMC.MP<n>.sw_coq_dis (mcucfg_reg) 0 */
			mmio_write_32(MPx_CPUTOP_SPMC, mmio_read_32(MPx_CPUTOP_SPMC) & ~sw_coq_dis);
		} else {	/* MODE_SPMC_HW or MODE_DORMANT */
			/* Set  cluster<n>.SPMC.MP<n>.sw_no_wait_for_q_channel (mcucfg_reg) 1 */
			mmio_write_32(MPx_CPUTOP_SPMC, mmio_read_32(MPx_CPUTOP_SPMC) | sw_no_wait_for_q_channel);
			mmio_write_32(MPx_CPUTOP_SPMC, mmio_read_32(MPx_CPUTOP_SPMC) | sw_coq_dis);

			/* TINFO="Wait STANDBYWFIL2 for Cluster 0" */
			PRINTF_SPMC("CPU_IDLE_STA_0x%x=0x%x\n", CPU_IDLE_STA, mmio_read_32(CPU_IDLE_STA));
			while (!(mmio_read_32(CPU_IDLE_STA) & MPx_CPUx_STANDBYWFI))
				;
			PRINTF_SPMC("CPU_IDLE_STA_0x%x=0x%x\n", CPU_IDLE_STA, mmio_read_32(CPU_IDLE_STA));

			/* TINFO="Set ADB pwrdnreqn for Cluster 0" */
			/* mmio_write_32(INFRA_TOPAXI_PROTECTEN_1,  mmio_read_32(INFRA_TOPAXI_PROTECTEN_1)|mpx_mask); */
			mmio_write_32(MCUSYS_PROTECTEN_SET, mpx_mask);
			PRINTF_SPMC("MCUSYS_PROTECTEN_SET_0x%x=0x%x\n", MCUSYS_PROTECTEN_SET, mmio_read_32(MCUSYS_PROTECTEN_SET));
			/* TINFO="Wait ADB pwrdnreqn for Cluster 0" */
			while ((mmio_read_32(MCUSYS_PROTECTEN_STA1) & mpx_mask) != mpx_mask)
				;
			PRINTF_SPMC("MCUSYS_PROTECTEN_STA1_0x%x=0x%x\n", MCUSYS_PROTECTEN_STA1, mmio_read_32(MCUSYS_PROTECTEN_STA1));
			/* ################ BUS PROTECT ENABLE and SNOOP/DVM DISABLE ################ */
		}
		PRINTF_SPMC("MPx_CPUTOP_SPMC_0x%x=0x%x\n", MPx_CPUTOP_SPMC, mmio_read_32(MPx_CPUTOP_SPMC));
/* Set mp<n>_spmc_resetpwron_config_cputop to 0 */
/*		mmio_write_32(MPx_CPUTOP_PWR_CON, mmio_read_32(MPx_CPUTOP_PWR_CON) & ~PWR_ON_2ND);*/

		if (mode == MODE_DORMANT) {	/*  Set mp<n>_spmc_sram_dormant_en 0 */
			mmio_write_32(SPMC_DORMANT_ENABLE,
				      mmio_read_32(SPMC_DORMANT_ENABLE) | (MP0_SPMC_SRAM_DORMANT_EN
									   << cputop_mpx));
		} else {	/* MODE_SPMC_HW or MODE_AUTO_SHUT_OFF *//*     Set mp<n>_spmc_sram_dormant_en 0 */
			mmio_write_32(SPMC_DORMANT_ENABLE,
				      mmio_read_32(SPMC_DORMANT_ENABLE) & ~(MP0_SPMC_SRAM_DORMANT_EN
									    << cputop_mpx));
		}
		PRINTF_SPMC("MPx_CPUTOP_SPMC_0x%x=0x%x\n", MPx_CPUTOP_SPMC, mmio_read_32(MPx_CPUTOP_SPMC));
		/*	Set  mp<n>_spmc_pwr_on_cputop 0 */
		mmio_write_32(MPx_CPUTOP_PWR_CON, mmio_read_32(MPx_CPUTOP_PWR_CON) & ~PWR_ON);
		if (mode == MODE_SPMC_HW) {	/* TINFO="Wait until CPU_PWR_STATUS = 0" */
			while (mmio_read_32(CPU_PWR_STATUS) & MPx_CPUTOP_PWR_STA_MASK)
				;
		mmio_write_32(CPU_EXT_BUCK_ISO, mmio_read_32(CPU_EXT_BUCK_ISO) | (0x1 << cputop_mpx));
		PRINTF_SPMC("CPU_EXT_BUCK_ISO_0x%x=0x%x\n", CPU_EXT_BUCK_ISO, mmio_read_32(CPU_EXT_BUCK_ISO));
		}
		/* TINFO="Finish to turn off MP0_CPUTOP" */
	} else {
		/* TINFO="Start to turn on MP0_CPUTOP" */

		mmio_write_32(CPU_EXT_BUCK_ISO, mmio_read_32(CPU_EXT_BUCK_ISO) & ~(0x1 << cputop_mpx));/* mp2_vproc_ext_off=0(DUAL_VCORE_VCA15MPWR_ISO_AON=0)*/
		PRINTF_SPMC("CPU_EXT_BUCK_ISO_0x%x=0x%x\n", CPU_EXT_BUCK_ISO, mmio_read_32(CPU_EXT_BUCK_ISO));
		/* TINFO="Set PWR_ON = 1" */
		PRINTF_SPMC("PWR_STATUS_0x%x=0x%x\n", CPU_PWR_STATUS, mmio_read_32(CPU_PWR_STATUS));
		PRINTF_SPMC("MPx_CPUTOP_PWR_CON_0x%x=0x%x\n", MPx_CPUTOP_PWR_CON, mmio_read_32(MPx_CPUTOP_PWR_CON));
		mmio_write_32(MPx_CPUTOP_PWR_CON, mmio_read_32(MPx_CPUTOP_PWR_CON) | PWR_ON);
		PRINTF_SPMC("MPx_CPUTOP_PWR_CON_0x%x=0x%x\n", MPx_CPUTOP_PWR_CON, mmio_read_32(MPx_CPUTOP_PWR_CON));
		/* TINFO="Wait until CPU_PWR_STATUS = 1" */
		while (!(mmio_read_32(CPU_PWR_STATUS) & MPx_CPUTOP_PWR_STA_MASK))
			;
		PRINTF_SPMC("PWR_STATUS_0x%x=0x%x\n", CPU_PWR_STATUS, mmio_read_32(CPU_PWR_STATUS));

/* ################ BUS PROTECT DISABLE and SNOOP/DVM ENABLE################ */
		/* TINFO="Release bus protect" */
		mmio_write_32(MCUSYS_PROTECTEN_CLR, mpx_mask);
		PRINTF_SPMC("MCUSYS_PROTECTEN_CLR_0x%x=0x%x\n", MCUSYS_PROTECTEN_CLR, mmio_read_32(MCUSYS_PROTECTEN_CLR));

		/* TINFO="Wait ADB ~pwrdnreqn for Cluster 0" */
		while ((mmio_read_32(MCUSYS_PROTECTEN_STA1) & mpx_mask) != 0)
			;
		PRINTF_SPMC("MCUSYS_PROTECTEN_STA1_0x%x=0x%x\n", MCUSYS_PROTECTEN_STA1, mmio_read_32(MCUSYS_PROTECTEN_STA1));

#if SPMC_DVT			/* by callee in plat_affinst_off() */
/*	Program MP<n>_AXI_CONFIG acinactm to 0*/
		mmio_write_32(MPx_AXI_CONFIG, (mmio_read_32(MPx_AXI_CONFIG) & ~MPx_AXI_CONFIG_ACINACTM));
		PRINTF_SPMC("MPx_AXI_CONFIG_0x%x=0x%x\n", MPx_AXI_CONFIG, mmio_read_32(MPx_AXI_CONFIG));
		mmio_write_32(MPx_SNOOP_CTRL, mmio_read_32(MPx_SNOOP_CTRL) | MPx_SNOOP_ENABLE);
		PRINTF_SPMC("MPx_SNOOP_CTRL%x=0x%x\n", MPx_SNOOP_CTRL, mmio_read_32(MPx_SNOOP_CTRL));
		while ((mmio_read_32(MPx_SNOOP_STATUS) & (1 << 31)) == (1 << 31))
			;
		/* mcsib_sw_workaround_main(); */
		PRINTF_SPMC("MPx_SNOOP_STATUS_0x%x=0x%x\n", MPx_SNOOP_STATUS, mmio_read_32(MPx_SNOOP_STATUS));
#endif
/* ################ BUS PROTECT DISABLE and SNOOP/DVM ENABLE################ */

		/* TINFO="Finish to turn on MP0_CPUTOP" */

	}
	return err;
}

int spmc_cpu_corex_onoff(int linear_id, int state, int mode)
{
	int err = 0;
	unsigned int CPUSYSx_CPUx_SPMC_CTL, MPx_CPUx_PWR_CON, MPx_CPUx_STANDBYWFI, MPx_CPUx_PWR_STA_MASK;

	PRINTF_SPMC(">>>>>>>> %s linear_id=%d state=%d mode=%d\n", __func__, linear_id, state, mode);
	switch (linear_id) {
	case 0:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS0_CPU0_SPMC_CTL;
		MPx_CPUx_STANDBYWFI = MP0_CPU0_STANDBYWFI;
		MPx_CPUx_PWR_CON = MP0_CPU0_PWR_CON;
		MPx_CPUx_PWR_STA_MASK = MP0_CPU0_PWR_STA_MASK;
		break;
	case 1:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS0_CPU1_SPMC_CTL;
		MPx_CPUx_STANDBYWFI = MP0_CPU1_STANDBYWFI;
		MPx_CPUx_PWR_CON = MP0_CPU1_PWR_CON;
		MPx_CPUx_PWR_STA_MASK = MP0_CPU1_PWR_STA_MASK;
		break;
	case 2:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS0_CPU2_SPMC_CTL;
		MPx_CPUx_STANDBYWFI = MP0_CPU2_STANDBYWFI;
		MPx_CPUx_PWR_CON = MP0_CPU2_PWR_CON;
		MPx_CPUx_PWR_STA_MASK = MP0_CPU2_PWR_STA_MASK;
		break;
	case 3:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS0_CPU3_SPMC_CTL;
		MPx_CPUx_STANDBYWFI = MP0_CPU3_STANDBYWFI;
		MPx_CPUx_PWR_CON = MP0_CPU3_PWR_CON;
		MPx_CPUx_PWR_STA_MASK = MP0_CPU3_PWR_STA_MASK;
		break;
	case 4:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS1_CPU0_SPMC_CTL;
		MPx_CPUx_STANDBYWFI = MP1_CPU0_STANDBYWFI;
		MPx_CPUx_PWR_CON = MP1_CPU0_PWR_CON;
		MPx_CPUx_PWR_STA_MASK = MP1_CPU0_PWR_STA_MASK;
		break;
	case 5:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS1_CPU1_SPMC_CTL;
		MPx_CPUx_STANDBYWFI = MP1_CPU1_STANDBYWFI;
		MPx_CPUx_PWR_CON = MP1_CPU1_PWR_CON;
		MPx_CPUx_PWR_STA_MASK = MP1_CPU1_PWR_STA_MASK;
		break;
	case 6:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS1_CPU2_SPMC_CTL;
		MPx_CPUx_STANDBYWFI = MP1_CPU2_STANDBYWFI;
		MPx_CPUx_PWR_CON = MP1_CPU2_PWR_CON;
		MPx_CPUx_PWR_STA_MASK = MP1_CPU2_PWR_STA_MASK;
		break;
	case 7:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS1_CPU3_SPMC_CTL;
		MPx_CPUx_STANDBYWFI = MP1_CPU3_STANDBYWFI;
		MPx_CPUx_PWR_CON = MP1_CPU3_PWR_CON;
		MPx_CPUx_PWR_STA_MASK = MP1_CPU3_PWR_STA_MASK;
		break;
	default:
		ERROR("[ATF]: %s() CPU%d not exists\n", __func__, (int)linear_id);
		assert(0);
	}

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MP0_CPU0" */
		if (!(cpu_bitmask & (1 << linear_id))) {
			PRINTF_SPMC("core%d already turn off !!! cpu_bitmask=0x%x\n", linear_id, cpu_bitmask);
			return 0;
		}
		PRINTF_SPMC("CPU_PWR_STATUS=0x%x\n", mmio_read_32(CPU_PWR_STATUS));

		if (mode == MODE_AUTO_SHUT_OFF) {
			mmio_write_32(CPUSYSx_CPUx_SPMC_CTL, mmio_read_32(CPUSYSx_CPUx_SPMC_CTL) & ~cpu_sw_no_wait_for_q_channel);
			PRINTF_SPMC("CPUSYSx_CPUx_SPMC_CTL_0x%x=0x%x\n", CPUSYSx_CPUx_SPMC_CTL, mmio_read_32(CPUSYSx_CPUx_SPMC_CTL));
/*			mmio_write_32(CPUSYSx_CPUx_SPMC_CTL, mmio_read_32(CPUSYSx_CPUx_SPMC_CTL) | cpu_sw_spark_en);
			PRINTF_SPMC("CPUSYSx_CPUx_SPMC_CTL_0x%x=0x%x\n", CPUSYSx_CPUx_SPMC_CTL, mmio_read_32(CPUSYSx_CPUx_SPMC_CTL)); */
			set_cpu_retention_control(1);
		} else {
			mmio_write_32(CPUSYSx_CPUx_SPMC_CTL, mmio_read_32(CPUSYSx_CPUx_SPMC_CTL) | cpu_sw_no_wait_for_q_channel);
			PRINTF_SPMC("MPx_CPUx_PWR_CON_0x%x=0x%x\n", MPx_CPUx_PWR_CON, mmio_read_32(MPx_CPUx_PWR_CON));
			PRINTF_SPMC("CPU_IDLE_STA_0x%x=0x%x\n", CPU_IDLE_STA, mmio_read_32(CPU_IDLE_STA));
			while (!(mmio_read_32(CPU_IDLE_STA) & MPx_CPUx_STANDBYWFI))
				;
		}
		/* TINFO="Set PWR_ON = 0" */
		mmio_write_32(MPx_CPUx_PWR_CON, mmio_read_32(MPx_CPUx_PWR_CON) & ~PWR_ON);
		PRINTF_SPMC("MPx_CPUx_PWR_CON_0x%x=0x%x\n", MPx_CPUx_PWR_CON, mmio_read_32(MPx_CPUx_PWR_CON));
		/* TINFO="Wait until CPU_PWR_STATUS = 0 */
		if (mode == MODE_SPMC_HW) {
			while (mmio_read_32(CPU_PWR_STATUS) & MPx_CPUx_PWR_STA_MASK)
				;
			PRINTF_SPMC("PWR_STATUS_0x%x=0x%x\n", CPU_PWR_STATUS, mmio_read_32(CPU_PWR_STATUS));
		}
		cpu_bitmask &= ~(1 << linear_id);
		PRINTF_SPMC("cpu_bitmask=0x%x\n", cpu_bitmask);
		PRINTF_SPMC("CPU_PWR_STATUS=0x%x\n", mmio_read_32(CPU_PWR_STATUS));
		/* TINFO="Finish to turn off MP0_CPU0" */
	} else {
		/* TINFO="Start to turn on MP0_CPU0" */
		/* TINFO="Set PWR_ON = 1" */
		PRINTF_SPMC("PWR_STATUS_0x%x=0x%x\n", CPU_PWR_STATUS, mmio_read_32(CPU_PWR_STATUS));
		/*mmio_write_32(MPx_CPUx_PWR_CON,  mmio_read_32(MPx_CPUx_PWR_CON) | PWR_RST_B); */
		PRINTF_SPMC("MPx_CPUx_PWR_CON_0x%x=0x%x\n", MPx_CPUx_PWR_CON, mmio_read_32(MPx_CPUx_PWR_CON));
		mmio_write_32(MPx_CPUx_PWR_CON, mmio_read_32(MPx_CPUx_PWR_CON) | PWR_ON);
		PRINTF_SPMC("MPx_CPUx_PWR_CON_0x%x=0x%x\n", MPx_CPUx_PWR_CON, mmio_read_32(MPx_CPUx_PWR_CON));
		/* TINFO="Wait until CPU_PWR_STATUS = 1 and CPU_PWR_STATUS_2ND = 1" */
		while ((mmio_read_32(CPU_PWR_STATUS) & MPx_CPUx_PWR_STA_MASK) != MPx_CPUx_PWR_STA_MASK)
			;
/*		mmio_write_32(CPUSYSx_CPUx_SPMC_CTL, mmio_read_32(CPUSYSx_CPUx_SPMC_CTL)| cpu_sw_spark_en);*/
		PRINTF_SPMC("PWR_STATUS_0x%x=0x%x\n", CPU_PWR_STATUS, mmio_read_32(CPU_PWR_STATUS));
		cpu_bitmask |= (1 << linear_id);
		PRINTF_SPMC("cpu_bitmask=0x%x\n", cpu_bitmask);
#if SPMC_SPARK2
		if (linear_id >= 4) {
			little_spark2_setldo(linear_id);
			little_spark2_core_enable(linear_id, 1);
		}
#endif
		/* TINFO="Finish to turn on MP0_CPU0" */
		PRINTF_SPMC("[AT] PowerOn CPU %d successfully\n", linear_id);
	}
	return err;
}

int little_spark2_setldo(unsigned int linear_id)
{
	unsigned long long base_vret;
	unsigned int offset, tmp, sparkvretcntrl = 0x34;	/* 34=.5 3f=.6 */

	if (linear_id >= PLATFORM_CORE_COUNT)
		return -1;
	/* PRINTF_SPMC("%s sparkvretcntrl=%x\n", __func__, sparkvretcntrl); */
	if (linear_id < 4) {
		offset = linear_id;
		base_vret = CPUSYS0_SPARKVRETCNTRL;
		tmp = (mmio_read_32(base_vret) & ~((0x3f) << (offset << 3))) | (sparkvretcntrl << (offset << 3));
		mmio_write_32(base_vret, tmp);
	} else {
		offset = linear_id - 4;
		base_vret = CPUSYS1_SPARKVRETCNTRL;
		tmp = (mmio_read_32(base_vret) & ~((0x3f) << (offset << 3))) | (sparkvretcntrl << (offset << 3));
		mmio_write_32(base_vret, tmp);
	}
	return 0;
}

int little_spark2_core_enable(unsigned int linear_id, unsigned int sw)
{
	unsigned int CPUSYSx_CPUx_SPMC_CTL, tmp;

	if (linear_id > 9 || sw > 1)
		return -1;

	switch (linear_id) {
	case 0:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS0_CPU0_SPMC_CTL;
		break;
	case 1:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS0_CPU1_SPMC_CTL;
		break;
	case 2:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS0_CPU2_SPMC_CTL;
		break;
	case 3:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS0_CPU3_SPMC_CTL;
		break;
	case 4:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS1_CPU0_SPMC_CTL;
		break;
	case 5:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS1_CPU1_SPMC_CTL;
		break;
	case 6:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS1_CPU2_SPMC_CTL;
		break;
	case 7:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS1_CPU3_SPMC_CTL;
		break;
	case 8:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS2_CPU0_SPMC_CTL;
		break;
	case 9:
		CPUSYSx_CPUx_SPMC_CTL = CPUSYS2_CPU1_SPMC_CTL;
		break;
	default:
		ERROR("%s() CPU%d not exists\n", __func__, (int)linear_id);
		assert(0);
	}
	tmp = (mmio_read_32(CPUSYSx_CPUx_SPMC_CTL) & ~cpu_sw_spark_en) | sw;
	mmio_write_32(CPUSYSx_CPUx_SPMC_CTL, tmp);
	return 0;
}
void SPARK2_reinit(void)
{
#if SPMC_SPARK2
	unsigned long linear_id = platform_get_core_pos(read_mpidr());
	if (linear_id >= 4) {
		little_spark2_setldo(linear_id);
		little_spark2_core_enable(linear_id, 1);
		set_cpu_retention_control(1);
	}
#endif
}

