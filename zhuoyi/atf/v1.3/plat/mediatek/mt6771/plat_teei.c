/*
 * Copyright (c) 2015-2017 MICROTRUST Incorporated
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 * Neither the name of MICROTRUST nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/

#if (defined MICROTRUST_TEE_VERSION && MICROTRUST_TEE_VERSION >= 270)

#include <assert.h>
#include <debug.h>
#include <string.h>

#include "plat_teei.h"
#include <arch_helpers.h>
#include <fiq_smp_call.h>
#include <bl_common.h>
#include <gic_v2.h>
#include <gic_v3.h>
#include <stdio.h>
#include <mtk_aee_debug.h>
#include <mt_gic_v3.h>
#include <runtime_svc.h>
#include <platform_def.h>
#include <rng.h>
#include "teei_private.h"
#include "teei_atf.h"
static struct tee_arg_t *teeiBootCfg;
static struct tee_keys_t *teeiKeys;

uint32_t spi_apc_num;

unsigned int sec_exc[8];
unsigned int nsec_exc[8];
#define DRM_SEC_EXC_CNT 8
unsigned int drm_sec_exc[DRM_SEC_EXC_CNT] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned int drm_sec_enable[DRM_SEC_EXC_CNT] = {0, 0, 0, 0, 0, 0, 0, 0};
#define SZ_64K                          (0x00010000)


unsigned int SEC_EXC_CNT;
unsigned int NSEC_EXC_CNT;

unsigned int NSEC_TIMER;	/* 30 */
unsigned int NSEC_UART;		/* 124 */

unsigned int SEC_TIMER;		/* 205 */
/*~ unsigned int SEC_TP; */    /* 189 */
unsigned int SEC_SPI = 132;

unsigned int SEC_APP_INTR;
unsigned int SEC_DRV_INTR;
unsigned int SEC_RDRV_INTR;
unsigned int SEC_TDUMP_INTR;

unsigned int gicd_get_isenabler(unsigned int base, unsigned int id)
{
	unsigned bit_num = id & ((1 << ISENABLER_SHIFT) - 1);
	unsigned int reg_val = gicd_read_isenabler(base, id);

	return (reg_val >> bit_num) & 0x1;
}

#ifndef LEGACY_GIC_SUPPORT
static void config_ns_group1(int enable)
{
	unsigned int scr_val = 0;

	scr_val = read_scr();
	write_scr(scr_val | SCR_NS_BIT);
	isb();

	if (enable)
		gicc_write_grpen1_el1(0x1);
	else
		gicc_write_grpen1_el1(0x0);

	isb();

	write_scr(scr_val);
	isb();
}
#endif

void disable_group(unsigned int grp)
{
#ifndef LEGACY_GIC_SUPPORT
	/* only support for group 1 now */
	if (grp != 1)
		return;

	config_ns_group1(0);
#else
	unsigned int  pre;

	pre = gicd_v3_read_ctlr(BASE_GICD_BASE);
	pre = pre & (~(1<<grp));
	gicd_v3_write_ctlr(BASE_GICD_BASE, pre);
	gicd_v3_do_wait_for_rwp(BASE_GICD_BASE);
#endif
}

void enable_group(unsigned int grp)
{
#ifndef LEGACY_GIC_SUPPORT
	/* only support for group 1 now */
	if (grp != 1)
		return;

	config_ns_group1(1);
#else
	unsigned int  pre;

	pre = gicd_v3_read_ctlr(BASE_GICD_BASE);
	pre = pre | (1<<grp);
	gicd_v3_write_ctlr(BASE_GICD_BASE, pre);
	gicd_v3_do_wait_for_rwp(BASE_GICD_BASE);
#endif
}


void  disable_ns_exc_intr(unsigned int gicd_base)
{
	unsigned int index;

	for (index = 0; index < NSEC_EXC_CNT; index++) {
		gicd_set_icenabler(gicd_base, nsec_exc[index]);
		gicd_v3_do_wait_for_rwp(gicd_base);
	}
}

void  enable_ns_exc_intr(unsigned int gicd_base)
{
	unsigned int index;

	for (index = 0; index < NSEC_EXC_CNT; index++) {
		gicd_set_isenabler(gicd_base,  nsec_exc[index]);
		gicd_v3_do_wait_for_rwp(gicd_base);
	}
}

void  disable_s_exc_intr(unsigned int gicd_base)
{
	unsigned int index;

	for (index = 0; index < SEC_EXC_CNT; index++) {
		gicd_set_icenabler(gicd_base, sec_exc[index]);
		gicd_v3_do_wait_for_rwp(gicd_base);
	}
	for (index = 0;  index < DRM_SEC_EXC_CNT; index++) {
		if (drm_sec_exc[index] != 0) {
			if (drm_sec_exc[index] < 32) {
				unsigned int rdist_sgi_base = 0;

				if (gic_populate_rdist(&rdist_sgi_base) == -1)
					return;
				rdist_sgi_base += SZ_64K;
				drm_sec_enable[index] = gicd_get_isenabler(rdist_sgi_base,  drm_sec_exc[index]);
				gicd_set_icenabler(rdist_sgi_base, drm_sec_exc[index]);
				gicd_v3_do_wait_for_rwp(rdist_sgi_base);
			} else {
				drm_sec_enable[index] = gicd_get_isenabler(gicd_base,  drm_sec_exc[index]);
				gicd_set_icenabler(gicd_base, drm_sec_exc[index]);
				gicd_v3_do_wait_for_rwp(gicd_base);
			}
		}
	}
}

void  enable_s_exc_intr(unsigned int gicd_base)
{
	unsigned int index;

	for (index = 0; index < SEC_EXC_CNT; index++) {
		gicd_set_isenabler(gicd_base,  sec_exc[index]);
		gicd_v3_set_irouter(gicd_base, sec_exc[index], gic_mpidr_to_affinity(read_mpidr()));
		gicd_v3_do_wait_for_rwp(gicd_base);

	}
	for (index = 0;  index < DRM_SEC_EXC_CNT; index++) {
		if (drm_sec_exc[index] != 0 && drm_sec_enable[index]) {
			if (drm_sec_exc[index] < 32) {
				unsigned int rdist_sgi_base = 0;

				if (gic_populate_rdist(&rdist_sgi_base) == -1)
					return;
				rdist_sgi_base += SZ_64K;
				gicd_set_isenabler(rdist_sgi_base, drm_sec_exc[index]);
				gicd_v3_do_wait_for_rwp(rdist_sgi_base);
			} else {
				gicd_set_isenabler(gicd_base,  drm_sec_exc[index]);
				gicd_v3_set_irouter(gicd_base, drm_sec_exc[index], gic_mpidr_to_affinity(read_mpidr()));
				gicd_v3_do_wait_for_rwp(gicd_base);
			}
		}
	}
}

DEFINE_SYSREG_RW_FUNCS(cntkctl_el1)
#define enable_cntkctl_el0vten(x)  (x |= (EL0VTEN_BIT))
#define disable_cntkctl_el0vten(x)  (x &= (~EL0VTEN_BIT))
int drm_config_virtual_timer_el1_rw(int rw)
{
	unsigned int ctl = 0;

	if (rw) {
		ctl = read_cntkctl_el1();
		enable_cntkctl_el0vten(ctl);
		write_cntkctl_el1(ctl);
	} else {
		ctl = read_cntkctl_el1();
		disable_cntkctl_el0vten(ctl);
		write_cntkctl_el1(ctl);
	}
	return 0;
}
/* gicr */
unsigned int enable_ppi_fiq(unsigned int fiq)
{
	unsigned int base = 0;

	/* get the base of redistributor first */
	if (gic_populate_rdist(&base) == -1)
		return 0;

	/* the SGI/PPI base is at 64K offset of current rdist base */
	base += SZ_64K;
#if !defined(SPD_trusty)
	/* set all SGI/PPI as NS-group1 by default */
	gicd_write_igroupr(base, 0, ~0);
#endif
	/* set fiq smp call as fiq */
	gicd_clr_igroupr(base, fiq);
	gicd_set_ipriorityr(base, fiq, GIC_HIGHEST_SEC_PRIORITY);
	gicd_set_isenabler(base, fiq);
	gicd_v3_do_wait_for_rwp(base);
	return base;
}
unsigned int disable_ppi_fiq(unsigned int fiq)
{
	unsigned int base = 0;

	/* get the base of redistributor first */
	if (gic_populate_rdist(&base) == -1)
		return 0;

	/* the SGI/PPI base is at 64K offset of current rdist base */
	base += SZ_64K;
	/* clr fiq smp call as fiq */
	gicd_set_igroupr(base, fiq);
	gicd_set_ipriorityr(base, fiq, GIC_HIGHEST_NS_PRIORITY);
	gicd_set_icenabler(base, fiq);
	gicd_v3_do_wait_for_rwp(base);
	return base;
}
int set_drm_int_secure(unsigned int id)
{
	int state = 1;
	unsigned int index;

	for (index = 0; index < DRM_SEC_EXC_CNT; index++) {
		if (drm_sec_exc[index] == 0) {
			drm_sec_exc[index] = id;
			state = 0;
			break;
		}
	}
	if (id < 32)
		goto set_gicr;

	gicd_set_icenabler(BASE_GICD_BASE,  id);
	gicd_clr_igroupr(BASE_GICD_BASE, id);
	gicd_v3_set_irouter(BASE_GICD_BASE, id, gic_mpidr_to_affinity(read_mpidr()));
	gicd_set_isenabler(BASE_GICD_BASE,  id);
	gicd_v3_do_wait_for_rwp(BASE_GICD_BASE);
	return state;
set_gicr:
	/* gicr */
	enable_ppi_fiq(id);
	return state;
}
int set_drm_int_nonsecure(unsigned int id)
{
	int state = 1;
	unsigned int index;

	for (index = 0; index < DRM_SEC_EXC_CNT; index++) {
		if (drm_sec_exc[index] == id)
			drm_sec_exc[index] = 0;

	}
	if (id < 32)
		goto set_gicr;

	gicd_set_icenabler(BASE_GICD_BASE,  id);
	gicd_set_igroupr(BASE_GICD_BASE, id);
	/***********************************************************
	 * gicd_v3_set_irouter(gicd_base, id,gic_mpidr_to_affinity(read_mpidr()));
	//not sure if it is OK
	*************************************************************/
	gicd_v3_set_irouter(BASE_GICD_BASE, id, GICD_V3_IROUTER_SPI_MODE_ANY);
	gicd_set_isenabler(BASE_GICD_BASE,  id);
	gicd_v3_do_wait_for_rwp(BASE_GICD_BASE);
	return state;
set_gicr:
	/* gicr */
	disable_ppi_fiq(id);
	return state;
}


void prepare_gic_for_nsec_boot(void)
{
	disable_s_exc_intr(BASE_GICD_BASE);
}

unsigned int get_irq_target(unsigned int irq)
{
	return gicd_read_itargetsr(BASE_GICD_BASE, irq);
}

void prepare_gic_for_sec_boot(void)
{
	unsigned int index;

	for (index = 0;  index < SEC_EXC_CNT; index++)
		gicd_set_icenabler(BASE_GICD_BASE,  sec_exc[index]);
}


void migrate_gic_context(uint32_t secure_state)
{
	if (secure_state == SECURE) {

		disable_ns_exc_intr(BASE_GICD_BASE);
		if (TEEI_STATE < TEEI_BUF_READY)
			disable_group(1);

		enable_s_exc_intr(BASE_GICD_BASE);
	} else {
		enable_ns_exc_intr(BASE_GICD_BASE);
		disable_s_exc_intr(BASE_GICD_BASE);
		if (TEEI_STATE < TEEI_BUF_READY)
			enable_group(1);
	}
}

void trigger_soft_intr(unsigned int id)
{
	gicd_v3_set_irouter(BASE_GICD_BASE, id, gic_mpidr_to_affinity(read_mpidr()));
	gicd_set_ispendr(BASE_GICD_BASE, id);
}
#if 0
void dump_gic_all_setting(void)
{
	unsigned int gicd_base, gicc_base;

	gicd_base = BASE_GICD_BASE;
	gicc_base = BASE_GICC_BASE;

	gic_print_all_setting(gicd_base, gicc_base);
}
#endif

void sec_exc_add(unsigned int intr_num)
{
	sec_exc[SEC_EXC_CNT] = intr_num;
	SEC_EXC_CNT++;
}

void nsec_exc_add(unsigned int intr_num)
{
	nsec_exc[NSEC_EXC_CNT] = intr_num;
	NSEC_EXC_CNT++;
}

void teei_init_interrupt(void)
{
	int i;

	SEC_EXC_CNT = 0;
	NSEC_EXC_CNT = 0;

	teeiBootCfg = (struct tee_arg_t *)(uintptr_t)TEEI_BOOT_PARAMS;
	teeiKeys =  (struct tee_keys_t *)(uintptr_t)TEEI_SECURE_PARAMS;

	for (i = 0; i < 5; i++) {
		if (teeiBootCfg->tee_dev[i].dev_type == MT_UART16550) {
			uart_apc_num = teeiBootCfg->tee_dev[i].apc_num;
			NSEC_UART =   teeiBootCfg->tee_dev[i].intr_num;
		}
		if (teeiBootCfg->tee_dev[i].dev_type == MT_SEC_GPT)
			SEC_TIMER = teeiBootCfg->tee_dev[i].intr_num;
	}
	SEC_APP_INTR = teeiBootCfg->ssiq_number[0];
	SEC_DRV_INTR = teeiBootCfg->ssiq_number[1];
	SEC_RDRV_INTR = teeiBootCfg->ssiq_number[2];
	SEC_TDUMP_INTR = teeiBootCfg->ssiq_number[3];

	/* FIXME: Make it can be configured */
	NSEC_TIMER =  30;

	/* init secure exlusive array */
	sec_exc_add(SEC_TIMER);
	sec_exc_add(SEC_APP_INTR);
	sec_exc_add(SEC_DRV_INTR);
	sec_exc_add(SEC_RDRV_INTR);
	sec_exc_add(SEC_TDUMP_INTR);
	/* sec_exc_add(SEC_SPI); */
#ifdef TUI_SUPPORT
	sec_exc_add(206);
	sec_exc_add(SEC_TUI_CANCEL);

#endif
	nsec_exc_add(NSEC_TIMER);
}

unsigned int teei_get_fp_id(void)
{
	if (teeiBootCfg->tee_dev[4].dev_type == 5)
		return teeiBootCfg->tee_dev[4].base_addr;

	return 0;
}

void teei_gic_setup(void)
{
	teei_init_interrupt();
	/************************************************************************
	 * set interrupt to group 0
	 * set GIC_HIGHEST_SEC_PRIORITY
	 * set itarget to current cpu
	 * use 1-N model, means as long as one of N core can handle, this will be handled
	 * set gic edge sensitive via GICD_ICFG
	 * set low polarity
	 * disable interrupt
	 **************************************************************************/
	for (unsigned int index = 0;  index < SEC_EXC_CNT; index++) {
		gicd_clr_igroupr(BASE_GICD_BASE, sec_exc[index]);
		gicd_set_ipriorityr(BASE_GICD_BASE,  sec_exc[index], GIC_HIGHEST_SEC_PRIORITY);
		gicd_v3_set_irouter(BASE_GICD_BASE, sec_exc[index], gic_mpidr_to_affinity(read_mpidr()));
		mt_irq_set_sens(BASE_GICD_BASE,  sec_exc[index], MT_EDGE_SENSITIVE);
		mt_irq_set_polarity(sec_exc[index], MT_POLARITY_HIGH); /* HIGH */
		gicd_set_icenabler(BASE_GICD_BASE,  sec_exc[index]);
#ifdef TUI_SUPPORT
		gicd_set_icpendr(BASE_GICD_BASE, 206);
#endif

	}
}

void teei_triggerSgiDump(void)
{
	uint64_t mpidr;
	uint32_t linear_id;

	/* send to all cpus except the current one */
	mpidr = read_mpidr();
	linear_id = platform_get_core_pos(mpidr);
	fiq_smp_call_function(0x3FF & ~(1 << linear_id), (inter_cpu_call_func_t)aee_wdt_dump, 0, 0);
	aee_wdt_dump(NULL);
}

void teei_ack_gic(void)
{
	uint32_t iar;

	iar = gicc_read_IAR(BASE_GICC_BASE);
	gicc_write_EOIR(BASE_GICC_BASE, iar);
}
uint32_t teei_rng(void)
{
	uint32_t value = 0;

	return plat_get_rnd(&value);
}

int s_wdt_lock;
uint64_t teei_fiq_handler(uint32_t id, uint32_t flags, void *handle, void *cookie)
{
	/***********************************************************
	* uint64_t mpidr = read_mpidr();
	* uint32_t linear_id = platform_get_core_pos(mpidr);
	* struct teei_context *teei_ctx = &secure_context[linear_id];
	**********************************************************/
	int caller_security_state = flags & 1;

	if (caller_security_state == SECURE)
		SMC_RET1(handle, SMC_UNK);

	switch (id) {
	case FIQ_SMP_CALL_SGI: {
			plat_ic_acknowledge_interrupt();
			fiq_icc_isr();
			plat_ic_end_of_interrupt(FIQ_SMP_CALL_SGI);
			SMC_RET0(handle);
			break;
		}
	case WDT_IRQ_BIT_ID: {
			/* if disable group 0 other core will not receive sgi */
			int lockval = 0;
			int tmp = 0;

			plat_ic_acknowledge_interrupt();
			__asm__ volatile(
				"1: ldxr  %w0, [%2]\n"
				" add %w0, %w0, %w3\n"
				" stxr  %w1, %w0, [%2]\n"
				" cbnz  %w1, 1b"
					: "=&r" (lockval),  "=&r" (tmp)
					: "r" (&(s_wdt_lock)), "Ir" (1)
					: "cc");
			if (s_wdt_lock == 1)
				teei_triggerSgiDump();

			plat_ic_end_of_interrupt(WDT_IRQ_BIT_ID);
			SMC_RET0(handle);
			break;
		}
	default:
		SMC_RET0(handle);
	}

	return 0;
}

#else
/* MICROTRUST_TEE_VERSION == 251 */

#include <assert.h>
#include <debug.h>
#include <string.h>

#include <platform.h>
#include "plat_private.h"
#include <arch_helpers.h>
#include <fiq_smp_call.h>
#include <bl_common.h>
#include "teei_private.h"
#include <gic_v2.h>
#include <gic_v3.h>
#include <stdio.h>
#include <mtk_aee_debug.h>
#include "plat_teei.h"
#include <mt_gic_v3.h>
#include <platform_def.h>
#include <rng.h>

void gicd_v3_do_wait_for_rwp(unsigned int gicd_base);
/* Initialization parameters for teei. */
static tee_arg_t_ptr teeiBootCfg;
static tee_keys_t_ptr teeiKeys;

extern uint32_t uart_apc_num;
extern uint32_t spi_apc_num;
extern uint32_t TEEI_STATE;

unsigned int sec_exc[8];
#define  SUPPORT_DRM 0

#ifdef SUPPORT_DRM
#define DRM_SEC_EXC_CNT 8
unsigned int drm_sec_exc[DRM_SEC_EXC_CNT] = {0, 0, 0, 0, 0, 0, 0, 0};
#endif

unsigned int nsec_exc[8];


unsigned int SEC_EXC_CNT;
unsigned int NSEC_EXC_CNT;

unsigned int NSEC_TIMER;	/* 30 */
unsigned int NSEC_UART;		/* 124 */

unsigned int SEC_TIMER;		/* 205 */
/*~ unsigned int SEC_TP; */    /* 189 */
unsigned int SEC_SPI = 132;

unsigned int SEC_APP_INTR;
unsigned int SEC_DRV_INTR;
unsigned int SEC_RDRV_INTR;
unsigned int SEC_TDUMP_INTR;

uint64_t gic_mpidr_to_affinity(uint64_t mpidr);
int gic_populate_rdist(unsigned int *rdist_base);

int gic_populate_rdist(unsigned int *rdist_base);
void irq_raise_softirq(unsigned int map, unsigned int irq);

static void config_ns_group1(int enable)
{
	unsigned int scr_val = 0;

	scr_val = read_scr();
	write_scr(scr_val | SCR_NS_BIT);
	isb();

	if (enable)
		gicc_write_grpen1_el1(0x1);
	else
		gicc_write_grpen1_el1(0x0);

	isb();

	write_scr(scr_val);
	isb();
}

void disable_group(unsigned int grp)
{
	/* only support for group 1 now */
	if (grp != 1)
		return;

	config_ns_group1(0);
}

void enable_group(unsigned int grp)
{
	/* only support for group 1 now */
	if (grp != 1)
		return;

	config_ns_group1(1);
}


void  disable_ns_exc_intr(unsigned int gicd_base)
{
	unsigned int index;

	for (index = 0; index < NSEC_EXC_CNT; index++) {
		gicd_set_icenabler(gicd_base, nsec_exc[index]);
		gicd_v3_do_wait_for_rwp(gicd_base);
	}
}

void  enable_ns_exc_intr(unsigned int gicd_base)
{
	unsigned int index;

	for (index = 0; index < NSEC_EXC_CNT; index++) {
		gicd_set_isenabler(gicd_base,  nsec_exc[index]);
		gicd_v3_do_wait_for_rwp(gicd_base);
	}
}

void  disable_s_exc_intr(unsigned int gicd_base)
{
	unsigned int index;

	for (index = 0; index < SEC_EXC_CNT; index++) {
		gicd_set_icenabler(gicd_base, sec_exc[index]);
		gicd_v3_do_wait_for_rwp(gicd_base);
	}
#ifdef SUPPORT_DRM
	for (index = 0; index < DRM_SEC_EXC_CNT; index++) {
		if (drm_sec_exc[index] != 0) {
			gicd_set_icenabler(gicd_base, drm_sec_exc[index]);
			gicd_v3_do_wait_for_rwp(gicd_base);
		}
	}
#endif
}

void  enable_s_exc_intr(unsigned int gicd_base)
{
	unsigned int index;
	for (index = 0; index < SEC_EXC_CNT; index++) {
		gicd_set_isenabler(gicd_base,  sec_exc[index]);
		gicd_v3_set_irouter(gicd_base, sec_exc[index],\
		                    gic_mpidr_to_affinity(read_mpidr()));
		gicd_v3_do_wait_for_rwp(gicd_base);

	}
#ifdef SUPPORT_DRM
	for (index = 0; index < DRM_SEC_EXC_CNT; index++) {
		if (drm_sec_exc[index] != 0) {
			gicd_set_isenabler(gicd_base,  drm_sec_exc[index]);
			gicd_v3_set_irouter(gicd_base, drm_sec_exc[index],\
					gic_mpidr_to_affinity(read_mpidr()));
			gicd_v3_do_wait_for_rwp(gicd_base);
		}
	}
#endif
}

#ifdef SUPPORT_DRM
int set_drm_int_secure(unsigned int id)
{
	int state = 1;
	unsigned int index;
	gicd_set_icenabler(BASE_GICD_BASE, id);
	gicd_clr_igroupr(BASE_GICD_BASE, id);
	gicd_v3_set_irouter(BASE_GICD_BASE, id, gic_mpidr_to_affinity(read_mpidr()));
	gicd_set_isenabler(BASE_GICD_BASE, id);
	gicd_v3_do_wait_for_rwp(BASE_GICD_BASE);
	for (index = 0 ; index < DRM_SEC_EXC_CNT ; index++) {
		if (drm_sec_exc[index] == 0) {
			drm_sec_exc[index] = id;
			state = 0;
			break;
		}
	}
	return state;
}
#endif

#ifdef SUPPORT_DRM
int set_drm_int_nonsecure(unsigned int id)
{
	int state = 1;
	unsigned int index;
	for (index = 0 ; index < DRM_SEC_EXC_CNT ; index++) {
		if( drm_sec_exc[index] == id) {
			drm_sec_exc[index] = 0;
			/* break; */
		}
	}

	gicd_set_icenabler(BASE_GICD_BASE,  id);
	gicd_set_igroupr(BASE_GICD_BASE, id);
	/* gicd_v3_set_irouter(gicd_base, id,gic_mpidr_to_affinity(read_mpidr())); */
	/* not sure if it is OK */
	gicd_v3_set_irouter(BASE_GICD_BASE, id, GICD_V3_IROUTER_SPI_MODE_ANY);
	gicd_set_isenabler(BASE_GICD_BASE,  id);
	gicd_v3_do_wait_for_rwp(BASE_GICD_BASE);
	return state;
}
#endif


void prepare_gic_for_nsec_boot(void)
{
	unsigned int gicd_base;

	gicd_base = BASE_GICD_BASE;
	disable_s_exc_intr(gicd_base);
}
unsigned int get_irq_target(unsigned int irq)
{
	unsigned int gicd_base;

	gicd_base = BASE_GICD_BASE;
	unsigned int val =   gicd_read_itargetsr(gicd_base, irq);
	return val;
}

void prepare_gic_for_sec_boot(void)
{
	unsigned int gicd_base, index;

	gicd_base = BASE_GICD_BASE;
	for (index = 0;  index < SEC_EXC_CNT; index++)
		gicd_set_icenabler(gicd_base,  sec_exc[index]);
}


void migrate_gic_context(uint32_t secure_state)
{
	unsigned int gicd_base, index;
	/*unsigned int val; */

	gicd_base = BASE_GICD_BASE;

	if (secure_state == SECURE) {

		disable_ns_exc_intr(gicd_base);
		/* enable_s_exc_intr(gicd_base); */
		if (TEEI_STATE < TEEI_BUF_READY)
			disable_group(1);

		for (index = 0 ; index < SEC_EXC_CNT ; index++) {
			/* val = gicd_read_itargetsr(gicd_base, sec_exc[index]); */
			gicd_v3_set_irouter(gicd_base, sec_exc[index],\
								gic_mpidr_to_affinity(read_mpidr()));
			gicd_set_isenabler(gicd_base,  sec_exc[index]);
		}
#ifdef SUPPORT_DRM
		for (index = 0 ; index < DRM_SEC_EXC_CNT ; index++) {
			if (drm_sec_exc[index] != 0) {
				gicd_read_itargetsr(gicd_base, drm_sec_exc[index]);
				gicd_v3_set_irouter(gicd_base, drm_sec_exc[index],\
									gic_mpidr_to_affinity(read_mpidr()));
				gicd_set_isenabler(gicd_base,  drm_sec_exc[index]);
			}
		}
#endif
	} else {
		enable_ns_exc_intr(gicd_base);
		disable_s_exc_intr(gicd_base);
		if (TEEI_STATE < TEEI_BUF_READY)
			enable_group(1);
	}
}

void trigger_soft_intr(unsigned int id)
{
#if 0
	unsigned int val;

	val = gicd_read_itargetsr(BASE_GICD_BASE, id);
#endif
	gicd_v3_set_irouter(BASE_GICD_BASE, id, gic_mpidr_to_affinity(read_mpidr()));
	gicd_set_ispendr(BASE_GICD_BASE, id);
}
/*
void dump_gic_all_setting(void)
{
	unsigned int gicd_base, gicc_base;

	gicd_base = BASE_GICD_BASE;
	gicc_base = BASE_GICC_BASE;

	gic_print_all_setting(gicd_base, gicc_base);
}
*/

void sec_exc_add(unsigned int intr_num)
{
	sec_exc[SEC_EXC_CNT] = intr_num;
	SEC_EXC_CNT++;
}

void nsec_exc_add(unsigned int intr_num)
{
	nsec_exc[NSEC_EXC_CNT] = intr_num;
	NSEC_EXC_CNT++;
}

void teei_init_interrupt(void)
{
	int i;

	SEC_EXC_CNT = 0;
	NSEC_EXC_CNT = 0;

	teeiBootCfg = (tee_arg_t_ptr)(uintptr_t)TEEI_BOOT_PARAMS;
	teeiKeys =  (tee_keys_t_ptr)(uintptr_t)TEEI_SECURE_PARAMS;

	for (i = 0; i < 5; i++) {
		if (teeiBootCfg->tee_dev[i].dev_type == MT_UART16550) {
			uart_apc_num = teeiBootCfg->tee_dev[i].apc_num;
			NSEC_UART =   teeiBootCfg->tee_dev[i].intr_num;
		}
		if (teeiBootCfg->tee_dev[i].dev_type == MT_SEC_GPT)
			SEC_TIMER = teeiBootCfg->tee_dev[i].intr_num;
	}
	SEC_APP_INTR = teeiBootCfg->ssiq_number[0];
	SEC_DRV_INTR = teeiBootCfg->ssiq_number[1];
	SEC_RDRV_INTR = teeiBootCfg->ssiq_number[2];
	SEC_TDUMP_INTR = teeiBootCfg->ssiq_number[3];

	/* FIXME: Make it can be configured */
	NSEC_TIMER =  30;

	/* init secure exlusive array */
	sec_exc_add(SEC_TIMER);
	sec_exc_add(SEC_APP_INTR);
	sec_exc_add(SEC_DRV_INTR);
	sec_exc_add(SEC_RDRV_INTR);
	sec_exc_add(SEC_TDUMP_INTR);
	/* sec_exc_add(SEC_SPI); */
	/* spec_intr_add(SEC_TP); */

	/*init non secure exlusive array*/
	nsec_exc_add(NSEC_TIMER);
}

unsigned int teei_get_fp_id(void)
{
	if (teeiBootCfg->tee_dev[4].dev_type == 5)
		return teeiBootCfg->tee_dev[4].base_addr;

	return 0;
}

void teei_gic_setup(void)
{
	unsigned int gicd_base;

	gicd_base = BASE_GICD_BASE;

	teei_init_interrupt();

	/* Configure  secure interrupts now */
	for (unsigned int index = 0; index < SEC_EXC_CNT; index++) {
		/*	set this interrupt to group 0*/
		gicd_clr_igroupr(gicd_base, sec_exc[index]);
		/*set this interrupt GIC_HIGHEST_SEC_PRIORITY*/
		gicd_set_ipriorityr(gicd_base,  sec_exc[index], GIC_HIGHEST_SEC_PRIORITY);
		/*set itarget to current cpu */
		/* use 1-N model, means as long as one of N core can handle, this will be handled */
		gicd_v3_set_irouter(gicd_base, sec_exc[index], gic_mpidr_to_affinity(read_mpidr()));
		/*set gic edge sensitive via GICD_ICFG*/
		mt_irq_set_sens(gicd_base,  sec_exc[index], MT_EDGE_SENSITIVE);
		/*set low polarity*/
		mt_irq_set_polarity(sec_exc[index], MT_POLARITY_HIGH); /* HIGH */
		/*disable  this interrupt in gicd*/
		gicd_set_icenabler(gicd_base,  sec_exc[index]);
	}
}

uint32_t teei_rng(void)
{
	uint32_t status = 0;
	uint32_t value = 0;

	status = plat_get_rnd(&value);
	if (status)
		value = 0xFFFFFFFF;
	return value;
}

#endif
