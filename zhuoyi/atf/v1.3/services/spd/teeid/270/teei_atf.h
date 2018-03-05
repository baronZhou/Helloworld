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
#ifndef __TEEI_ATF_H__
#define __TEEI_ATF_H__

#ifndef MICROTRUST_ATF_VERSION
	error MICROTRUST_ATF_VERSION;
#endif

#include <debug.h>

#if MICROTRUST_ATF_VERSION >= 13
	#include <mtk_plat_common.h>
	#define CM_GET_CONTEXT(x)	cm_get_context_by_index(platform_get_core_pos(read_mpidr()), x)
	#define SET_MODULE_APC(x, y, z)	/* set_module_apc(x, y, z) */
	#define UART_INIT	console_init(gteearg.atf_log_port, UART_CLOCK, UART_BAUDRATE)
	#define UART_UNINIT	console_uninit()
	#define GICD_BASE	BASE_GICD_BASE
	#define GICC_BASE	BASE_GICC_BASE
	#ifdef DEBUG
	#define DBG_PRINTF(...)	tf_printf(__VA_ARGS__)
	#endif
	extern struct atf_arg_t gteearg;
#elif MICROTRUST_ATF_VERSION == 12
	#include <plat_private.h>
	#define CM_GET_CONTEXT(x)	cm_get_context_by_mpidr(read_mpidr(), x)
	#define SET_MODULE_APC(x, y, z)	/* set_module_apc(x, y, z) */
	#define UART_INIT	console_init(gteearg.atf_log_port, UART_CLOCK, UART_BAUDRATE)
	#define UART_UNINIT	console_uninit()
	#define GICD_BASE	BASE_GICD_BASE
	#define GICC_BASE	BASE_GICC_BASE
	#ifdef DEBUG
	#define DBG_PRINTF(...)	tf_printf(__VA_ARGS__)
	#endif
	extern atf_arg_t gteearg;
#else
	#include <plat_private.h>
	#include "plat_def.h"
	#include "plat_config.h"
	#define CM_GET_CONTEXT(x)	cm_get_context_by_mpidr(read_mpidr(), x)
	#define SET_MODULE_APC(x, y, z)	set_module_apc(x, y, z)
	#define UART_INIT	set_uart_flag()
	#define UART_UNINIT	clear_uart_flag()
	#define GICD_BASE	get_plat_config()->gicd_base
	#define GICC_BASE	get_plat_config()->gicc_base
	#ifdef DEBUG
	#define DBG_PRINTF(...)	printf(__VA_ARGS__)
	#endif
	extern atf_arg_t gteearg;
#endif

#ifndef DEBUG
#define DBG_PRINTF(...)
#endif

#endif
