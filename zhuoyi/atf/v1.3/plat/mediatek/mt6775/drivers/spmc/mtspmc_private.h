#ifndef _MTSPMC_PRIVATE_H_
#define _MTSPMC_PRIVATE_H_

uint64_t read_cpuectlr(void);
void write_cpuectlr(uint64_t);

/*
 * per_cpu/cluster helper
 */
struct per_cpu_reg {
	int cluster_addr;
	int cpu_stride;
};

#define per_cpu(cluster, cpu, reg) (reg[cluster].cluster_addr + (cpu << reg[cluster].cpu_stride))
#define per_cluster(cluster, reg) (reg[cluster].cluster_addr)

/* SPMC related registers */
#define SPM_POWERON_CONFIG_EN              (SPM_BASE + 0x000)
/* bit-fields of SPM_POWERON_CONFIG_EN */
#define SPM_PROJECT_CODE                   0xB16
#define BCLK_CG_EN                         (1 << 0)

#define SPM_PWR_STATUS                     (SPM_BASE + 0x190)
#define SPM_CPU_PWR_STATUS                 (SPM_BASE + 0x198)
#define SPM_CPU_PWR_STATUS_2ND             (SPM_BASE + 0x19c)

/* bit-fields of SPM_CPU_PWR_STATUS */
#define MCU0_CPU_PWR_STA_MASK              (1 << 0)
#define MCU1_CPU_PWR_STA_MASK              (1 << 1)
#define MCU2_CPU_PWR_STA_MASK              (1 << 2)
#define MCU3_CPU_PWR_STA_MASK              (1 << 3)
#define MCU4_CPU_PWR_STA_MASK              (1 << 4)
#define MCU5_CPU_PWR_STA_MASK              (1 << 5)
#define MCU6_CPU_PWR_STA_MASK              (1 << 6)
#define MCU7_CPU_PWR_STA_MASK              (1 << 7)
#define MCU8_CPU_PWR_STA_MASK              (1 << 8)
#define MCU9_CPU_PWR_STA_MASK              (1 << 9)
#define MCU10_CPU_PWR_STA_MASK             (1 << 10)
#define MCU11_CPU_PWR_STA_MASK             (1 << 11)

#define SPM_BYPASS_SPMC                    (SPM_BASE + 0x26c)

#define SPM_MCU0_PWR_CON                   (SPM_BASE + 0x0200)
#define SPM_MCU1_PWR_CON                   (SPM_BASE + 0x0204) /* MCUSYS */
#define SPM_MCU2_PWR_CON                   (SPM_BASE + 0x0208) /* MP0_CPUSYS */
#define SPM_MCU3_PWR_CON                   (SPM_BASE + 0x020C) /* MP0_CORE0 */
#define SPM_MCU4_PWR_CON                   (SPM_BASE + 0x0210)
#define SPM_MCU5_PWR_CON                   (SPM_BASE + 0x0214)
#define SPM_MCU6_PWR_CON                   (SPM_BASE + 0x0218)
#define SPM_MCU7_PWR_CON                   (SPM_BASE + 0x021C) /* MP2_CPUSYS */
#define SPM_MCU8_PWR_CON                   (SPM_BASE + 0x0220) /* MP2_CORE0 */
#define SPM_MCU9_PWR_CON                   (SPM_BASE + 0x0224)
#define SPM_MCU10_PWR_CON                  (SPM_BASE + 0x0228)
#define SPM_MCU11_PWR_CON                  (SPM_BASE + 0x022C)
#define SPM_MCU12_PWR_CON                  (SPM_BASE + 0x0230)
#define SPM_MCU13_PWR_CON                  (SPM_BASE + 0x0234)
#define SPM_MCU14_PWR_CON                  (SPM_BASE + 0x0238)
#define SPM_MCU15_PWR_CON                  (SPM_BASE + 0x023C)
#define SPM_MCU16_PWR_CON                  (SPM_BASE + 0x0240)
#define SPM_MCU17_PWR_CON                  (SPM_BASE + 0x0244)

#define SPM_CPU_EXT_BUCK_ISO               (SPM_BASE + 0x0260)

/* bit-fields of SPM_CPU_EXT_BUCK_ISO */
#define MP0_EXT_BUCK_ISO                   (1 << 0)
#define MP1_EXT_BUCK_ISO                   (1 << 1)
#define MP_EXT_BUCK_ISO                    (1 << 2)


#define SPM_SPMC_DORMANT_ENABLE            (SPM_BASE + 0x0268)
/* bit-fields of SPM_SPMC_DORMANT_ENABLE */
#define MP0_SPMC_SRAM_DORMANT_EN           (1 << 0)
#define MP1_SPMC_SRAM_DORMANT_EN           (1 << 1)

/* bit-fields of SPM_MCU?_PWR_CON */
#define PWRCTRL_PWR_RST_B                  (1 << 0)
#define PWRCTRL_PWR_ISO                    (1 << 1)
#define PWRCTRL_PWR_ON                     (1 << 2)
#define PWRCTRL_PWR_ON_2ND                 (1 << 3)
#define PWRCTRL_PWR_CLK_DIS                (1 << 4)
#define PWRCTRL_SRAM_CKISO                 (1 << 5)
#define PWRCTRL_SRAM_ISOINT_B              (1 << 6)
#define PWRCTRL_SRAM_PD_SLPB_CLAMP         (1 << 7)
#define PWRCTRL_SRAM_PDN                   (1 << 8)
#define PWRCTRL_SRAM_SLEEP_B               (1 << 12)
#define PWRCTRL_SRAM_PDN_ACK               (1 << 24)
#define PWRCTRL_SRAM_SLEEP_B_ACK           (1 << 28)
#define PWRCTRL_PWR_ACK                    (1 << 30)
#define PWRCTRL_PWR_ACK_2ND                (1 << 31)

/* per_cpu registers for SPM_MCU?_PWR_CON */
static const struct per_cpu_reg SPM_CPU_PWR[] = {
	[0] = { .cluster_addr = SPM_MCU3_PWR_CON, .cpu_stride = 2 },
	[1] = { .cluster_addr = SPM_MCU8_PWR_CON, .cpu_stride = 2 },
};

/* per_cluster registers for SPM_MCU?_PWR_CON */
static const struct per_cpu_reg SPM_CLUSTER_PWR[] = {
	[0] = { .cluster_addr = SPM_MCU2_PWR_CON, },
	[1] = { .cluster_addr = SPM_MCU7_PWR_CON, },
};

/* APB Module infracfg_ao */
#define INFRA_MCUSYS_PROTECTEN            (INFRACFG_AO_BASE + 0x2b0)
#define INFRA_MCUSYS_PROTECTEN_SET        (INFRACFG_AO_BASE + 0x2b4)
#define INFRA_MCUSYS_PROTECTEN_CLR        (INFRACFG_AO_BASE + 0x2b8)
#define INFRA_MCUSYS_PROTECTEN_STA0       (INFRACFG_AO_BASE + 0x2bc)
#define INFRA_MCUSYS_PROTECTEN_STA1       (INFRACFG_AO_BASE + 0x2c0)

#define MCU2_PROT_BIT_MASK                ((1 << 0)|(1 << 1)|(1 << 2)|(1 << 3))
#define MCU2_PROT_BIT_ACK_MASK            ((1 << 0)|(1 << 1)|(1 << 2)|(1 << 3))
#define MCU7_PROT_BIT_MASK                ((1 << 4)|(1 << 5)|(1 << 6)|(1 << 7))
#define MCU7_PROT_BIT_ACK_MASK            ((1 << 4)|(1 << 5)|(1 << 6)|(1 << 7))

/*
 * MCU configuration registers
 */
#define MCUCFG_MP0_AXI_CONFIG                   MP0_AXI_CONFIG
#define MCUCFG_MP1_AXI_CONFIG                   MP1_AXI_CONFIG
/* bit-fields of MCUCFG_MP?_AXI_CONFIG */
#define MCUCFG_AXI_CONFIG_BROADCASTINNER        (1 << 0)
#define MCUCFG_AXI_CONFIG_BROADCASTOUTER        (1 << 1)
#define MCUCFG_AXI_CONFIG_BROADCASTCACHEMAINT   (1 << 2)
#define MCUCFG_AXI_CONFIG_SYSBARDISABLE         (1 << 3)
#define MCUCFG_AXI_CONFIG_ACINACTM              (1 << 4)
#define MCUCFG_AXI_CONFIG_AINACTS               (1 << 5)

/* per_cpu registers for MCUCFG_MP?_AXI_CONFIG */
static const struct per_cpu_reg MCUCFG_SCUCTRL[] = {
	[0] = { .cluster_addr = MCUCFG_MP0_AXI_CONFIG },
	[1] = { .cluster_addr = MCUCFG_MP1_AXI_CONFIG },
};

#define MCUCFG_MP0_MISC_CONFIG2         MP0_MISC_CONFIG2
#define MCUCFG_MP0_MISC_CONFIG3         MP0_MISC_CONFIG3
#define MCUCFG_MP1_MISC_CONFIG2         MP1_MISC_CONFIG2
#define MCUCFG_MP1_MISC_CONFIG3         MP1_MISC_CONFIG3

/* SPARK for MP0 */
#define MCUCFG_CPUSYS0_SPARKVRETCNTRL         (MCUCFG_BASE + 0x1c00)
/* bit-fields of MCUCFG_CPUSYS0_SPARKVRETCNTRL */
#define CPU0_SPARK_VRET_CTRL                  (0x3f << 0)
#define CPU1_SPARK_VRET_CTRL                  (0x3f << 8)
#define CPU2_SPARK_VRET_CTRL                  (0x3f << 16)
#define CPU3_SPARK_VRET_CTRL                  (0x3f << 24)

/* SPARK control in little cores */
#define MCUCFG_CPUSYS0_CPU0_SPMC_CTL          (MCUCFG_BASE + 0x1c30)
#define MCUCFG_CPUSYS0_CPU1_SPMC_CTL          (MCUCFG_BASE + 0x1c34)
#define MCUCFG_CPUSYS0_CPU2_SPMC_CTL          (MCUCFG_BASE + 0x1c38)
#define MCUCFG_CPUSYS0_CPU3_SPMC_CTL          (MCUCFG_BASE + 0x1c3c)
/* bit-fields of MCUCFG_CPUSYS0_CPU?_SPMC_CTL */
#define SW_SPARK_EN                           (1 << 0)
#define SW_NO_WAIT_Q                          (1 << 1)

/* the BIG cores use MCUCFG_BASE+0x2000 as its MCUCFG top */
#define MCUCFG_MP2_BASE                       (MCUCFG_BASE + 0x2000)
#define MCUCFG_MP2_PWR_RST_CTL                (MCUCFG_MP2_BASE + 0x8)
/* bit-fields of MCUCFG_MP2_PWR_RST_CTL */
#define SW_RST_B                              (1 << 0)
#define TOPAON_APB_MASK                       (1 << 1)

#define MCUCFG_MP2_CPUCFG                     (MCUCFG_MP2_BASE + 0x208)
#define MCUCFG_MP2_RVADDR0                    (MCUCFG_MP2_BASE + 0x290)
#define MCUCFG_MP2_RVADDR1                    (MCUCFG_MP2_BASE + 0x298)
#define MCUCFG_MP2_RVADDR2                    (MCUCFG_MP2_BASE + 0x2c0)
#define MCUCFG_MP2_RVADDR3                    (MCUCFG_MP2_BASE + 0x2c8)

/* per_cpu registers for MCUCFG_MP?_MISC_CONFIG2 */
static const struct per_cpu_reg MCUCFG_BOOTADDR[] = {
	[0] = { .cluster_addr = MCUCFG_MP0_MISC_CONFIG2, .cpu_stride = 3 },
};

/* per_cpu registers for MCUCFG_MP?_MISC_CONFIG3 */
static const struct per_cpu_reg MCUCFG_INITARCH[] = {
	[0] = { .cluster_addr = MCUCFG_MP0_MISC_CONFIG3 },
	[1] = { .cluster_addr = MCUCFG_MP2_CPUCFG },
};

/* SPARK control in BIG cores */
#define MCUCFG_MP2_CA15M_MON_SEL              (MCUCFG_MP2_BASE + 0x400)
#define MCUCFG_MP2_PTP3_CPU0_SPMC0            (MCUCFG_MP2_BASE + 0x430)
#define MCUCFG_MP2_PTP3_CPU0_SPMC1            (MCUCFG_MP2_BASE + 0x434)
#define MCUCFG_MP2_PTP3_CPU1_SPMC0            (MCUCFG_MP2_BASE + 0x438)
#define MCUCFG_MP2_PTP3_CPU1_SPMC1            (MCUCFG_MP2_BASE + 0x43c)
#define MCUCFG_MP2_PTP3_CPU2_SPMC0            (MCUCFG_MP2_BASE + 0x440)
#define MCUCFG_MP2_PTP3_CPU2_SPMC1            (MCUCFG_MP2_BASE + 0x444)
#define MCUCFG_MP2_PTP3_CPU3_SPMC0            (MCUCFG_MP2_BASE + 0x448)
#define MCUCFG_MP2_PTP3_CPU3_SPMC1            (MCUCFG_MP2_BASE + 0x44c)
/* bit-fields of MCUCFG_MP2_PTP3_CPU?_SPMC? */
#define SW_SPARK_EN                           (1 << 0)
#define SW_NO_WAIT_Q                          (1 << 1)

#define MCUCFG_MP2_SPARK2LDO                  (MCUCFG_MP2_BASE + 0x700)
/* bit-fields of MCUCFG_MP2_SPARK2LDO */
#define SPARK_VRET_CTRL                       (0x3f << 0)
#define CPU0_SPARK_LDO_AMUXSEL                (0xf  << 6)
#define CPU1_SPARK_LDO_AMUXSEL                (0xf  << 10)
#define CPU2_SPARK_LDO_AMUXSEL                (0xf  << 14)
#define CPU3_SPARK_LDO_AMUXSEL                (0xf  << 18)

/* per_cpu registers for SPARK */
static const struct per_cpu_reg MCUCFG_SPARK[] = {
	[0] = { .cluster_addr = MCUCFG_CPUSYS0_CPU0_SPMC_CTL, .cpu_stride = 2 },
	[1] = { .cluster_addr = MCUCFG_MP2_PTP3_CPU0_SPMC0, .cpu_stride = 3 },
};

/* per_cpu registers for SPARK2LDO */
static const struct per_cpu_reg MCUCFG_SPARK2LDO[] = {
	[0] = { .cluster_addr = MCUCFG_CPUSYS0_SPARKVRETCNTRL },
	[1] = { .cluster_addr = MCUCFG_MP2_SPARK2LDO },
};

#endif
