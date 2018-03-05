/*
 * Copyright (c) 2014 TRUSTONIC LIMITED
 * All rights reserved
 *
 * The present software is the confidential and proprietary information of
 * TRUSTONIC LIMITED. You shall not disclose the present software and shall
 * use it only in accordance with the terms of the license agreement you
 * entered into with TRUSTONIC LIMITED. This software may be subject to
 * export or import laws in certain countries.
 */

#include <arch_helpers.h>
#include <assert.h>
#include <debug.h>
#include <gic_v2.h>
#include <gic_v3.h>
#include <interrupt_mgmt.h>
#include <mmio.h>
#include <mtk_aee_debug.h>
#include "plat_private.h"
#include <platform.h>
#include <platform_def.h>
#include <string.h>
#include <tbase_private.h>
#include <tzc400.h>
#include <rng.h>

/* Defines */
#define PLAT_TBASE_INPUT_HWIDENTITY	(0x1)
#define PLAT_TBASE_INPUT_HWKEY		(0x2)
#define PLAT_TBASE_INPUT_RNG		(0x3)

#define PLAT_TBASE_INPUT_OK		((uint32_t)0)
#define PLAT_TBASE_INPUT_ERROR		((uint32_t)-1)

#if 0
/* Forward declarations */
static void coreDump(uint32_t coreId);
static void platformDump(void);

void irq_raise_softirq(unsigned int map, unsigned int irq);
#endif

/* Code */
/* <t-base handler for SWd fastcall INPUT */
uint32_t plat_tbase_input(uint64_t DataId, uint64_t *Length, void *out)
{
	uint32_t Status = PLAT_TBASE_INPUT_OK;
	uint32_t length = 0, idx = 0;

	/* Implement associated actions */
	switch (DataId) {
	case PLAT_TBASE_INPUT_HWIDENTITY: {
		atf_arg_t_ptr teearg = &gteearg;

		NOTICE("PLAT_TBASE_INPUT_HWIDENTITY\n");
		/* Maybe we can move this to a memcpy instead */
		for (idx = 0; idx < (*Length / sizeof(uint32_t)) &&
				idx < (sizeof(teearg->hwuid) / sizeof(uint32_t)); idx++) {
			((uint32_t *)out)[idx] = teearg->hwuid[idx];
			length += sizeof(uint32_t);
		}
		break;
	}
	case PLAT_TBASE_INPUT_HWKEY: {
		atf_arg_t_ptr teearg = &gteearg;

		NOTICE("PLAT_TBASE_INPUT_HWKEY\n");
		for (idx = 0; idx < teearg->atf_hrid_size; idx++) {
			/*
			 * Get HRID size from "teearg->atf_hrid_size",
			 * not the array size of "teearg->HRID"
			 */
			NOTICE("HRID%d:0x%x\n", idx, teearg->HRID[idx]);
			((uint32_t *)out)[idx] = teearg->HRID[idx];
			length += sizeof(uint32_t);
		}
		*Length = length;
		NOTICE("Length: %lu\n", *Length);
		break;
	}
	case PLAT_TBASE_INPUT_RNG: {
		NOTICE("PLAT_TBASE_INPUT_RNG\n");
		Status = plat_get_rnd((uint32_t *)out);
		*Length = 4;
		break;
	}
	default:
		/* Unsupported request */
		Status = PLAT_TBASE_INPUT_ERROR;
		break;
	}
	return Status;
}

void plat_tbase_fiqforward_init(void)
{
	NOTICE("Configuring t-base forwarded FIQs...\n");

	/* Watchdog FIQ configuration */
	tbase_fiqforward_configure(WDT_IRQ_BIT_ID,   /* interrupt id */
								TBASE_FLAG_SET);  /* enable forward */

#if FIQFORWARD_TEST
	/* Another forwarded FIQ, just for testing purpose */
	tbase_fiqforward_configure(161,              /* interrupt id */
								TBASE_FLAG_SET); /* enable forward */
#endif
}

