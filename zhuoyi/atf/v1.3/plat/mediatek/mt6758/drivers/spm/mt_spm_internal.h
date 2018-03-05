#ifndef __MT_SPM_INTERNAL__
#define __MT_SPM_INTERNAL__

/**************************************
 * Config and Parameter
 **************************************/
#define POWER_ON_VAL1_DEF	0x00015820
#define PCM_FSM_STA_DEF		0x00048490
#define SPM_WAKEUP_EVENT_MASK_DEF	0xF0F92218

#define PCM_WDT_TIMEOUT		(30 * 32768)	/* 30s */
#define PCM_TIMER_MAX		(0xffffffff - PCM_WDT_TIMEOUT)

/**************************************
 * Define and Declare
 **************************************/
#define PCM_PWRIO_EN_R0		(1 << 0)
#define PCM_PWRIO_EN_R7		(1 << 7)
#define PCM_RF_SYNC_R0		(1 << 16)
#define PCM_RF_SYNC_R6		(1 << 22)
#define PCM_RF_SYNC_R7		(1 << 23)

#define PCM_SW_INT0		(1 << 0)
#define PCM_SW_INT1		(1 << 1)
#define PCM_SW_INT2		(1 << 2)
#define PCM_SW_INT3		(1 << 3)
#define PCM_SW_INT4		(1 << 4)
#define PCM_SW_INT5		(1 << 5)
#define PCM_SW_INT6		(1 << 6)
#define PCM_SW_INT7		(1 << 7)
#define PCM_SW_INT8		(1 << 8)
#define PCM_SW_INT9		(1 << 9)
#define PCM_SW_INT_ALL		(PCM_SW_INT9 | PCM_SW_INT8 | PCM_SW_INT7 | \
				 PCM_SW_INT6 | PCM_SW_INT5 | PCM_SW_INT4 | \
				 PCM_SW_INT3 | PCM_SW_INT2 | PCM_SW_INT1 | \
				 PCM_SW_INT0)

#define WFI_OP_AND		1
#define WFI_OP_OR		0

#define ISRM_TWAM		(1 << 2)
#define ISRM_PCM_RETURN		(1 << 3)
#define ISRM_RET_IRQ0		(1 << 8)
#define ISRM_RET_IRQ1		(1 << 9)
#define ISRM_RET_IRQ2		(1 << 10)
#define ISRM_RET_IRQ3		(1 << 11)
#define ISRM_RET_IRQ4		(1 << 12)
#define ISRM_RET_IRQ5		(1 << 13)
#define ISRM_RET_IRQ6		(1 << 14)
#define ISRM_RET_IRQ7		(1 << 15)
#define ISRM_RET_IRQ8		(1 << 16)
#define ISRM_RET_IRQ9		(1 << 17)
#define ISRM_RET_IRQ10		(1 << 18)
#define ISRM_RET_IRQ11		(1 << 19)
#define ISRM_RET_IRQ12		(1 << 20)
#define ISRM_RET_IRQ13		(1 << 21)
#define ISRM_RET_IRQ14		(1 << 22)
#define ISRM_RET_IRQ15		(1 << 23)

#define ISRM_RET_IRQ_AUX	(ISRM_RET_IRQ15 | ISRM_RET_IRQ14 | \
				 ISRM_RET_IRQ13 | ISRM_RET_IRQ12 | \
				 ISRM_RET_IRQ11 | ISRM_RET_IRQ10 | \
				 ISRM_RET_IRQ9 | ISRM_RET_IRQ8 | \
				 ISRM_RET_IRQ7 | ISRM_RET_IRQ6 | \
				 ISRM_RET_IRQ5 | ISRM_RET_IRQ4 | \
				 ISRM_RET_IRQ3 | ISRM_RET_IRQ2 | \
				 ISRM_RET_IRQ1)
#define ISRM_ALL_EXC_TWAM	(ISRM_RET_IRQ_AUX /*| ISRM_RET_IRQ0 | ISRM_PCM_RETURN*/)
#define ISRM_ALL		(ISRM_ALL_EXC_TWAM | ISRM_TWAM)

#define ISRS_TWAM		(1 << 2)
#define ISRS_PCM_RETURN		(1 << 3)
#define ISRS_SW_INT0		(1 << 4)

#define ISRC_TWAM		ISRS_TWAM
#define ISRC_ALL_EXC_TWAM	ISRS_PCM_RETURN
#define ISRC_ALL		(ISRC_ALL_EXC_TWAM | ISRC_TWAM)

#define WAKE_MISC_TWAM		(1 << 22)
#define WAKE_MISC_PCM_TIMER	(1 << 23)
#define WAKE_MISC_CPU_WAKE	(1 << 24)

extern int spm_for_gps_flag;

struct pcm_desc {
	const char *version;	/* PCM code version */
	__uint32_t *base;	/* binary array base */
	__uint32_t base_dma;	/* dma addr of base */
	const __uint16_t size;		/* binary array size */
	const __uint8_t sess;		/* session number */
	const __uint8_t replace;	/* replace mode */
	const __uint16_t addr_2nd;	/* 2nd binary array size */
	const __uint16_t reserved;	/* for 32bit alignment */

	__uint32_t vec0;		/* event vector 0 config */
	__uint32_t vec1;		/* event vector 1 config */
	__uint32_t vec2;		/* event vector 2 config */
	__uint32_t vec3;		/* event vector 3 config */
	__uint32_t vec4;		/* event vector 4 config */
	__uint32_t vec5;		/* event vector 5 config */
	__uint32_t vec6;		/* event vector 6 config */
	__uint32_t vec7;		/* event vector 7 config */
	__uint32_t vec8;		/* event vector 8 config */
	__uint32_t vec9;		/* event vector 9 config */
	__uint32_t vec10;		/* event vector 10 config */
	__uint32_t vec11;		/* event vector 11 config */
	__uint32_t vec12;		/* event vector 12 config */
	__uint32_t vec13;		/* event vector 13 config */
	__uint32_t vec14;		/* event vector 14 config */
	__uint32_t vec15;		/* event vector 15 config */
};

struct pwr_ctrl {
	/* for SPM */
	__uint32_t pcm_flags;
	__uint32_t pcm_flags_cust;	/* can override pcm_flags */
	__uint32_t pcm_flags_cust_set;	/* set bit of pcm_flags, after pcm_flags_cust */
	__uint32_t pcm_flags_cust_clr;	/* clr bit of pcm_flags, after pcm_flags_cust */
	__uint32_t pcm_flags1;
	__uint32_t pcm_flags1_cust;	/* can override pcm_flags1 */
	__uint32_t pcm_flags1_cust_set;	/* set bit of pcm_flags1, after pcm_flags1_cust */
	__uint32_t pcm_flags1_cust_clr;	/* clr bit of pcm_flags1, after pcm_flags1_cust */
	__uint32_t timer_val;		/* @ 1T 32K */
	__uint32_t timer_val_cust;	/* @ 1T 32K, can override timer_val */
	__uint32_t timer_val_ramp_en;		/* stress for dpidle */
	__uint32_t timer_val_ramp_en_sec;	/* stress for suspend */
	__uint32_t wake_src;
	__uint32_t wake_src_cust;	/* can override wake_src */
	__uint8_t opp_level;
	__uint8_t wdt_disable;		/* disable wdt in suspend */
	__uint8_t syspwreq_mask;	/* make 26M off when attach ICE */

	/* Auto-gen Start */

	/* SPM_CLK_CON */
	__uint8_t reg_srcclken0_ctl;
	__uint8_t reg_srcclken1_ctl;
	__uint8_t reg_spm_lock_infra_dcm;
	__uint8_t reg_srcclken_mask;
	__uint8_t reg_md1_c32rm_en;
	__uint8_t reg_md2_c32rm_en;
	__uint8_t reg_clksq0_sel_ctrl;
	__uint8_t reg_clksq1_sel_ctrl;
	__uint8_t reg_srcclken0_en;
	__uint8_t reg_srcclken1_en;
	__uint8_t reg_sysclk0_src_mask_b;
	__uint8_t reg_sysclk1_src_mask_b;

	/* SPM_SRC_REQ */
	__uint8_t reg_spm_apsrc_req;
	__uint8_t reg_spm_f26m_req;
	__uint8_t reg_spm_infra_req;
	__uint8_t reg_spm_ddren_req;
	__uint8_t reg_spm_vrf18_req;
	__uint8_t reg_spm_dvfs_level0_req;
	__uint8_t reg_spm_dvfs_level1_req;
	__uint8_t reg_spm_dvfs_level2_req;
	__uint8_t reg_spm_dvfs_level3_req;
	__uint8_t reg_spm_dvfs_level4_req;
	__uint8_t reg_spm_sspm_mailbox_req;
	__uint8_t reg_spm_sw_mailbox_req;
	__uint8_t reg_spm_cksel2_req;
	__uint8_t reg_spm_cksel3_req;

	/* SPM_SRC_MASK */
	__uint8_t reg_csyspwreq_mask;
	__uint8_t reg_md_srcclkena_0_infra_mask_b;
	__uint8_t reg_md_srcclkena_1_infra_mask_b;
	__uint8_t reg_md_apsrc_req_0_infra_mask_b;
	__uint8_t reg_md_apsrc_req_1_infra_mask_b;
	__uint8_t reg_conn_srcclkena_infra_mask_b;
	__uint8_t reg_conn_infra_req_mask_b;
	__uint8_t reg_sspm_srcclkena_infra_mask_b;
	__uint8_t reg_sspm_infra_req_mask_b;
	__uint8_t reg_scp_srcclkena_infra_mask_b;
	__uint8_t reg_scp_infra_req_mask_b;
	__uint8_t reg_srcclkeni0_infra_mask_b;
	__uint8_t reg_srcclkeni1_infra_mask_b;
	__uint8_t reg_srcclkeni2_infra_mask_b;
	__uint8_t reg_ccif0_md_event_mask_b;
	__uint8_t reg_ccif0_ap_event_mask_b;
	__uint8_t reg_ccif1_md_event_mask_b;
	__uint8_t reg_ccif1_ap_event_mask_b;
	__uint8_t reg_ccif2_md_event_mask_b;
	__uint8_t reg_ccif2_ap_event_mask_b;
	__uint8_t reg_ccif3_md_event_mask_b;
	__uint8_t reg_ccif3_ap_event_mask_b;
	__uint8_t reg_ccifmd_md1_event_mask_b;
	__uint8_t reg_ccifmd_md2_event_mask_b;
	__uint8_t reg_c2k_ps_rccif_wake_mask_b;
	__uint8_t reg_c2k_l1_rccif_wake_mask_b;
	__uint8_t reg_ps_c2k_rccif_wake_mask_b;
	__uint8_t reg_l1_c2k_rccif_wake_mask_b;
	__uint8_t reg_disp2_req_mask_b;
	__uint8_t reg_md_ddr_en_0_mask_b;
	__uint8_t reg_md_ddr_en_1_mask_b;
	__uint8_t reg_conn_ddr_en_mask_b;

	/* SPM_SRC2_MASK */
	__uint8_t reg_disp0_req_mask_b;
	__uint8_t reg_disp1_req_mask_b;
	__uint8_t reg_disp_od_req_mask_b;
	__uint8_t reg_mfg_req_mask_b;
	__uint8_t reg_vdec0_req_mask_b;
	__uint8_t reg_gce_req_mask_b;
	__uint8_t reg_gce_vrf18_req_mask_b;
	__uint8_t reg_lpdma_req_mask_b;
	__uint8_t reg_conn_srcclkena_cksel2_mask_b;
	__uint8_t reg_sspm_apsrc_req_ddren_mask_b;
	__uint8_t reg_scp_apsrc_req_ddren_mask_b;
	__uint8_t reg_md_vrf18_req_0_mask_b;
	__uint8_t reg_md_vrf18_req_1_mask_b;
	__uint8_t reg_next_dvfs_level0_mask_b;
	__uint8_t reg_next_dvfs_level1_mask_b;
	__uint8_t reg_next_dvfs_level2_mask_b;
	__uint8_t reg_next_dvfs_level3_mask_b;
	__uint8_t reg_next_dvfs_level4_mask_b;
	__uint8_t reg_sw2spm_int0_mask_b;
	__uint8_t reg_sw2spm_int1_mask_b;
	__uint8_t reg_sw2spm_int2_mask_b;
	__uint8_t reg_sw2spm_int3_mask_b;
	__uint8_t reg_sspm2spm_int0_mask_b;
	__uint8_t reg_sspm2spm_int1_mask_b;
	__uint8_t reg_sspm2spm_int2_mask_b;
	__uint8_t reg_sspm2spm_int3_mask_b;
	__uint8_t reg_dqssoc_req_mask_b;
	__uint8_t reg_gce_vrf18_req2_mask_b;
	__uint8_t reg_ufs_srcclkena_mask_b;
	__uint8_t reg_ufs_vrf18_req_mask_b;

	/* SPM_SRC3_MASK */
	__uint8_t reg_mpwfi_op;
	__uint8_t reg_spm_resource_req_rsv1_4_mask_b;
	__uint8_t reg_spm_resource_req_rsv1_3_mask_b;
	__uint8_t reg_spm_resource_req_rsv1_2_mask_b;
	__uint8_t reg_spm_resource_req_rsv1_1_mask_b;
	__uint8_t reg_spm_resource_req_rsv1_0_mask_b;
	__uint8_t reg_spm_resource_req_rsv0_4_mask_b;
	__uint8_t reg_spm_resource_req_rsv0_3_mask_b;
	__uint8_t reg_spm_resource_req_rsv0_2_mask_b;
	__uint8_t reg_spm_resource_req_rsv0_1_mask_b;
	__uint8_t reg_spm_resource_req_rsv0_0_mask_b;
	__uint8_t reg_srcclkeni2_cksel3_mask_b;
	__uint8_t reg_srcclkeni2_cksel2_mask_b;
	__uint8_t reg_srcclkeni1_cksel3_mask_b;
	__uint8_t reg_srcclkeni1_cksel2_mask_b;
	__uint8_t reg_srcclkeni0_cksel3_mask_b;
	__uint8_t reg_srcclkeni0_cksel2_mask_b;
	__uint8_t reg_md_ddr_en_0_dbc_en;
	__uint8_t reg_md_ddr_en_1_dbc_en;
	__uint8_t reg_conn_ddr_en_dbc_en;
	__uint8_t reg_sspm_mask_b;
	__uint8_t reg_md_0_mask_b;
	__uint8_t reg_md_1_mask_b;
	__uint8_t reg_scp_mask_b;
	__uint8_t reg_srcclkeni0_mask_b;
	__uint8_t reg_srcclkeni1_mask_b;
	__uint8_t reg_srcclkeni2_mask_b;
	__uint8_t reg_md_apsrc_1_sel;
	__uint8_t reg_md_apsrc_0_sel;
	__uint8_t reg_conn_mask_b;
	__uint8_t reg_conn_apsrc_sel;
	__uint8_t reg_md_srcclkena_0_vrf18_mask_b;

	/* SPM_SRC4_MASK */
	__uint8_t reg_ccif4_md_event_mask_b;
	__uint8_t reg_ccif4_ap_event_mask_b;
	__uint8_t reg_ccif5_md_event_mask_b;
	__uint8_t reg_ccif5_ap_event_mask_b;

	/* SPM_WAKEUP_EVENT_MASK */
	__uint32_t reg_wakeup_event_mask;

	/* SPM_EXT_WAKEUP_EVENT_MASK */
	__uint32_t reg_ext_wakeup_event_mask;

	/* MCU0_WFI_EN */
	__uint8_t mcu0_wfi_en;

	/* MCU1_WFI_EN */
	__uint8_t mcu1_wfi_en;

	/* MCU2_WFI_EN */
	__uint8_t mcu2_wfi_en;

	/* MCU3_WFI_EN */
	__uint8_t mcu3_wfi_en;

	/* MCU4_WFI_EN */
	__uint8_t mcu4_wfi_en;

	/* MCU5_WFI_EN */
	__uint8_t mcu5_wfi_en;

	/* MCU6_WFI_EN */
	__uint8_t mcu6_wfi_en;

	/* MCU7_WFI_EN */
	__uint8_t mcu7_wfi_en;

	/* MCU8_WFI_EN */
	__uint8_t mcu8_wfi_en;

	/* MCU9_WFI_EN */
	__uint8_t mcu9_wfi_en;

	/* MCU10_WFI_EN */
	__uint8_t mcu10_wfi_en;

	/* MCU11_WFI_EN */
	__uint8_t mcu11_wfi_en;

	/* MCU12_WFI_EN */
	__uint8_t mcu12_wfi_en;

	/* MCU13_WFI_EN */
	__uint8_t mcu13_wfi_en;

	/* MCU14_WFI_EN */
	__uint8_t mcu14_wfi_en;

	/* MCU15_WFI_EN */
	__uint8_t mcu15_wfi_en;

	/* MCU16_WFI_EN */
	__uint8_t mcu16_wfi_en;

	/* MCU17_WFI_EN */
	__uint8_t mcu17_wfi_en;

	/* SPM_RSV_CON2 */
	__uint32_t spm_rsv_con2;

	/* Auto-gen End */
};

/* code gen by spm_pwr_ctrl_atf.pl, need struct pwr_ctrl */
enum pwr_ctrl_enum {
	PWR_PCM_FLAGS,
	PWR_PCM_FLAGS_CUST,
	PWR_PCM_FLAGS_CUST_SET,
	PWR_PCM_FLAGS_CUST_CLR,
	PWR_PCM_FLAGS1,
	PWR_PCM_FLAGS1_CUST,
	PWR_PCM_FLAGS1_CUST_SET,
	PWR_PCM_FLAGS1_CUST_CLR,
	PWR_TIMER_VAL,
	PWR_TIMER_VAL_CUST,
	PWR_TIMER_VAL_RAMP_EN,
	PWR_TIMER_VAL_RAMP_EN_SEC,
	PWR_WAKE_SRC,
	PWR_WAKE_SRC_CUST,
	PWR_OPP_LEVEL,
	PWR_WDT_DISABLE,
	PWR_SYSPWREQ_MASK,
	PWR_REG_SRCCLKEN0_CTL,
	PWR_REG_SRCCLKEN1_CTL,
	PWR_REG_SPM_LOCK_INFRA_DCM,
	PWR_REG_SRCCLKEN_MASK,
	PWR_REG_MD1_C32RM_EN,
	PWR_REG_MD2_C32RM_EN,
	PWR_REG_CLKSQ0_SEL_CTRL,
	PWR_REG_CLKSQ1_SEL_CTRL,
	PWR_REG_SRCCLKEN0_EN,
	PWR_REG_SRCCLKEN1_EN,
	PWR_REG_SYSCLK0_SRC_MASK_B,
	PWR_REG_SYSCLK1_SRC_MASK_B,
	PWR_REG_SPM_APSRC_REQ,
	PWR_REG_SPM_F26M_REQ,
	PWR_REG_SPM_INFRA_REQ,
	PWR_REG_SPM_DDREN_REQ,
	PWR_REG_SPM_VRF18_REQ,
	PWR_REG_SPM_DVFS_LEVEL0_REQ,
	PWR_REG_SPM_DVFS_LEVEL1_REQ,
	PWR_REG_SPM_DVFS_LEVEL2_REQ,
	PWR_REG_SPM_DVFS_LEVEL3_REQ,
	PWR_REG_SPM_DVFS_LEVEL4_REQ,
	PWR_REG_SPM_SSPM_MAILBOX_REQ,
	PWR_REG_SPM_SW_MAILBOX_REQ,
	PWR_REG_SPM_CKSEL2_REQ,
	PWR_REG_SPM_CKSEL3_REQ,
	PWR_REG_CSYSPWREQ_MASK,
	PWR_REG_MD_SRCCLKENA_0_INFRA_MASK_B,
	PWR_REG_MD_SRCCLKENA_1_INFRA_MASK_B,
	PWR_REG_MD_APSRC_REQ_0_INFRA_MASK_B,
	PWR_REG_MD_APSRC_REQ_1_INFRA_MASK_B,
	PWR_REG_CONN_SRCCLKENA_INFRA_MASK_B,
	PWR_REG_CONN_INFRA_REQ_MASK_B,
	PWR_REG_SSPM_SRCCLKENA_INFRA_MASK_B,
	PWR_REG_SSPM_INFRA_REQ_MASK_B,
	PWR_REG_SCP_SRCCLKENA_INFRA_MASK_B,
	PWR_REG_SCP_INFRA_REQ_MASK_B,
	PWR_REG_SRCCLKENI0_INFRA_MASK_B,
	PWR_REG_SRCCLKENI1_INFRA_MASK_B,
	PWR_REG_SRCCLKENI2_INFRA_MASK_B,
	PWR_REG_CCIF0_MD_EVENT_MASK_B,
	PWR_REG_CCIF0_AP_EVENT_MASK_B,
	PWR_REG_CCIF1_MD_EVENT_MASK_B,
	PWR_REG_CCIF1_AP_EVENT_MASK_B,
	PWR_REG_CCIF2_MD_EVENT_MASK_B,
	PWR_REG_CCIF2_AP_EVENT_MASK_B,
	PWR_REG_CCIF3_MD_EVENT_MASK_B,
	PWR_REG_CCIF3_AP_EVENT_MASK_B,
	PWR_REG_CCIFMD_MD1_EVENT_MASK_B,
	PWR_REG_CCIFMD_MD2_EVENT_MASK_B,
	PWR_REG_C2K_PS_RCCIF_WAKE_MASK_B,
	PWR_REG_C2K_L1_RCCIF_WAKE_MASK_B,
	PWR_REG_PS_C2K_RCCIF_WAKE_MASK_B,
	PWR_REG_L1_C2K_RCCIF_WAKE_MASK_B,
	PWR_REG_DISP2_REQ_MASK_B,
	PWR_REG_MD_DDR_EN_0_MASK_B,
	PWR_REG_MD_DDR_EN_1_MASK_B,
	PWR_REG_CONN_DDR_EN_MASK_B,
	PWR_REG_DISP0_REQ_MASK_B,
	PWR_REG_DISP1_REQ_MASK_B,
	PWR_REG_DISP_OD_REQ_MASK_B,
	PWR_REG_MFG_REQ_MASK_B,
	PWR_REG_VDEC0_REQ_MASK_B,
	PWR_REG_GCE_REQ_MASK_B,
	PWR_REG_GCE_VRF18_REQ_MASK_B,
	PWR_REG_LPDMA_REQ_MASK_B,
	PWR_REG_CONN_SRCCLKENA_CKSEL2_MASK_B,
	PWR_REG_SSPM_APSRC_REQ_DDREN_MASK_B,
	PWR_REG_SCP_APSRC_REQ_DDREN_MASK_B,
	PWR_REG_MD_VRF18_REQ_0_MASK_B,
	PWR_REG_MD_VRF18_REQ_1_MASK_B,
	PWR_REG_NEXT_DVFS_LEVEL0_MASK_B,
	PWR_REG_NEXT_DVFS_LEVEL1_MASK_B,
	PWR_REG_NEXT_DVFS_LEVEL2_MASK_B,
	PWR_REG_NEXT_DVFS_LEVEL3_MASK_B,
	PWR_REG_NEXT_DVFS_LEVEL4_MASK_B,
	PWR_REG_SW2SPM_INT0_MASK_B,
	PWR_REG_SW2SPM_INT1_MASK_B,
	PWR_REG_SW2SPM_INT2_MASK_B,
	PWR_REG_SW2SPM_INT3_MASK_B,
	PWR_REG_SSPM2SPM_INT0_MASK_B,
	PWR_REG_SSPM2SPM_INT1_MASK_B,
	PWR_REG_SSPM2SPM_INT2_MASK_B,
	PWR_REG_SSPM2SPM_INT3_MASK_B,
	PWR_REG_DQSSOC_REQ_MASK_B,
	PWR_REG_GCE_VRF18_REQ2_MASK_B,
	PWR_REG_UFS_SRCCLKENA_MASK_B,
	PWR_REG_UFS_VRF18_REQ_MASK_B,

	/* SPM_SRC3_MASK */
	PWR_REG_MPWFI_OP,
	PWR_REG_SPM_RESOURCE_REQ_RSV1_4_MASK_B,
	PWR_REG_SPM_RESOURCE_REQ_RSV1_3_MASK_B,
	PWR_REG_SPM_RESOURCE_REQ_RSV1_2_MASK_B,
	PWR_REG_SPM_RESOURCE_REQ_RSV1_1_MASK_B,
	PWR_REG_SPM_RESOURCE_REQ_RSV1_0_MASK_B,
	PWR_REG_SPM_RESOURCE_REQ_RSV0_4_MASK_B,
	PWR_REG_SPM_RESOURCE_REQ_RSV0_3_MASK_B,
	PWR_REG_SPM_RESOURCE_REQ_RSV0_2_MASK_B,
	PWR_REG_SPM_RESOURCE_REQ_RSV0_1_MASK_B,
	PWR_REG_SPM_RESOURCE_REQ_RSV0_0_MASK_B,
	PWR_REG_SRCCLKENI2_CKSEL3_MASK_B,
	PWR_REG_SRCCLKENI2_CKSEL2_MASK_B,
	PWR_REG_SRCCLKENI1_CKSEL3_MASK_B,
	PWR_REG_SRCCLKENI1_CKSEL2_MASK_B,
	PWR_REG_SRCCLKENI0_CKSEL3_MASK_B,
	PWR_REG_SRCCLKENI0_CKSEL2_MASK_B,
	PWR_REG_MD_DDR_EN_0_DBC_EN,
	PWR_REG_MD_DDR_EN_1_DBC_EN,
	PWR_REG_CONN_DDR_EN_DBC_EN,
	PWR_REG_SSPM_MASK_B,
	PWR_REG_MD_0_MASK_B,
	PWR_REG_MD_1_MASK_B,
	PWR_REG_SCP_MASK_B,
	PWR_REG_SRCCLKENI0_MASK_B,
	PWR_REG_SRCCLKENI1_MASK_B,
	PWR_REG_SRCCLKENI2_MASK_B,
	PWR_REG_MD_APSRC_1_SEL,
	PWR_REG_MD_APSRC_0_SEL,
	PWR_REG_CONN_MASK_B,
	PWR_REG_CONN_APSRC_SEL,
	PWR_REG_MD_SRCCLKENA_0_VRF18_MASK_B,
	PWR_REG_CCIF4_MD_EVENT_MASK_B,
	PWR_REG_CCIF4_AP_EVENT_MASK_B,
	PWR_REG_CCIF5_MD_EVENT_MASK_B,
	PWR_REG_CCIF5_AP_EVENT_MASK_B,
	PWR_REG_WAKEUP_EVENT_MASK,
	PWR_REG_EXT_WAKEUP_EVENT_MASK,
	PWR_MCU0_WFI_EN,
	PWR_MCU1_WFI_EN,
	PWR_MCU2_WFI_EN,
	PWR_MCU3_WFI_EN,
	PWR_MCU4_WFI_EN,
	PWR_MCU5_WFI_EN,
	PWR_MCU6_WFI_EN,
	PWR_MCU7_WFI_EN,
	PWR_MCU8_WFI_EN,
	PWR_MCU9_WFI_EN,
	PWR_MCU10_WFI_EN,
	PWR_MCU11_WFI_EN,
	PWR_MCU12_WFI_EN,
	PWR_MCU13_WFI_EN,
	PWR_MCU14_WFI_EN,
	PWR_MCU15_WFI_EN,
	PWR_MCU16_WFI_EN,
	PWR_MCU17_WFI_EN,
	PWR_SPM_RSV_CON2,
	PWR_MAX_COUNT,
};

enum {
	SPM_SUSPEND,
	SPM_RESUME,
	SPM_DPIDLE_ENTER,
	SPM_DPIDLE_LEAVE,
	SPM_ENTER_SODI,
	SPM_LEAVE_SODI,
	SPM_ENTER_SODI3,
	SPM_LEAVE_SODI3,
	SPM_ENTER_MCSODI,
	SPM_LEAVE_MCSODI,
	SPM_SUSPEND_PREPARE,
	SPM_POST_SUSPEND,
	SPM_DPIDLE_PREPARE,
	SPM_POST_DPIDLE,
	SPM_SODI_PREPARE,
	SPM_POST_SODI,
	SPM_SODI3_PREPARE,
	SPM_POST_SODI3,
	SPM_MCSODI_PREPARE,
	SPM_POST_MCSODI,
	SPM_VCORE_PWARP_CMD,
	SPM_PWR_CTRL_SUSPEND,
	SPM_PWR_CTRL_DPIDLE,
	SPM_PWR_CTRL_SODI,
	SPM_PWR_CTRL_SODI3,
	SPM_PWR_CTRL_MCSODI,
	SPM_PWR_CTRL_VCOREFS,
};

#define DYNA_LOAD_PCM_PATH_SIZE 128
#define PCM_FIRMWARE_VERSION_SIZE 128

enum dyna_load_pcm_index {
	DYNA_LOAD_PCM_SUSPEND = 0,
	DYNA_LOAD_PCM_MAX,
};

struct dyna_load_pcm_t {
	char path[DYNA_LOAD_PCM_PATH_SIZE];
	char version[PCM_FIRMWARE_VERSION_SIZE];
	char *buf;
	struct pcm_desc desc;
	int ready;
};

extern struct dyna_load_pcm_t dyna_load_pcm[DYNA_LOAD_PCM_MAX];

struct wake_status {
	__uint32_t assert_pc;          /* PCM_REG_DATA_INI */
	__uint32_t r12;                /* PCM_REG12_DATA */
	__uint32_t r12_ext;            /* PCM_REG12_DATA */
	__uint32_t raw_sta;            /* SLEEP_ISR_RAW_STA */
	__uint32_t raw_ext_sta;        /* SPM_WAKEUP_EXT_STA */
	__uint32_t wake_misc;          /* SLEEP_WAKEUP_MISC */
	__uint32_t timer_out;          /* PCM_TIMER_OUT */
	__uint32_t r13;                /* PCM_REG13_DATA */
	__uint32_t idle_sta;           /* SLEEP_SUBSYS_IDLE_STA */
	__uint32_t debug_flag;         /* PCM_PASR_DPD_3 */
	__uint32_t event_reg;          /* PCM_EVENT_REG_STA */
	__uint32_t isr;                /* SLEEP_ISR_STATUS */
	__uint32_t log_index;
};

struct spm_lp_scen {
	struct pcm_desc *pcmdesc;
	struct pwr_ctrl *pwrctrl;
};

extern struct spm_lp_scen __spm_suspend;
extern struct spm_lp_scen __spm_vcorefs;
extern struct spm_lp_scen __spm_dpidle;
#if 0
extern struct spm_lp_scen __spm_sodi3;
#endif
extern struct spm_lp_scen __spm_sodi;

extern int __spm_get_spmfw_idx(void);
extern void __spm_set_cpu_status(int cpu);
extern void __spm_reset_and_init_pcm(const struct pcm_desc *pcmdesc);
extern void __spm_kick_im_to_fetch(const struct pcm_desc *pcmdesc);

extern void __spm_init_pcm_register(void);	/* init r0 and r7 */
extern void __spm_init_event_vector(const struct pcm_desc *pcmdesc);
extern void __spm_src_req_update(const struct pwr_ctrl *pwrctrl, unsigned int resource_usage);
extern void __spm_set_power_control(const struct pwr_ctrl *pwrctrl);
extern void __spm_set_wakeup_event(const struct pwr_ctrl *pwrctrl);
extern void __spm_kick_pcm_to_run(struct pwr_ctrl *pwrctrl);

extern void __spm_get_wakeup_status(struct wake_status *wakesta);
extern void __spm_clean_after_wakeup(void);
extern wake_reason_t __spm_output_wake_reason(const struct wake_status *wakesta,
		const struct pcm_desc *pcmdesc);

extern void __spm_sync_vcore_dvfs_power_control(struct pwr_ctrl *dest_pwr_ctrl, const struct pwr_ctrl *src_pwr_ctrl);

extern void __spm_set_pcm_wdt(int en);
extern __uint32_t _spm_get_wake_period(int pwake_time, wake_reason_t last_wr);


static inline void set_pwrctrl_pcm_flags(struct pwr_ctrl *pwrctrl, __uint32_t flags)
{
	if (pwrctrl->pcm_flags_cust == 0)
		pwrctrl->pcm_flags = flags;
	else
		pwrctrl->pcm_flags = pwrctrl->pcm_flags_cust;
}

static inline void set_pwrctrl_pcm_flags1(struct pwr_ctrl *pwrctrl, __uint32_t flags)
{
	if (pwrctrl->pcm_flags1_cust == 0)
		pwrctrl->pcm_flags1 = flags;
	else
		pwrctrl->pcm_flags1 = pwrctrl->pcm_flags1_cust;
}

#endif /* __MT_SPM_INTERNAL__ */
