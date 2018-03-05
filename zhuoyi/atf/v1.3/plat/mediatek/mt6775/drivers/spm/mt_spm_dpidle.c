#include <arch_helpers.h>
#include <debug.h>
#include <mmio.h>
#include <mt_spm.h>
#include <mt_spm_internal.h>
#include <mt_spm_reg.h>
#include <mt_spm_vcorefs.h>
#include <plat_pm.h>
#include <platform.h>
#include <platform_def.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/*
 * only for internal debug
 */
#define SPM_PCMWDT_EN           1
#define SPM_BYPASS_SYSPWREQ     1

struct wake_status spm_wakesta; /* record last wakesta */
static wake_reason_t spm_wake_reason = WR_NONE;

static unsigned int resource_usage;

static bool is_sleep_dpidle;
static unsigned int dpidle_pcm_timer_val;
static unsigned int dpidle_wake_src;

/*
 * SW code for SPM deepidle
 */

#define WAKE_SRC_FOR_COMMON_DPIDLE (\
	(WAKE_SRC_R12_PCMTIMER) | \
	(WAKE_SRC_R12_SSPM_WDT_EVENT_B) | \
	(WAKE_SRC_R12_KP_IRQ_B) | \
	(WAKE_SRC_R12_APXGPT1_EVENT_B) | \
	(WAKE_SRC_R12_SYS_TIMER_EVENT_B) | \
	(WAKE_SRC_R12_EINT_EVENT_B) | \
	(WAKE_SRC_R12_C2K_WDT_IRQ_B) | \
	(WAKE_SRC_R12_CCIF0_EVENT_B) | \
	(WAKE_SRC_R12_SSPM_SPM_IRQ_B) | \
	(WAKE_SRC_R12_SCP_IPC_MD2SPM_B) | \
	(WAKE_SRC_R12_SCP_WDT_EVENT_B) | \
	(WAKE_SRC_R12_USBX_CDSC_B) | \
	(WAKE_SRC_R12_USBX_POWERDWN_B) | \
	(WAKE_SRC_R12_CONN2AP_WAKEUP_B) | \
	(WAKE_SRC_R12_EINT_EVENT_SECURE_B) | \
	(WAKE_SRC_R12_CCIF1_EVENT_B) | \
	(WAKE_SRC_R12_AFE_IRQ_MCU_B) | \
	(WAKE_SRC_R12_SCP_CIRQ_IRQ_B) | \
	(WAKE_SRC_R12_CONN2AP_WDT_IRQ_B) | \
	(WAKE_SRC_R12_MD1_WDT_B) | \
	(WAKE_SRC_R12_MD2AP_PEER_WAKEUP_EVENT))

#if defined(CFG_MICROTRUST_TEE_SUPPORT)
#define WAKE_SRC_FOR_DPIDLE (WAKE_SRC_FOR_COMMON_DPIDLE)
#else
#define WAKE_SRC_FOR_DPIDLE \
	(WAKE_SRC_FOR_COMMON_DPIDLE | WAKE_SRC_R12_SEJ_EVENT_B)
#endif /* #if defined(CFG_MICROTRUST_TEE_SUPPORT) */

static struct pwr_ctrl dpidle_ctrl = {
	.wake_src = WAKE_SRC_FOR_DPIDLE,

#if SPM_BYPASS_SYSPWREQ
	.syspwreq_mask = 1,
#endif

	/* Auto-gen Start */

	/* SPM_CLK_CON */
	.reg_srcclken0_ctl = 0,
	.reg_srcclken1_ctl = 0x3,
	.reg_spm_lock_infra_dcm = 1,
	.reg_srcclken_mask = 1,
	.reg_md1_c32rm_en = 0,
	.reg_md2_c32rm_en = 0,
	.reg_clksq0_sel_ctrl = 0,
	.reg_clksq1_sel_ctrl = 1,
	.reg_srcclken0_en = 1,
	.reg_srcclken1_en = 0,
	.reg_sysclk0_src_mask_b = 0,
	.reg_sysclk1_src_mask_b = 0x20,

	/* SPM_SRC_REQ */
	.reg_spm_apsrc_req = 0,
	.reg_spm_f26m_req = 0,
	.reg_spm_infra_req = 0,
	.reg_spm_ddren_req = 0,
	.reg_spm_vrf18_req = 0,
	.reg_spm_dvfs_level0_req = 0,
	.reg_spm_dvfs_level1_req = 0,
	.reg_spm_dvfs_level2_req = 0,
	.reg_spm_dvfs_level3_req = 0,
	.reg_spm_dvfs_level4_req = 0,
	.reg_spm_sspm_mailbox_req = 0,
	.reg_spm_sw_mailbox_req = 0,
	.reg_spm_cksel2_req = 0,
	.reg_spm_cksel3_req = 0,

	/* SPM_SRC_MASK */
	.reg_csyspwreq_mask = 1,
	.reg_md_srcclkena_0_infra_mask_b = 1,
	.reg_md_srcclkena_1_infra_mask_b = 0,
	.reg_md_apsrc_req_0_infra_mask_b = 0,
	.reg_md_apsrc_req_1_infra_mask_b = 0,
	.reg_conn_srcclkena_infra_mask_b = 1,
	.reg_conn_infra_req_mask_b = 0,
	.reg_sspm_srcclkena_infra_mask_b = 0,
	.reg_sspm_infra_req_mask_b = 1,
	.reg_scp_srcclkena_infra_mask_b = 0,
	.reg_scp_infra_req_mask_b = 1,
	.reg_srcclkeni0_infra_mask_b = 0,
	.reg_srcclkeni1_infra_mask_b = 0,
	.reg_srcclkeni2_infra_mask_b = 0,
	.reg_ccif0_md_event_mask_b = 1,
	.reg_ccif0_ap_event_mask_b = 1,
	.reg_ccif1_md_event_mask_b = 1,
	.reg_ccif1_ap_event_mask_b = 1,
	.reg_ccif2_md_event_mask_b = 1,
	.reg_ccif2_ap_event_mask_b = 1,
	.reg_ccif3_md_event_mask_b = 1,
	.reg_ccif3_ap_event_mask_b = 1,
	.reg_ccifmd_md1_event_mask_b = 0,
	.reg_ccifmd_md2_event_mask_b = 0,
	.reg_c2k_ps_rccif_wake_mask_b = 1,
	.reg_c2k_l1_rccif_wake_mask_b = 0,
	.reg_ps_c2k_rccif_wake_mask_b = 1,
	.reg_l1_c2k_rccif_wake_mask_b = 0,
	.reg_disp2_req_mask_b = 0,
	.reg_md_ddr_en_0_mask_b = 1,
	.reg_md_ddr_en_1_mask_b = 0,
	.reg_conn_ddr_en_mask_b = 1,

	/* SPM_SRC2_MASK */
	.reg_disp0_apsrc_req_mask_b = 1,
	.reg_disp1_apsrc_req_mask_b = 1,
	.reg_disp_od_req_mask_b = 0,
	.reg_mfg_req_mask_b = 0,
	.reg_vdec0_req_mask_b = 0,
	.reg_gce_apsrc_req_mask_b = 1,
	.reg_gce_ddr_en_req_mask_b = 1,
	.reg_lpdma_req_mask_b = 0,
	.reg_conn_srcclkena_cksel2_mask_b = 0,
	.reg_sspm_apsrc_req_ddren_mask_b = 1,
	.reg_scp_apsrc_req_ddren_mask_b = 1,
	.reg_md_vrf18_req_0_mask_b = 1,
	.reg_md_vrf18_req_1_mask_b = 0,
	.reg_next_dvfs_level0_mask_b = 1,
	.reg_next_dvfs_level1_mask_b = 1,
	.reg_next_dvfs_level2_mask_b = 1,
	.reg_next_dvfs_level3_mask_b = 1,
	.reg_next_dvfs_level4_mask_b = 1,
	.reg_sw2spm_int0_mask_b = 1,
	.reg_sw2spm_int1_mask_b = 1,
	.reg_sw2spm_int2_mask_b = 1,
	.reg_sw2spm_int3_mask_b = 1,
	.reg_sspm2spm_int0_mask_b = 1,
	.reg_sspm2spm_int1_mask_b = 1,
	.reg_sspm2spm_int2_mask_b = 1,
	.reg_sspm2spm_int3_mask_b = 1,
	.reg_dqssoc_req_mask_b = 0,
	.reg_gce_busclk_req_mask_b = 1,
	.reg_ufs_srcclkena_mask_b = 1,
	.reg_ufs_vrf18_req_mask_b = 0,

	/* SPM_SRC3_MASK */
	.reg_mpwfi_op = 1,
	.reg_spm_resource_req_rsv1_4_mask_b = 0,
	.reg_spm_resource_req_rsv1_3_mask_b = 0,
	.reg_spm_resource_req_rsv1_2_mask_b = 0,
	.reg_spm_resource_req_rsv1_1_mask_b = 0,
	.reg_spm_resource_req_rsv1_0_mask_b = 0,
	.reg_spm_resource_req_rsv0_4_mask_b = 0,
	.reg_spm_resource_req_rsv0_3_mask_b = 0,
	.reg_spm_resource_req_rsv0_2_mask_b = 0,
	.reg_spm_resource_req_rsv0_1_mask_b = 0,
	.reg_spm_resource_req_rsv0_0_mask_b = 0,
	.reg_srcclkeni2_cksel3_mask_b = 0,
	.reg_srcclkeni2_cksel2_mask_b = 0,
	.reg_srcclkeni1_cksel3_mask_b = 0,
	.reg_srcclkeni1_cksel2_mask_b = 0,
	.reg_srcclkeni0_cksel3_mask_b = 0,
	.reg_srcclkeni0_cksel2_mask_b = 0,
	.reg_md_ddr_en_0_dbc_en = 1,
	.reg_md_ddr_en_1_dbc_en = 0,
	.reg_conn_ddr_en_dbc_en = 1,
	.reg_sspm_mask_b = 1,
	.reg_md_0_mask_b = 1,
	.reg_md_1_mask_b = 0,
	.reg_scp_mask_b = 1,
	.reg_srcclkeni0_mask_b = 1,
	.reg_srcclkeni1_mask_b = 1,
	.reg_srcclkeni2_mask_b = 0,
	.reg_md_apsrc_1_sel = 0,
	.reg_md_apsrc_0_sel = 0,
	.reg_conn_mask_b = 1,
	.reg_conn_apsrc_sel = 0,
	.reg_md_srcclkena_0_vrf18_mask_b = 1,

	/* SPM_SRC4_MASK */
	.reg_ccif4_ap_event_mask_b = 1,
	.reg_ccif4_md_event_mask_b = 1,
	.reg_ccif5_ap_event_mask_b = 1,
	.reg_ccif5_md_event_mask_b = 1,
	.reg_disp0_ddren_req_mask_b = 1,
	.reg_disp1_ddren_req_mask_b = 1,

	/* SPM_WAKEUP_EVENT_MASK */
	.reg_wakeup_event_mask = 0xF1282208,

	/* SPM_EXT_WAKEUP_EVENT_MASK */
	.reg_ext_wakeup_event_mask = 0xFFFFFFFF,

	/* MCU0_WFI_EN */
	.mcu0_wfi_en = 1,

	/* MCU1_WFI_EN */
	.mcu1_wfi_en = 1,

	/* MCU2_WFI_EN */
	.mcu2_wfi_en = 1,

	/* MCU3_WFI_EN */
	.mcu3_wfi_en = 1,

	/* MCU4_WFI_EN */
	.mcu4_wfi_en = 1,

	/* MCU5_WFI_EN */
	.mcu5_wfi_en = 1,

	/* MCU6_WFI_EN */
	.mcu6_wfi_en = 1,

	/* MCU7_WFI_EN */
	.mcu7_wfi_en = 1,

	/* MCU8_WFI_EN */
	.mcu8_wfi_en = 1,

	/* MCU9_WFI_EN */
	.mcu9_wfi_en = 1,

	/* MCU10_WFI_EN */
	.mcu10_wfi_en = 1,

	/* MCU11_WFI_EN */
	.mcu11_wfi_en = 1,

	/* MCU12_WFI_EN */
	.mcu12_wfi_en = 0,

	/* MCU13_WFI_EN */
	.mcu13_wfi_en = 0,

	/* MCU14_WFI_EN */
	.mcu14_wfi_en = 0,

	/* MCU15_WFI_EN */
	.mcu15_wfi_en = 0,

	/* MCU16_WFI_EN */
	.mcu16_wfi_en = 0,

	/* MCU17_WFI_EN */
	.mcu17_wfi_en = 0,

	/* SPM_RSV_CON2 */
	.spm_rsv_con2 =
		(SPM_RSV_CON2_DIS_MCDSR),

	/* Auto-gen End */
};

struct spm_lp_scen __spm_dpidle = {
	.pwrctrl = &dpidle_ctrl,
};

void spm_dpidle_args(__uint64_t x1, __uint64_t x2, __uint64_t x3)
{
	struct pwr_ctrl *pwrctrl;

	pwrctrl = __spm_dpidle.pwrctrl;
	pwrctrl->pcm_flags = x1;

	/* Get SPM resource request from kernel */
	resource_usage = x2;
	pwrctrl->pcm_flags1 = x3;
}

void spm_sleep_dpidle_args(__uint64_t x1, __uint64_t x2, __uint64_t x3)
{
	struct pwr_ctrl *pwrctrl = __spm_dpidle.pwrctrl;

	is_sleep_dpidle = true;

	/* backup original dpidle setting */
	dpidle_pcm_timer_val = pwrctrl->timer_val;
	dpidle_wake_src = pwrctrl->wake_src;

	pwrctrl->timer_val = x1;
	pwrctrl->wake_src = x2;
	pwrctrl->pcm_flags1 = x3;
}

void go_to_dpidle_before_wfi(void)
{
	struct pcm_desc *pcmdesc = NULL;
	struct pwr_ctrl *pwrctrl;
	uint64_t mpidr = read_mpidr();
	__uint32_t cpu = platform_get_core_pos(mpidr);
	int spmfw_idx = __spm_get_spmfw_idx();

	if (dyna_load_pcm[spmfw_idx].ready) {
		pcmdesc = &(dyna_load_pcm[spmfw_idx].desc);
	} else {
		INFO("firmware is not ready!!!\n");
		return;
	}
	pwrctrl = __spm_dpidle.pwrctrl;

#if 0
	INFO("Online CPU is %d, suspend FW ver. is %s\n",
			cpu, pcmdesc->version);

	INFO("sec = %u, wakesrc = 0x%x (%u)(%u)\n",
			pwrctrl->timer_val, pwrctrl->wake_src,
			is_cpu_pdn(pwrctrl->pcm_flags),
			is_infra_pdn(pwrctrl->pcm_flags));
#endif

	__spm_set_cpu_status(cpu);
	__spm_reset_and_init_pcm(pcmdesc);
	__spm_kick_im_to_fetch(pcmdesc);
	__spm_init_pcm_register();
	__spm_init_event_vector(pcmdesc);

	__spm_sync_vcore_dvfs_power_control(pwrctrl, __spm_vcorefs.pwrctrl);

	__spm_set_power_control(pwrctrl);
	__spm_src_req_update(pwrctrl, resource_usage);
	__spm_set_wakeup_event(pwrctrl);

#if SPM_PCMWDT_EN
	if (is_sleep_dpidle && !pwrctrl->wdt_disable)
		__spm_set_pcm_wdt(1);
#endif

	__spm_kick_pcm_to_run(pwrctrl);
}

void go_to_dpidle_after_wfi(void)
{
	struct pcm_desc *pcmdesc = NULL;
	struct pwr_ctrl *pwrctrl;
	int spmfw_idx = __spm_get_spmfw_idx();

	if (dyna_load_pcm[spmfw_idx].ready) {
		pcmdesc = &(dyna_load_pcm[spmfw_idx].desc);
	} else {
		INFO("firmware is not ready!!!\n");
		return;
	}

	pwrctrl = __spm_dpidle.pwrctrl;

	if (is_sleep_dpidle) {
		is_sleep_dpidle = false;

#if SPM_PCMWDT_EN
	if (!pwrctrl->wdt_disable)
		__spm_set_pcm_wdt(0);
#endif

		/* restore original dpidle setting */
		pwrctrl->timer_val = dpidle_pcm_timer_val;
		pwrctrl->wake_src = dpidle_wake_src;
	}

	__spm_get_wakeup_status(&spm_wakesta);
	__spm_clean_after_wakeup();
	spm_wake_reason = __spm_output_wake_reason(&spm_wakesta, pcmdesc);
#if 0
	INFO("spm_wake_reason=%d\n", spm_wake_reason);
#endif
}

void spm_dpidle(void)
{
	spm_lock_get();
	go_to_dpidle_before_wfi();
	spm_lock_release();
}

void spm_dpidle_finish(void)
{
	spm_lock_get();
	go_to_dpidle_after_wfi();
	spm_lock_release();
}
