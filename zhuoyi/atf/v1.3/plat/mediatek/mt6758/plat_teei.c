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

#include <assert.h>
#include <debug.h>
#include <string.h>

#include <platform.h>
#include "plat_private.h"
#include <arch_helpers.h>
#include <fiq_smp_call.h>
#include <bl_common.h>
#include <teei_private.h>
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

unsigned int NSEC_TIMER;    /* 30 */
unsigned int NSEC_UART;      /* 124 */

unsigned int SEC_TIMER;      /* 205 */
/* ~ unsigned int SEC_TP;              //           189 */
unsigned int SEC_SPI = 132;

unsigned int SEC_APP_INTR;
unsigned int SEC_DRV_INTR;
unsigned int SEC_RDRV_INTR;
unsigned int SEC_TDUMP_INTR;

uint64_t gic_mpidr_to_affinity(uint64_t mpidr);
int gic_populate_rdist(unsigned int *rdist_base);

int gic_populate_rdist(unsigned int *rdist_base);
void irq_raise_softirq(unsigned int map, unsigned int irq);
extern void gicc_write_grpen1_el1(uint64_t val);

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

	for (index = 0;  index < NSEC_EXC_CNT; index++) {
		gicd_set_icenabler(gicd_base, nsec_exc[index]);
		gicd_v3_do_wait_for_rwp(gicd_base);
	}
}

void  enable_ns_exc_intr(unsigned int gicd_base)
{
	unsigned int index;

	for (index = 0;  index < NSEC_EXC_CNT; index++) {
		gicd_set_isenabler(gicd_base,  nsec_exc[index]);
		gicd_v3_do_wait_for_rwp(gicd_base);
	}
}

void  disable_s_exc_intr(unsigned int gicd_base)
{
	unsigned int index;

	for (index = 0;  index < SEC_EXC_CNT; index++) {
		gicd_set_icenabler(gicd_base, sec_exc[index]);
		gicd_v3_do_wait_for_rwp(gicd_base);
	}
#ifdef SUPPORT_DRM
	for (index = 0;  index < DRM_SEC_EXC_CNT; index++) {
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

	for (index = 0;  index < SEC_EXC_CNT; index++) {
		gicd_set_isenabler(gicd_base,  sec_exc[index]);
		gicd_v3_set_irouter(gicd_base, sec_exc[index],
				    gic_mpidr_to_affinity(read_mpidr()));
		gicd_v3_do_wait_for_rwp(gicd_base);

	}
#ifdef SUPPORT_DRM
	for (index = 0;  index < DRM_SEC_EXC_CNT; index++) {
		if (drm_sec_exc[index] != 0) {
			gicd_set_isenabler(gicd_base,  drm_sec_exc[index]);
			gicd_v3_set_irouter(gicd_base, drm_sec_exc[index],
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

	gicd_set_icenabler(BASE_GICD_BASE,  id);
	gicd_clr_igroupr(BASE_GICD_BASE, id);
	gicd_v3_set_irouter(BASE_GICD_BASE, id, gic_mpidr_to_affinity(read_mpidr()));
	gicd_set_isenabler(BASE_GICD_BASE,  id);
	gicd_v3_do_wait_for_rwp(BASE_GICD_BASE);
	for (index = 0; index < DRM_SEC_EXC_CNT; index++) {
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

	for (index = 0; index < DRM_SEC_EXC_CNT; index++) {
		if (drm_sec_exc[index] == id) {
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
	/* unsigned int val; */
	gicd_base = BASE_GICD_BASE;

	if (secure_state == SECURE) {

		disable_ns_exc_intr(gicd_base);
		/* enable_s_exc_intr(gicd_base); */
		if (TEEI_STATE < TEEI_BUF_READY)
			disable_group(1);
		for (index = 0;  index < SEC_EXC_CNT; index++) {
			/* val = gicd_read_itargetsr(gicd_base, sec_exc[index]); */
			gicd_v3_set_irouter(gicd_base, sec_exc[index],
					    gic_mpidr_to_affinity(read_mpidr()));
			gicd_set_isenabler(gicd_base,  sec_exc[index]);
		}
#ifdef SUPPORT_DRM
		for (index = 0;  index < DRM_SEC_EXC_CNT; index++) {
			if (drm_sec_exc[index] != 0) {
				gicd_read_itargetsr(gicd_base, drm_sec_exc[index]);
				gicd_v3_set_irouter(gicd_base, drm_sec_exc[index],
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
void dump_gic_all_setting()
{
    unsigned int gicd_base, gicc_base;

    gicd_base = BASE_GICD_BASE;
    gicc_base = BASE_GICC_BASE;

    gic_print_all_setting(gicd_base, gicc_base);
}
*/

void sec_exc_add(unsigned int intr_num)
{
	sec_exc[SEC_EXC_CNT] =  intr_num;
	SEC_EXC_CNT++;
}

void nsec_exc_add(unsigned int intr_num)
{
	nsec_exc[NSEC_EXC_CNT] =  intr_num;
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
	for (unsigned int index = 0;  index < SEC_EXC_CNT; index++) {
		gicd_clr_igroupr(gicd_base, sec_exc[index]);                                                                        /*  set this interrupt to group 0*/
		gicd_set_ipriorityr(gicd_base,  sec_exc[index], GIC_HIGHEST_SEC_PRIORITY);        /*set this interrupt GIC_HIGHEST_SEC_PRIORITY*/
		/*set itarget to current cpu */
		/* use 1-N model, means as long as one of N core can handle, this will be handled */
		gicd_v3_set_irouter(gicd_base, sec_exc[index], gic_mpidr_to_affinity(read_mpidr()));
		mt_irq_set_sens(gicd_base,  sec_exc[index], MT_EDGE_SENSITIVE);                          /*set gic edge sensitive via GICD_ICFG*/
		mt_irq_set_polarity(sec_exc[index], MT_POLARITY_LOW); /*set low polarity*/
		gicd_set_icenabler(gicd_base,  sec_exc[index]);                                                                 /*disable  this interrupt in gicd*/
	}
}

uint32_t teei_rng(void)
{
	uint32_t status = 0;
	uint32_t value = 0;

	status = plat_get_rnd(&value);
	if (status != 0)
		value = 0xffffffff;

	return value;
}
