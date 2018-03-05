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
#include <mtk_sip_svc.h>

static struct pwr_ctrl vcorefs_ctrl = {
	.wake_src		= WAKE_SRC_R12_PCMTIMER,

	/* default VCORE DVFS is disabled */
	.pcm_flags		= (SPM_FLAG_RUN_COMMON_SCENARIO | SPM_FLAG_DIS_VCORE_DVS | SPM_FLAG_DIS_VCORE_DFS
				   | SPM_FLAG_DIS_DVFS_MMPLL_SET),

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
	.reg_md_srcclkena_0_infra_mask_b = 0,
	.reg_md_srcclkena_1_infra_mask_b = 0,
	.reg_md_apsrc_req_0_infra_mask_b = 0,
	.reg_md_apsrc_req_1_infra_mask_b = 0,
	.reg_conn_srcclkena_infra_mask_b = 0,
	.reg_conn_infra_req_mask_b = 0,
	.reg_sspm_srcclkena_infra_mask_b = 0,
	.reg_sspm_infra_req_mask_b = 0,
	.reg_scp_srcclkena_infra_mask_b = 0,
	.reg_scp_infra_req_mask_b = 0,
	.reg_srcclkeni0_infra_mask_b = 0,
	.reg_srcclkeni1_infra_mask_b = 0,
	.reg_srcclkeni2_infra_mask_b = 0,
	.reg_ccif0_md_event_mask_b = 0,
	.reg_ccif0_ap_event_mask_b = 0,
	.reg_ccif1_md_event_mask_b = 0,
	.reg_ccif1_ap_event_mask_b = 0,
	.reg_ccif2_md_event_mask_b = 0,
	.reg_ccif2_ap_event_mask_b = 0,
	.reg_ccif3_md_event_mask_b = 0,
	.reg_ccif3_ap_event_mask_b = 0,
	.reg_ccifmd_md1_event_mask_b = 0,
	.reg_ccifmd_md2_event_mask_b = 0,
	.reg_c2k_ps_rccif_wake_mask_b = 0,
	.reg_c2k_l1_rccif_wake_mask_b = 0,
	.reg_ps_c2k_rccif_wake_mask_b = 0,
	.reg_l1_c2k_rccif_wake_mask_b = 0,
	.reg_disp2_req_mask_b = 0,
	.reg_md_ddr_en_0_mask_b = 0,
	.reg_md_ddr_en_1_mask_b = 0,
	.reg_conn_ddr_en_mask_b = 0,

	/* SPM_SRC2_MASK */
	.reg_disp0_req_mask_b = 0,
	.reg_disp1_req_mask_b = 0,
	.reg_disp_od_req_mask_b = 0,
	.reg_mfg_req_mask_b = 0,
	.reg_vdec0_req_mask_b = 0,
	.reg_gce_req_mask_b = 0,
	.reg_gce_vrf18_req_mask_b = 0,
	.reg_lpdma_req_mask_b = 0,
	.reg_conn_srcclkena_cksel2_mask_b = 0,
	.reg_sspm_apsrc_req_ddren_mask_b = 0,
	.reg_scp_apsrc_req_ddren_mask_b = 0,
	.reg_md_vrf18_req_0_mask_b = 0,
	.reg_md_vrf18_req_1_mask_b = 0,
	.reg_next_dvfs_level0_mask_b = 1,
	.reg_next_dvfs_level1_mask_b = 1,
	.reg_next_dvfs_level2_mask_b = 1,
	.reg_next_dvfs_level3_mask_b = 1,
	.reg_next_dvfs_level4_mask_b = 1,
	.reg_sw2spm_int0_mask_b = 0,
	.reg_sw2spm_int1_mask_b = 0,
	.reg_sw2spm_int2_mask_b = 0,
	.reg_sw2spm_int3_mask_b = 0,
	.reg_sspm2spm_int0_mask_b = 0,
	.reg_sspm2spm_int1_mask_b = 0,
	.reg_sspm2spm_int2_mask_b = 0,
	.reg_sspm2spm_int3_mask_b = 0,
	.reg_dqssoc_req_mask_b = 0,
	.reg_ufs_srcclkena_mask_b = 0,
	.reg_ufs_vrf18_req_mask_b = 0,

	/* SPM_SRC3_MASK */
	.reg_mpwfi_op = 0,
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
	.reg_md_ddr_en_0_dbc_en = 0,
	.reg_md_ddr_en_1_dbc_en = 0,
	.reg_conn_ddr_en_dbc_en = 0,
	.reg_sspm_mask_b = 0,
	.reg_md_0_mask_b = 0,
	.reg_md_1_mask_b = 0,
	.reg_scp_mask_b = 0,
	.reg_srcclkeni0_mask_b = 0,
	.reg_srcclkeni1_mask_b = 0,
	.reg_srcclkeni2_mask_b = 0,
	.reg_md_apsrc_1_sel = 0,
	.reg_md_apsrc_0_sel = 0,
	.reg_conn_mask_b = 0,
	.reg_conn_apsrc_sel = 0,
	.reg_md_srcclkena_0_vrf18_mask_b = 0,

	/* SPM_WAKEUP_EVENT_MASK */
	.reg_wakeup_event_mask = 0xFFFFFFFF,

	/* SPM_EXT_WAKEUP_EVENT_MASK */
	.reg_ext_wakeup_event_mask = 0xFFFFFFFF,

	/* SLEEP_MCU0_WFI_EN */
	.mcu0_wfi_en = 1,

	/* SLEEP_MCU1_WFI_EN */
	.mcu1_wfi_en = 1,

	/* SLEEP_MCU2_WFI_EN */
	.mcu2_wfi_en = 1,

	/* SLEEP_MCU3_WFI_EN */
	.mcu3_wfi_en = 1,

	/* SLEEP_MCU4_WFI_EN */
	.mcu4_wfi_en = 1,

	/* SLEEP_MCU5_WFI_EN */
	.mcu5_wfi_en = 1,

	/* SLEEP_MCU6_WFI_EN */
	.mcu6_wfi_en = 1,

	/* SLEEP_MCU7_WFI_EN */
	.mcu7_wfi_en = 1,

	/* SLEEP_MCU8_WFI_EN */
	.mcu8_wfi_en = 1,

	/* SLEEP_MCU9_WFI_EN */
	.mcu9_wfi_en = 1,

	/* SLEEP_MCU10_WFI_EN */
	.mcu10_wfi_en = 1,

	/* SLEEP_MCU11_WFI_EN */
	.mcu11_wfi_en = 1,

	/* SLEEP_MCU12_WFI_EN */
	.mcu12_wfi_en = 1,

	/* SLEEP_MCU13_WFI_EN */
	.mcu13_wfi_en = 1,

	/* SLEEP_MCU14_WFI_EN */
	.mcu14_wfi_en = 1,

	/* SLEEP_MCU15_WFI_EN */
	.mcu15_wfi_en = 0,

	/* SLEEP_MCU16_WFI_EN */
	.mcu16_wfi_en = 0,

	/* SLEEP_MCU17_WFI_EN */
	.mcu17_wfi_en = 0,

	/* Auto-gen End */
};

struct spm_lp_scen __spm_vcorefs = {
	.pwrctrl	= &vcorefs_ctrl,
};

struct spm_setclr_reg vcorefs_setclr_regs[] = {
	CFG_SPM_SETCLR_REG(0, "ID_SPM_DVFS_INITIAL_MASK", SPM_DVFS_INITIAL_MASK_SET, SPM_DVFS_INITIAL_MASK_CLR, SPM_DVFS_INITIAL_MASK),
	CFG_SPM_SETCLR_REG(1, "ID_SPM_DVFS_FLOOR_MASK", SPM_DVFS_FLOOR_MASK_SET, SPM_DVFS_FLOOR_MASK_CLR, SPM_DVFS_FLOOR_MASK),
	CFG_SPM_SETCLR_REG(2, "ID_SPM_DVFS_FLOOR_MASK2", SPM_DVFS_FLOOR_MASK2_SET, SPM_DVFS_FLOOR_MASK2_CLR, SPM_DVFS_FLOOR_MASK2),
	CFG_SPM_SETCLR_REG(3, "ID_SPM_DVFS_FLOOR_MASK3", SPM_DVFS_FLOOR_MASK3_SET, SPM_DVFS_FLOOR_MASK3_CLR, SPM_DVFS_FLOOR_MASK3),
	CFG_SPM_SETCLR_REG(4, "ID_SPM_DVFS_FLOOR_MASK4", SPM_DVFS_FLOOR_MASK4_SET, SPM_DVFS_FLOOR_MASK4_CLR, SPM_DVFS_FLOOR_MASK4),
	CFG_SPM_SETCLR_REG(5, "ID_SPM_DVFS_FORCE_MASK0", SPM_DVFS_FORCE_MASK0_SET, SPM_DVFS_FORCE_MASK0_CLR, SPM_DVFS_FORCE_MASK0),
	CFG_SPM_SETCLR_REG(6, "ID_SPM_DVFS_FORCE", SPM_DVFS_FORCE_SET, SPM_DVFS_FORCE_CLR, SPM_DVFS_FORCE),
};

static void spm_dvfsfw_init(__uint64_t boot_up_opp, __uint64_t ch)
{
	__uint32_t opp_val = 0x1 << (NUM_OPP - 1 - (__uint32_t)boot_up_opp);

	/* SW kicker */
	if (mmio_read_32(SPM_SW_RSV_5) == 0xFFFFFFFF) {
		mmio_write_32(vcorefs_setclr_regs[ID_SPM_DVFS_FORCE].clr_addr, 0xFFFFFFFF);
		mmio_write_32(SPM_DVFS_INITIAL_SETTING, 0xC00);
		mmio_write_32(SPM_DVFS_SW_FLOOR0, 0);
		mmio_write_32(SPM_DVFS_SW_FLOOR1, 0);
		mmio_write_32(SPM_DVFS_SW_FLOOR2, 0);
		mmio_write_32(SPM_DVFS_SW_FLOOR3, 0);
		mmio_write_32(SPM_DVFS_SW_FLOOR4, 0);
		mmio_write_32(SPM_DVFS_SW_FLOOR5, 0);
		mmio_write_32(SPM_DVFS_SW_FLOOR6, 0);
		mmio_write_32(SPM_DVFS_SW_FLOOR7, 0);
	}

	/* MD policy */
	mmio_write_32(SPM_MD_DVFS_FLOOR, 0x1B6DB6DB);
	mmio_write_32(SPM_MD_DVFS_FLOOR2, 0x24000);
	mmio_write_32(SPM_MD_DVFS_FLOOR3, 0);
	mmio_write_32(SPM_MD_DVFS_FLOOR4, 0x40);
	mmio_write_32(SPM_MD_DVFS_INITIAL, 0xF00F5555);
	mmio_write_32(SPM_MD_DVFS_INITIAL2, 0);
	mmio_write_32(SPM2MD_DVFS_CON, 0xFFFF);
	mmio_write_32(vcorefs_setclr_regs[ID_SPM_DVFS_FLOOR_MASK4].clr_addr, 0xFFFFFFFF);
	mmio_write_32(vcorefs_setclr_regs[ID_SPM_DVFS_FLOOR_MASK4].set_addr, 0xC3FF);
	mmio_write_32(vcorefs_setclr_regs[ID_SPM_DVFS_INITIAL_MASK].clr_addr, 0xFFFFFFFF);
	mmio_write_32(vcorefs_setclr_regs[ID_SPM_DVFS_INITIAL_MASK].set_addr, 0xC3FF);
	mmio_write_32(SPM_AP_SCENARIO, 0xFFFF);

	/* Mask */
	mmio_write_32(vcorefs_setclr_regs[ID_SPM_DVFS_FORCE_MASK0].clr_addr, 0xFFFFFFFF);
	mmio_write_32(vcorefs_setclr_regs[ID_SPM_DVFS_FORCE_MASK0].set_addr, 0x28);
	mmio_write_32(vcorefs_setclr_regs[ID_SPM_DVFS_FLOOR_MASK].clr_addr, 0xFFFFFFFF);
	mmio_write_32(vcorefs_setclr_regs[ID_SPM_DVFS_FLOOR_MASK].set_addr, 0xFC0);
	mmio_write_32(vcorefs_setclr_regs[ID_SPM_DVFS_FLOOR_MASK2].clr_addr, 0xFFFFFFFF);

	/* set by SCP */
	/* mmio_write_32(vcorefs_setclr_regs[ID_SPM_DVFS_FLOOR_MASK3].clr_addr, 0xFFFFFFFF);*/
	mmio_write_32(SPM_DVFS_CNFG, 4);

	/* debounce setting: */
	/* 15ns*33333 = 500us @66M */
	mmio_write_32(SPM_DVFS_DEBOUNCE3, (mmio_read_32(SPM_DVFS_DEBOUNCE3) & 0xffe00000) |
			SPM_DVFS_DEBOUNCE_SEL3_LSB | 33333);

	/* MSDC halt setting: by pass all */
	mmio_write_32(BYPASS_MSDC_DVFS_HALT, 0xF);

	/* init SPM_SW_RSV_5 */
	if (mmio_read_32(SPM_SW_RSV_5) == 0xFFFFFFFF)
		mmio_write_32(SPM_SW_RSV_5, opp_val << 16);
	else
		mmio_write_32(SPM_SW_RSV_5,
			(mmio_read_32(SPM_SW_RSV_5) & ~(0x000F0000)) | (opp_val << 16));

	mmio_write_32(DVFS_LEVEL,
			(mmio_read_32(DVFS_LEVEL) & ~(0x0000FF1F)) |
			(opp_val | opp_val << 8 | opp_val << 12));
}

static void spm_set_kicker_opp(__uint64_t opp, __uint64_t type)
{
	__uint32_t kicker_opp = (opp >> 8) & 0xf;
	__uint32_t dvfs_opp = opp & 0xf;
	__uint32_t floor_opp = dvfs_opp;
	__uint32_t set_val = 0;
	__uint32_t clr_val = 0;

	if (type & (1 << KIR_GROUP_FIX)) {
		if (kicker_opp < NUM_OPP) {
			if (kicker_opp < OPP_3)
				set_val = 1 << (2 - kicker_opp);
			set_val |= 8;
			clr_val = ~set_val & 0xF;
		} else {
			set_val = 0;
			clr_val = 0xF;
		}
		mmio_write_32(vcorefs_setclr_regs[ID_SPM_DVFS_FORCE].set_addr, (set_val << 8));
		mmio_write_32(vcorefs_setclr_regs[ID_SPM_DVFS_FORCE].clr_addr, (clr_val << 8));
	}

	if (type & (1 << KIR_GROUP_HPM)) {
		if (dvfs_opp < NUM_OPP)
			set_val = 1 << (OPP_3-dvfs_opp);
		else
			set_val = 0;
		mmio_write_32(SPM_DVFS_SW_FLOOR0, (mmio_read_32(SPM_DVFS_SW_FLOOR0) & ~(0xF)) | (set_val));
	}

	if (type & (1 << KIR_GROUP_HPM_NON_FORCE)) {
		if (dvfs_opp < NUM_OPP) {
			floor_opp = (dvfs_opp == (NUM_OPP-1))? dvfs_opp : dvfs_opp + 1;
			set_val = 1 << (OPP_3 - floor_opp);
		} else
			set_val = 0;
		mmio_write_32(SPM_DVFS_SW_FLOOR1, (mmio_read_32(SPM_DVFS_SW_FLOOR1) & ~(0xF)) | (set_val));
		if (dvfs_opp < NUM_OPP) {
			if ( dvfs_opp < OPP_3)
				set_val = 1 << (2 - dvfs_opp);
			set_val |= 8;
		} else
			set_val = 0;
		mmio_write_32(SPM_DVFS_INITIAL_SETTING, (mmio_read_32(SPM_DVFS_INITIAL_SETTING) & ~(0xF << 4)) | (set_val << 4));
	}
}

void __spm_sync_vcore_dvfs_power_control(struct pwr_ctrl *dest_pwr_ctrl, const struct pwr_ctrl *src_pwr_ctrl)
{
	__uint32_t dvfs_mask =  SPM_FLAG_DIS_VCORE_DVS | SPM_FLAG_DIS_VCORE_DFS | SPM_FLAG_DIS_DVFS_MMPLL_SET;

	/* SPM_SRC_REQ */
	dest_pwr_ctrl->reg_spm_dvfs_level0_req = src_pwr_ctrl->reg_spm_dvfs_level0_req;
	dest_pwr_ctrl->reg_spm_dvfs_level1_req = src_pwr_ctrl->reg_spm_dvfs_level1_req;
	dest_pwr_ctrl->reg_spm_dvfs_level2_req = src_pwr_ctrl->reg_spm_dvfs_level2_req;
	dest_pwr_ctrl->reg_spm_dvfs_level3_req = src_pwr_ctrl->reg_spm_dvfs_level3_req;
	dest_pwr_ctrl->reg_spm_dvfs_level4_req = src_pwr_ctrl->reg_spm_dvfs_level4_req;

	/* SPM_SRC2_MASK */
	dest_pwr_ctrl->reg_next_dvfs_level0_mask_b = src_pwr_ctrl->reg_next_dvfs_level0_mask_b;
	dest_pwr_ctrl->reg_next_dvfs_level1_mask_b = src_pwr_ctrl->reg_next_dvfs_level1_mask_b;
	dest_pwr_ctrl->reg_next_dvfs_level2_mask_b = src_pwr_ctrl->reg_next_dvfs_level2_mask_b;
	dest_pwr_ctrl->reg_next_dvfs_level3_mask_b = src_pwr_ctrl->reg_next_dvfs_level3_mask_b;
	dest_pwr_ctrl->reg_next_dvfs_level4_mask_b = src_pwr_ctrl->reg_next_dvfs_level4_mask_b;

	dest_pwr_ctrl->pcm_flags = (dest_pwr_ctrl->pcm_flags & (~dvfs_mask)) |
					(src_pwr_ctrl->pcm_flags & dvfs_mask);

	if (dest_pwr_ctrl->pcm_flags_cust)
		dest_pwr_ctrl->pcm_flags_cust = (dest_pwr_ctrl->pcm_flags_cust & (~dvfs_mask)) |
						(src_pwr_ctrl->pcm_flags & dvfs_mask);

	if (src_pwr_ctrl->pcm_flags1 & 0x2)
		dest_pwr_ctrl->pcm_flags1 |= 0x2;
}

static void spm_go_to_vcorefs(__uint64_t spm_flags)
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

	pwrctrl = __spm_vcorefs.pwrctrl;

	set_pwrctrl_pcm_flags(pwrctrl, spm_flags);

	__spm_reset_and_init_pcm(pcmdesc);

	__spm_kick_im_to_fetch(pcmdesc);

	__spm_init_pcm_register();

	__spm_init_event_vector(pcmdesc);

	__spm_set_power_control(pwrctrl);

	__spm_set_wakeup_event(pwrctrl);

	__spm_kick_pcm_to_run(pwrctrl);
}

void spm_vcorefs_reg_write(__uint32_t offset, __uint32_t reg_value)
{
	__uint32_t addr;

	addr = SPM_BASE + offset;
	mmio_write_32(addr, reg_value);
	dsb();
}

uint32_t spm_vcorefs_reg_read(__uint32_t offset)
{
	__uint32_t addr;

	addr = SPM_BASE + offset;
	return mmio_read_32(addr);
}

__uint64_t spm_vcorefs_args(__uint64_t x1, __uint64_t x2, __uint64_t x3)
{
	__uint64_t cmd = x1;
	__uint64_t r = MTK_SIP_E_SUCCESS;
	switch (cmd) {
		case VCOREFS_SMC_CMD_0:
			spm_dvfsfw_init(x2, x3);
			break;
		case VCOREFS_SMC_CMD_1:
			spm_go_to_vcorefs(x2);
			break;
		case VCOREFS_SMC_CMD_2:
			spm_set_kicker_opp(x2, x3);
			break;
		case VCOREFS_SMC_CMD_3:
			spm_vcorefs_reg_write(x2, x3);
			break;
		case VCOREFS_SMC_CMD_4:
			r = (__uint64_t)spm_vcorefs_reg_read(x2);
			break;
		default:
			INFO("Not support vcorfs cmd!!!\n");
			break;
	}
	return r;
}

