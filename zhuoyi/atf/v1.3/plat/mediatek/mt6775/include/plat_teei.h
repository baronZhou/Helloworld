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

#ifndef __PLAT_TEEI_H__
#define __PLAT_TEEI_H__

#include <mtk_plat_common.h>
#include <platform.h>
#include <plat_private.h>
#include "platform_def_teei.h"
#include "plat_teei_inner.h"

/*secure world FIQ */
extern unsigned int SEC_APP_INTR;
extern unsigned int SEC_DRV_INTR;
extern unsigned int SEC_RDRV_INTR;
extern unsigned int SEC_TDUMP_INTR;

extern void migrate_gic_context(uint32_t secure_state);
extern void disable_group(unsigned int grp);
extern void enable_group(unsigned int grp);
extern void trigger_soft_intr(unsigned int id);
extern void prepare_gic_for_nsec_boot(void);
extern void prepare_gic_for_sec_boot(void);
extern void teei_gic_setup(void);
extern void teei_ack_gic(void);
extern void teei_triggerSgiDump(void);
extern void plat_teei_dump(void);

extern uint64_t teei_fiq_handler(uint32_t id, uint32_t flags, void *handle, void *cookie);

extern uint32_t teei_rng(void);
extern unsigned int teei_get_fp_id(void);
extern int set_drm_int_secure(unsigned int id);
extern int set_drm_int_nonsecure(unsigned int id);
#endif
