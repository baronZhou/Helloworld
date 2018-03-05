/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2014. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#include <arch.h>
#include <arch_helpers.h>
#include <assert.h>
#include <console.h>
#include <debug.h>
#include <devapc.h>
#include <emi_drv.h>
#include <log.h>
#include <mcsi.h>
#include <mcucfg.h>
#include <mmio.h>
#include <mt_cpuxgpt.h>
#include <mt_spm.h>
#include <mt_spm_vcorefs.h>
#include <mt_spm_dpidle.h>
#include <mt_spm_sodi.h>
#include <mt_spm_idle.h>
#include <mtk_plat_common.h>
#include <mtk_sip_svc.h>
#include <plat_debug.h>
#include <plat_dcm.h>
#include <plat_debug.h>
#include <plat_dramc.h>
#include <plat_pm.h>
#include <plat_private.h>
#include <platform.h>
#include <platform_def.h>
#include <rpmb_hmac.h>
#include <runtime_svc.h>
#include <xlat_tables.h>
#include <ufs.h>
#include <fde_aes.h>
#ifdef SECURE_DEINT_SUPPORT
#include <eint.h>
#endif
#include <crypto_hw.h>
#include <mediatek/fiq_cache.h>
#include <mtspmc.h>
#include <rng.h>
#include <mtk_mcdi.h>
#include <md.h>
#include <amms.h>
extern int mt_cluster_ops(int cputop_mpx, int state, int mode);
extern int mt_core_ops(int cpux, int state, int mode);
extern int mt_lastpc_main(void);

extern void dfd_disable(void);
extern uint64_t mt_irq_dump_status(uint32_t irq);
extern struct atf_arg_t gteearg;
/*******************************************************************************
 * SIP top level handler for servicing SMCs.
 ******************************************************************************/
extern void set_kernel_k32_64(uint64_t k32_64);
extern uint64_t wdt_kernel_cb_addr;

/*******************************************************************************

 * SMC Call for Kernel MCUSYS register write
 ******************************************************************************/

static uint64_t mcusys_write_count = 0;
static uint64_t sip_mcusys_write(unsigned int reg_addr, unsigned int reg_value)
{
	if((reg_addr & 0xFFFF0000) != (MCUCFG_BASE & 0xFFFF0000))
	{
		INFO("[ATF] sip_mcusys_write: %x Invalid Range.\n", reg_addr);
		return MTK_SIP_E_INVALID_RANGE;
	}
	/* Perform range check */
	/* if(( MP0_MISC_CONFIG0 <= reg_addr && reg_addr <= MP0_MISC_CONFIG9 ) ||
		 ( MP1_MISC_CONFIG0 <= reg_addr && reg_addr <= MP1_MISC_CONFIG9 )) {
			return MTK_SIP_E_PERMISSION_DENY;
	}

	if (check_cpuxgpt_write_permission(reg_addr, reg_value) == 0) { */
	/* Not allow to clean enable bit[0], Force to set bit[0] as 1 */
	/* reg_value |= 0x1;
	} */

	mmio_write_32(reg_addr, reg_value);
	dsb();

	mcusys_write_count++;

	return MTK_SIP_E_SUCCESS;
}

/*******************************************************************************
 * SIP top level handler for servicing SMCs.
 ******************************************************************************/
uint64_t mediatek_plat_sip_handler(uint32_t smc_fid,
			  uint64_t x1,
			  uint64_t x2,
			  uint64_t x3,
			  uint64_t x4,
			  void *cookie,
			  void *handle,
			  uint64_t flags)
{
	uint64_t rc = 0;
	uint32_t ns;
	struct atf_arg_t *teearg = &gteearg;
	unsigned int atf_crash_log_addr;
	unsigned int atf_crash_log_size;
	unsigned int atf_crash_flag_addr;
	unsigned int *atf_crash_flag_addr_ptr;
	unsigned int rnd_val = 0;

	/* Determine which security state this SMC originated from */
	ns = is_caller_non_secure(flags);
	if (!ns) {
		/* SiP SMC service secure world's call */
		;
	} else {
		switch (smc_fid) {
		case MTK_SIP_KERNEL_MCUSYS_WRITE_AARCH32:
		case MTK_SIP_KERNEL_MCUSYS_WRITE_AARCH64:
			rc = sip_mcusys_write(x1, x2);
			break;
		case MTK_SIP_KERNEL_MCUSYS_ACCESS_COUNT_AARCH32:
		case MTK_SIP_KERNEL_MCUSYS_ACCESS_COUNT_AARCH64:
			rc = mcusys_write_count;
			break;
		case MTK_SIP_LK_WDT_AARCH32:
		case MTK_SIP_LK_WDT_AARCH64:
			set_kernel_k32_64(LINUX_KERNEL_32);
			wdt_kernel_cb_addr = x1;
			NOTICE("MTK_SIP_LK_WDT : 0x%lx \n", wdt_kernel_cb_addr);
			rc = teearg->atf_aee_debug_buf_start;
			break;
		case MTK_SIP_LK_PLAT_DEBUG_AARCH32:
		case MTK_SIP_LK_PLAT_DEBUG_AARCH64:
			console_init(gteearg.atf_log_port, UART_CLOCK, UART_BAUDRATE);
			rc = plat_debug_smc_dispatcher(x1, x2, x3);
			console_uninit();
			break;
		case MTK_SIP_LK_LASTPC_AARCH32:
		case MTK_SIP_LK_LASTPC_AARCH64:
			mt_lastpc_main();
			break;
		case MTK_SIP_LK_DUMP_ATF_LOG_INFO_AARCH32:
		case MTK_SIP_LK_DUMP_ATF_LOG_INFO_AARCH64:
			console_init(gteearg.atf_log_port, UART_CLOCK, UART_BAUDRATE);
			atf_crash_log_addr = mt_log_get_crash_log_addr();
			atf_crash_log_size = mt_log_get_crash_log_size();
			atf_crash_flag_addr_ptr = mt_log_get_crash_flag_addr();
			atf_crash_flag_addr = (uint64_t)atf_crash_flag_addr_ptr&0xffffffff;

			INFO("LK Dump\n");
			INFO("- Buf addr:0x%x buf size:%u flag addr:0x%x flag:0x%x\n", atf_crash_log_addr, atf_crash_log_size, atf_crash_flag_addr, *atf_crash_flag_addr_ptr);
			console_uninit();
			SMC_RET3(handle, atf_crash_log_addr, atf_crash_log_size, atf_crash_flag_addr);

		case MTK_SIP_LK_MD_REG_WRITE_AARCH32:
		case MTK_SIP_LK_MD_REG_WRITE_AARCH64:
			rc = sip_write_md_regs((uint32_t)x1,(uint32_t)x2,(uint32_t)x3,(uint32_t)x4);
		break;
#ifdef MTK_ATF_RAM_DUMP
		case MTK_SIP_RAM_DUMP_ADDR_AARCH32:
			atf_ram_dump_base = x1<<32| (x2&0xffffffff);
			atf_ram_dump_size = x3<<32 | (x4&0xffffffff);
			break;
		case MTK_SIP_RAM_DUMP_ADDR_AARCH64:
			atf_ram_dump_base = x1;
			atf_ram_dump_size = x2;
			break;
#endif
		case MTK_SIP_KERNEL_WDT_AARCH32:
		case MTK_SIP_KERNEL_WDT_AARCH64:
			wdt_kernel_cb_addr = x1;
			NOTICE("MTK_SIP_KERNEL_WDT : 0x%lx \n", wdt_kernel_cb_addr);
			NOTICE("teearg->atf_aee_debug_buf_start : 0x%x \n",
				   teearg->atf_aee_debug_buf_start);
			rc = teearg->atf_aee_debug_buf_start;
			break;

		case MTK_SIP_KERNEL_DO_FIQ_CACHE_AARCH32:
		case MTK_SIP_KERNEL_DO_FIQ_CACHE_AARCH64:
			cache_flush_all_by_fiq();
			rc = 0;
			break;

		case MTK_SIP_KERNEL_FIQ_CACHE_INIT_AARCH32:
		case MTK_SIP_KERNEL_FIQ_CACHE_INIT_AARCH64:
			init_fiq_cache_step(x1);
			rc = 0;
			break;

		case MTK_SIP_KERNEL_GIC_DUMP_AARCH32:
		case MTK_SIP_KERNEL_GIC_DUMP_AARCH64:
#if (TARGET_BUILD_VARIANT_ENG == 1)
			if (x3 == GENERATE_ATF_CRASH_REPORT) {
				/* Jump to Null address and assert */
				void (*fp)(void);

				fp = NULL;
				(*fp)();
			} else
#endif
			rc = mt_irq_dump_status(x1);
			break;
		case MTK_SIP_LK_DAPC_INIT_AARCH32:
		case MTK_SIP_LK_DAPC_INIT_AARCH64:
			/* Setup DEVAPC in ATF */
			rc = start_devapc();
			break;
		case MTK_SIP_KERNEL_MSG_AARCH32:
		case MTK_SIP_KERNEL_MSG_AARCH64:
			if (x1 == 0 && x2 == 1)
				mtk_suspend_timestamp_addr = x3;
			else if (x1 == 0 && x2 == 2)
				mtk_suspend_footprint_addr = x3;

			rc = MTK_SIP_E_SUCCESS;
			break;
		case MTK_SIP_KERNEL_EMIMPU_READ_AARCH32:
		case MTK_SIP_KERNEL_EMIMPU_READ_AARCH64:
			rc = (uint64_t)sip_emimpu_read(x1);
			break;
		case MTK_SIP_KERNEL_EMIMPU_WRITE_AARCH32:
		case MTK_SIP_KERNEL_EMIMPU_WRITE_AARCH64:
			rc = sip_emimpu_write(x1, x2);
			break;
		case MTK_SIP_KERNEL_EMIMPU_SET_AARCH32:
		case MTK_SIP_KERNEL_EMIMPU_SET_AARCH64:
			rc = sip_emimpu_set_region_protection(x1, x2, x3);
			break;
		case MTK_SIP_KERNEL_SPM_SUSPEND_ARGS_AARCH32:
		case MTK_SIP_KERNEL_SPM_SUSPEND_ARGS_AARCH64:
			spm_suspend_args(x1, x2, x3);
			break;
		case MTK_SIP_KERNEL_SPM_FIRMWARE_STATUS_AARCH32:
		case MTK_SIP_KERNEL_SPM_FIRMWARE_STATUS_AARCH64:
			rc = spm_load_firmware_status();
			break;
		case MTK_SIP_KERNEL_SPM_IRQ0_HANDLER_AARCH32:
		case MTK_SIP_KERNEL_SPM_IRQ0_HANDLER_AARCH64:
			spm_irq0_handler(x1);
			break;
		case MTK_SIP_KERNEL_SPM_AP_MDSRC_REQ_AARCH32:
		case MTK_SIP_KERNEL_SPM_AP_MDSRC_REQ_AARCH64:
			spm_ap_mdsrc_req(x1);
			break;
		case MTK_SIP_KERNEL_SPM_PWR_CTRL_ARGS_AARCH32:
		case MTK_SIP_KERNEL_SPM_PWR_CTRL_ARGS_AARCH64:
			spm_pwr_ctrl_args(x1, x2, x3);
			break;
		case MTK_SIP_KERNEL_SPM_GET_PWR_CTRL_ARGS_AARCH32:
		case MTK_SIP_KERNEL_SPM_GET_PWR_CTRL_ARGS_AARCH64:
			rc = spm_get_pwr_ctrl_args(x1, x2, x3);
			break;
		case MTK_SIP_KERNEL_SPM_LEGACY_SLEEP_AARCH32:
		case MTK_SIP_KERNEL_SPM_LEGACY_SLEEP_AARCH64:
			spm_legacy_sleep_wfi(x1, x2, x3);
			break;
		case MTK_SIP_KERNEL_SPM_VCOREFS_ARGS_AARCH32:
		case MTK_SIP_KERNEL_SPM_VCOREFS_ARGS_AARCH64:
			spm_vcorefs_args(x1, x2, x3);
			break;
		case MTK_SIP_KERNEL_SPM_DPIDLE_ARGS_AARCH32:
		case MTK_SIP_KERNEL_SPM_DPIDLE_ARGS_AARCH64:
			spm_dpidle_args(x1, x2, x3);
			break;
		case MTK_SIP_KERNEL_SPM_SODI_ARGS_AARCH32:
		case MTK_SIP_KERNEL_SPM_SODI_ARGS_AARCH64:
			spm_sodi_args(x1, x2, x3);
			break;
		case MTK_SIP_KERNEL_SPM_SLEEP_DPIDLE_ARGS_AARCH32:
		case MTK_SIP_KERNEL_SPM_SLEEP_DPIDLE_ARGS_AARCH64:
			spm_sleep_dpidle_args(x1, x2, x3);
			break;
		case MTK_SIP_KERNEL_CHECK_SECURE_CG_AARCH32:
		case MTK_SIP_KERNEL_CHECK_SECURE_CG_AARCH64:
			rc = spm_idle_check_secure_cg(x1, x2, x3);
			break;
		case MTK_SIP_KERNEL_SPM_DCS_S1_AARCH32:
		case MTK_SIP_KERNEL_SPM_DCS_S1_AARCH64:
			/* spm_dcs_s1_setting(x1, x2); */
			break;
		case MTK_SIP_KERNEL_SPM_DUMMY_READ_AARCH32:
		case MTK_SIP_KERNEL_SPM_DUMMY_READ_AARCH64:
			spm_dummy_read(x1, x2);
			break;
		case MTK_SIP_KERNEL_SPM_ARGS_AARCH32:
		case MTK_SIP_KERNEL_SPM_ARGS_AARCH64:
			spm_args(x1, x2, x3);
			break;
/* Disable for ep */
#ifdef __MTK_RPMB
		case MTK_SIP_LK_RPMB_INIT_AARCH32:
		case MTK_SIP_LK_RPMB_INIT_AARCH64:
		/* create shared memory for rpmb atf module */
			rpmb_init();
			break;
		case MTK_SIP_LK_RPMB_UNINIT_AARCH32:
		case MTK_SIP_LK_RPMB_UNINIT_AARCH64:
			/* mark leaving lk and release resources. */
			rpmb_uninit();
			break;
		case MTK_SIP_LK_RPMB_HMAC_AARCH32:
		case MTK_SIP_LK_RPMB_HMAC_AARCH64:
			/* rpmb hmac calculation module */
			rpmb_hmac(x1, x2);
			break;
#endif
		case MTK_SIP_LK_CRYPTO_HW_ENGINE_INIT_AARCH32:
		case MTK_SIP_LK_CRYPTO_HW_ENGINE_INIT_AARCH64:
			rc = crypto_hw_init((intptr_t)x1, x2);
			SMC_RET1(handle, rc);
			break;
		case MTK_SIP_LK_CRYPTO_HW_ENGINE_DISABLE_AARCH32:
		case MTK_SIP_LK_CRYPTO_HW_ENGINE_DISABLE_AARCH64:
			rc = crypto_hw_disable();
			SMC_RET1(handle, rc);
			break;
		case MTK_SIP_LK_CRYPTO_SHA256_INIT_AARCH32:
		case MTK_SIP_LK_CRYPTO_SHA256_INIT_AARCH64:
			rc = sha256hw_init();
			SMC_RET1(handle, rc);
			break;
		case MTK_SIP_LK_CRYPTO_SHA256_PROCESS_AARCH32:
		case MTK_SIP_LK_CRYPTO_SHA256_PROCESS_AARCH64:
			rc = sha256hw_process((const intptr_t)x1, (unsigned long)x2);
			SMC_RET1(handle, rc);
			break;
		case MTK_SIP_LK_CRYPTO_SHA256_DONE_AARCH32:
		case MTK_SIP_LK_CRYPTO_SHA256_DONE_AARCH64:
			rc = sha256hw_done();
			SMC_RET1(handle, rc);
			break;
		case MTK_SIP_LK_GET_RND_AARCH32:
		case MTK_SIP_LK_GET_RND_AARCH64:
			rc = plat_get_rnd(&rnd_val);
			SMC_RET2(handle, rc, rnd_val);
			break;
		case MTK_SIP_LK_AMMS_MD_BASE_ADDR_AARCH32:
		case MTK_SIP_LK_AMMS_MD_BASE_ADDR_AARCH64:
			rc = sip_amms_set_md_base_address((unsigned long long)x1);
			break;
		case MTK_SIP_LK_AMMS_GET_MD_BASE_ADDR_AARCH32:
		case MTK_SIP_LK_AMMS_GET_MD_BASE_ADDR_AARCH64:
			rc = sip_amms_get_md_base_address();
			SMC_RET1(handle, rc);
			break;

		case MTK_SIP_KERNEL_AMMS_GET_FREE_ADDR_AARCH32:
		case MTK_SIP_KERNEL_AMMS_GET_FREE_ADDR_AARCH64:
			rc = sip_kernel_amms_get_free_addr();
			SMC_RET1(handle, rc);
			break;
		case MTK_SIP_KERNEL_AMMS_GET_FREE_LENGTH_AARCH32:
		case MTK_SIP_KERNEL_AMMS_GET_FREE_LENGTH_AARCH64:
			rc = sip_kernel_amms_get_free_length();
			SMC_RET1(handle, rc);
			break;
#ifdef MTK_UFS_BOOTING
		case MTK_SIP_KERNEL_HW_FDE_UFS_INIT_AARCH32:
		case MTK_SIP_KERNEL_HW_FDE_UFS_INIT_AARCH64:
			rc = ufs_mtk_atf_crypto_init();
			break;
#endif
		case MTK_SIP_KERNEL_HW_FDE_AES_INIT_AARCH32:
		case MTK_SIP_KERNEL_HW_FDE_AES_INIT_AARCH64:
			rc = fde_aes_action(x1, x2);
			break;
		case MTK_SIP_KERNEL_DRAM_DCS_CHB_AARCH32:
		case MTK_SIP_KERNEL_DRAM_DCS_CHB_AARCH64:
			rc = (uint64_t)sip_dram_smc_chcd(x1);
			break;
		case MTK_SIP_KERNEL_TIME_SYNC_AARCH32:
		case MTK_SIP_KERNEL_TIME_SYNC_AARCH64:
			INFO("SIP_KTIME: 0x%lx 0x%lx atf: 0x%lx\n", x1, x2, sched_clock());
			/* in arch32, high 32 bits is stored in x2 and this would be 0 in arch64 */
			sched_clock_init(sched_clock() - (x1 + (x2 << 32)), 0);
			MT_LOG_KTIME_SET();
			INFO("ATF wall time is now synced with kernel, 0x%lx\n", sched_clock());
			rc = MTK_SIP_E_SUCCESS;
			break;

		case MTK_SIP_POWER_DOWN_CLUSTER_AARCH32:
		case MTK_SIP_POWER_DOWN_CLUSTER_AARCH64:
			mt_cluster_ops(x1,x2,0);
			break;
		case MTK_SIP_POWER_UP_CLUSTER_AARCH32:
		case MTK_SIP_POWER_UP_CLUSTER_AARCH64:
			mt_cluster_ops(x1,x2,1);
			break;
		case MTK_SIP_POWER_DOWN_CORE_AARCH32:
		case MTK_SIP_POWER_DOWN_CORE_AARCH64:
			INFO("[ATF]Power down core %lu\n",x1);
			mt_core_ops(x1,x2,0);
			break;
		case MTK_SIP_POWER_UP_CORE_AARCH32:
		case MTK_SIP_POWER_UP_CORE_AARCH64:
			INFO("[ATF]Power up core %lu\n",x1);
			mt_core_ops(x1,x2,1);
			break;
		case MTK_SIP_KERNEL_DCM_AARCH32:
		case MTK_SIP_KERNEL_DCM_AARCH64:
			if (x2)
				rc = dcm_get_enabled_cnt(x1);
			else
				dcm_msg_handler(x1);
			break;
#ifdef MCUPM_FW_USE_PARTITION
		case MTK_SIP_KERNEL_MCUPM_ARGS_AARCH32:
		case MTK_SIP_KERNEL_MCUPM_ARGS_AARCH64:
			mcdi_args(x1, x2, x3);
			break;
		case MTK_SIP_KERNEL_MCUPM_FW_STATUS_ARGS_AARCH32:
		case MTK_SIP_KERNEL_MCUPM_FW_STATUS_ARGS_AARCH64:
			rc = mcdi_load_firmware_status();
			break;
#endif
		default:
			rc = SMC_UNK;
			console_init(gteearg.atf_log_port, UART_CLOCK, UART_BAUDRATE);
			ERROR("%s: unhandled SMC (0x%x)\n", __func__, smc_fid);
			console_uninit();
		}
	}
	SMC_RET1(handle, rc);
}

