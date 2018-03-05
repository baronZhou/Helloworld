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

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <arch_helpers.h>
#include <console.h>
#include <platform.h>
#include <context_mgmt.h>
#include <runtime_svc.h>
#include <bl31.h>
#include "teei_private.h"
#include "teei_atf.h"

#ifdef TEEI_PM_ENABLE

#include <psci.h>


static void save_sysregs_core(uint64_t fromCoreNro, uint32_t toCoreNro)
{
	if (fromCoreNro != toCoreNro) {
		cpu_context_t *cpu_ctx = &secure_context[fromCoreNro].cpu_ctx;

		memcpy(&secure_context[toCoreNro].cpu_ctx, cpu_ctx, sizeof(cpu_context_t));
	}
}
void teei_init_core(uint64_t mpidr)
{
	uint32_t linear_id = platform_get_core_pos(mpidr);
	struct teei_context *teei_ctx = &secure_context[linear_id];

	if (mpidr == teeiBootCoreMpidr)
		return;

	teei_init_secure_context(teei_ctx);
	uint32_t boot_core_nro = platform_get_core_pos(teeiBootCoreMpidr);

	save_sysregs_core(boot_core_nro, linear_id);
}
/*******************************************************************************
 * This cpu has been turned on.
 ******************************************************************************/
static void teei_cpu_on_finish_handler(uint64_t cookie)
{

	uint64_t mpidr = read_mpidr();

	/*uint32_t linear_id = platform_get_core_pos(mpidr);*/
	/*teei_context *teei_ctx = &secure_context[linear_id];*/
	teei_init_core(mpidr);
}

/*******************************************************************************
 * Structure populated by the TEEI Dispatcher to be given a chance to perform any
 * TEEI bookkeeping before PSCI executes a power mgmt. operation.
 ******************************************************************************/
const spd_pm_ops_t teei_pm = {
	NULL,
	NULL,
	NULL,
	teei_cpu_on_finish_handler,
	NULL,
	NULL,
	NULL
};

#endif

