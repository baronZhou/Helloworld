#include <debug.h>
#include <devapc.h>
#include <platform.h>
#include <plat_private.h>


#if !defined(SPD_no)

/* For all the TEE projects */

static const struct INFRA_PERI_DEVICE_INFO D_APC_INFRA_Devices_TEE[] = {
	/*              module,                                  AP permission,  MD permission,    SCP permission */

	/* 0 */
	DAPC_INFRA_ATTR("INFRA_AO_INFRASYS_CONFIG_REGS",         E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRA_AO_PMIC_WRAP",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("RESERVED",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_KEYPAD",                       E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_APXGPT",                       E_NO_PROTECTION, E_FORBIDDEN,     E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRA_AO_AP_CIRQ_EINT",                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_DEVICE_APC_MPU",               E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_DEVICE_APC_AO",                E_SEC_RW_ONLY,   E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SEJ",                          E_SEC_RW_ONLY,   E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("RESERVED",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),

	/* 10 */
	DAPC_INFRA_ATTR("RESERVED",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_AES_TOP_0",                    E_FORBIDDEN,     E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_AES_TOP_1",                    E_FORBIDDEN,     E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_MDEM_TEMP_SHARE",              E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_SYSTEM_Timer",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRA_AO_CLDMA_AO_TOP_AP",              E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_CLDMA_AO_TOP_MD",              E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_DVFS_CTRL_PROC",               E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_CRYPTO_WRAPPER",               E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSHUB",                        E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),

	/* 20 */
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_0",                       E_FORBIDDEN,     E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_1",                       E_SEC_RW_NS_R,   E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_2",                       E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_3",                       E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_4",                       E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_5",                       E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_6",                       E_FORBIDDEN,     E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_7",                       E_FORBIDDEN,     E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_8",                       E_FORBIDDEN,     E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SLEEP_CONTROL_PROCESSOR_0",    E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),

	/* 30 */
	DAPC_INFRA_ATTR("INFRA_AO_SLEEP_CONTROL_PROCESSOR_1",    E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SLEEP_CONTROL_PROCESSOR_2",    E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_TOP_MODULE",                   E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_SECURITY_GROUP",               E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_EMI_GROUP",                    E_NO_PROTECTION, E_FORBIDDEN,     E_SEC_RW_NS_R),
	DAPC_INFRA_ATTR("INFRASYS_CCIF0_1_CLDMA_AP",             E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_CCIF2_3_AP",                   E_NO_PROTECTION, E_FORBIDDEN,     E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_CFG_GROUP",                    E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_SYS_CIRQ",                     E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),

	/* 40 */
	DAPC_INFRA_ATTR("INFRASYS_GCE_GROUP",                    E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_SMI_GROUP",                    E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_PTP_Thermal_GROUP",            E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_MM_IOMMU_GROUP",               E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_GPIO_GROUP",                   E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_BUS_TRACE_GROUP",              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_EMI_MPU_REG",                  E_SEC_RW_ONLY,   E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_CCIF4_5_AP",                   E_NO_PROTECTION, E_FORBIDDEN,     E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_AUXADC",                       E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),

	/* 50 */
	DAPC_INFRA_ATTR("INFRASYS_BSI_BPI_GROUP",                E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_DRAMC_0_GROUP",                E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("CSI",                                   E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("EAST_IO_R",                             E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("EAST_IO_RT",                            E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("SSUSB_PHY",                             E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("EFUSE",                                 E_SEC_RW_NS_R,   E_SEC_RW_NS_R,   E_FORBIDDEN),
	DAPC_INFRA_ATTR("SOUTH_IO_B",                            E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("SOUTH_IO_BR",                           E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),

	/* 60 */
	DAPC_INFRA_ATTR("MIPI_DSI0",                             E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("MIPI_DSI1",                             E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("WEST_IO_L",                             E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("Reserved",                              E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("UFS MPHY",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("MSDC_PAD_MACRO",                        E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("NORTH_IO_T",                            E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_CCIF0_MD1",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_CCIF1_C2K",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),

	/* 70 */
	DAPC_INFRA_ATTR("INFRASYS_CCIF2_MD1",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_CCIF3_C2K",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_CLDMA_MD",                     E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_ MD1_C2K_CCIF",                E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_C2K_MD1_CCIF",                 E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_CCIF4_MD1",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_CCIF5_C2K",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("RESERVED",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_PDN_MCSIB",                       E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_AUDIO",                        E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),

	/* 80 */
	DAPC_INFRA_ATTR("CONNSYS",                               E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),

};

static const struct INFRA_PERI_DEVICE_INFO D_APC_PERI_Devices_TEE[] = {
	/*              module,                                 AP permission,   MD permission,   SCP permission */

	/* 0 */
	DAPC_PERI_ATTR("APDMA",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Pericfg_reg",                           E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("UART0",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_NO_PROTECTION),
	DAPC_PERI_ATTR("UART1",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_NO_PROTECTION),
	DAPC_PERI_ATTR("UART2",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_NO_PROTECTION),
	DAPC_PERI_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("PWM",                                   E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C0",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C1",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),

	/* 10 */
	DAPC_PERI_ATTR("I2C2 (i3c)",                            E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI6",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C4",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C5",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C6 ( I2C treble)",                    E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI0",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C3",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C8",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI7",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),

	/* 20 */
	DAPC_PERI_ATTR("DISP_PWM0",                             E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI1",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI2",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C7",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Peri_devapc",                           E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI3",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI4",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI5",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),

	/* 30 */
	DAPC_PERI_ATTR("I2C10",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C11",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("CQDMA",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Peri_MBIST",                            E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Peri_ao_MBIST",                         E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C9",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("dx_cc private",                         E_SEC_RW_ONLY,   E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("TRNG",                                  E_SEC_RW_ONLY,   E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("BTIF",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("peri_dfp_top",                          E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),

	/* 40 */
	DAPC_PERI_ATTR("USB2.0",                                E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_PERI_ATTR("MSDC0",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("MSDC1",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("USB2.0 Sub security region",            E_FORBIDDEN,     E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_PERI_ATTR("MSDC3",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("UFS_DEV",                               E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("DEBUG_TOP",                             E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),

};


#else

/* For Non-TEE projects */

static const struct INFRA_PERI_DEVICE_INFO D_APC_INFRA_Devices[] = {
	/*              module,                                  AP permission,   MD permission,   SCP permission */

	/* 0 */
	DAPC_INFRA_ATTR("INFRA_AO_INFRASYS_CONFIG_REGS",         E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRA_AO_PMIC_WRAP",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("RESERVED",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_KEYPAD",                       E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_APXGPT",                       E_NO_PROTECTION, E_FORBIDDEN,     E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRA_AO_AP_CIRQ_EINT",                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_DEVICE_APC_MPU",               E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_DEVICE_APC_AO",                E_SEC_RW_ONLY,   E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SEJ",                          E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("RESERVED",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),

	/* 10 */
	DAPC_INFRA_ATTR("RESERVED",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_AES_TOP_0",                    E_FORBIDDEN,     E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_AES_TOP_1",                    E_FORBIDDEN,     E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_MDEM_TEMP_SHARE",              E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_SYSTEM_Timer",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRA_AO_CLDMA_AO_TOP_AP",              E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_CLDMA_AO_TOP_MD",              E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_DVFS_CTRL_PROC",               E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_CRYPTO_WRAPPER",               E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSHUB",                        E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),

	/* 20 */
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_0",                       E_FORBIDDEN,     E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_1",                       E_SEC_RW_NS_R,   E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_2",                       E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_3",                       E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_4",                       E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_5",                       E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_6",                       E_FORBIDDEN,     E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_7",                       E_FORBIDDEN,     E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SSPM_8",                       E_FORBIDDEN,     E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SLEEP_CONTROL_PROCESSOR_0",    E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),

	/* 30 */
	DAPC_INFRA_ATTR("INFRA_AO_SLEEP_CONTROL_PROCESSOR_1",    E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_AO_SLEEP_CONTROL_PROCESSOR_2",    E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_TOP_MODULE",                   E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_SECURITY_GROUP",               E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_EMI_GROUP",                    E_NO_PROTECTION, E_FORBIDDEN,     E_SEC_RW_NS_R),
	DAPC_INFRA_ATTR("INFRASYS_CCIF0_1_CLDMA_AP",             E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_CCIF2_3_AP",                   E_NO_PROTECTION, E_FORBIDDEN,     E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_CFG_GROUP",                    E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_SYS_CIRQ",                     E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),

	/* 40 */
	DAPC_INFRA_ATTR("INFRASYS_GCE_GROUP",                    E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_SMI_GROUP",                    E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_PTP_Thermal_GROUP",            E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_MM_IOMMU_GROUP",               E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_GPIO_GROUP",                   E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_BUS_TRACE_GROUP",              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_EMI_MPU_REG",                  E_SEC_RW_ONLY,   E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_CCIF4_5_AP",                   E_NO_PROTECTION, E_FORBIDDEN,     E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_AUXADC",                       E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),

	/* 50 */
	DAPC_INFRA_ATTR("INFRASYS_BSI_BPI_GROUP",                E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_DRAMC_0_GROUP",                E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("CSI",                                   E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("EAST_IO_R",                             E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("EAST_IO_RT",                            E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("SSUSB_PHY",                             E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("EFUSE",                                 E_SEC_RW_NS_R,   E_SEC_RW_NS_R,   E_FORBIDDEN),
	DAPC_INFRA_ATTR("SOUTH_IO_B",                            E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("SOUTH_IO_BR",                           E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),

	/* 60 */
	DAPC_INFRA_ATTR("MIPI_DSI0",                             E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("MIPI_DSI1",                             E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("WEST_IO_L",                             E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("Reserved",                              E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("UFS MPHY",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("MSDC_PAD_MACRO",                        E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("NORTH_IO_T",                            E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_CCIF0_MD1",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_CCIF1_C2K",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),

	/* 70 */
	DAPC_INFRA_ATTR("INFRASYS_CCIF2_MD1",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_CCIF3_C2K",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_CLDMA_MD",                     E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_ MD1_C2K_CCIF",                E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_C2K_MD1_CCIF",                 E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_CCIF4_MD1",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("INFRASYS_CCIF5_C2K",                    E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),
	DAPC_INFRA_ATTR("RESERVED",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRA_PDN_MCSIB",                       E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_INFRA_ATTR("INFRASYS_AUDIO",                        E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),

	/* 80 */
	DAPC_INFRA_ATTR("CONNSYS",                               E_NO_PROTECTION, E_NO_PROTECTION, E_NO_PROTECTION),

};

static const struct INFRA_PERI_DEVICE_INFO D_APC_PERI_Devices[] = {
	/*              module,                                 AP permission,   MD permission,   SCP permission */

	/* 0 */
	DAPC_PERI_ATTR("APDMA",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Pericfg_reg",                           E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("UART0",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_NO_PROTECTION),
	DAPC_PERI_ATTR("UART1",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_NO_PROTECTION),
	DAPC_PERI_ATTR("UART2",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_NO_PROTECTION),
	DAPC_PERI_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("PWM",                                   E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C0",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C1",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),

	/* 10 */
	DAPC_PERI_ATTR("I2C2 (i3c)",                            E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI6",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C4",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C5",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C6 ( I2C treble)",                    E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI0",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C3",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C8",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI7",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),

	/* 20 */
	DAPC_PERI_ATTR("DISP_PWM0",                             E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI1",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI2",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C7",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Peri_devapc",                           E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI3",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI4",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("SPI5",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),

	/* 30 */
	DAPC_PERI_ATTR("I2C10",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C11",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("CQDMA",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Peri_MBIST",                            E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Peri_ao_MBIST",                         E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("I2C9",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("dx_cc private",                         E_SEC_RW_ONLY,   E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("TRNG",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("BTIF",                                  E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("peri_dfp_top",                          E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),

	/* 40 */
	DAPC_PERI_ATTR("USB2.0",                                E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_PERI_ATTR("MSDC0",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("MSDC1",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("USB2.0 Sub security region",            E_FORBIDDEN,     E_NO_PROTECTION, E_FORBIDDEN),
	DAPC_PERI_ATTR("MSDC3",                                 E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("UFS_DEV",                               E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("Reserved",                              E_NO_PROTECTION, E_FORBIDDEN,     E_FORBIDDEN),
	DAPC_PERI_ATTR("DEBUG_TOP",                             E_NO_PROTECTION, E_NO_PROTECTION, E_FORBIDDEN),

};

#endif

static const struct SSPM_DEVICE_INFO D_APC_SSPM_Devices[] = {
	/*      module,                          CONN2AP permission,     SPM permission          */

	/* 0 */
	DAPC_SSPM_ATTR("INFRA_AO_SSPM_0",          E_FORBIDDEN,          E_FORBIDDEN),
	DAPC_SSPM_ATTR("INFRA_AO_SSPM_1",          E_FORBIDDEN,          E_SEC_RW_ONLY),
	DAPC_SSPM_ATTR("INFRA_AO_SSPM_2",          E_FORBIDDEN,          E_FORBIDDEN),
	DAPC_SSPM_ATTR("INFRA_AO_SSPM_3",          E_FORBIDDEN,          E_FORBIDDEN),
	DAPC_SSPM_ATTR("INFRA_AO_SSPM_4",          E_FORBIDDEN,          E_FORBIDDEN),
	DAPC_SSPM_ATTR("INFRA_AO_SSPM_5",          E_FORBIDDEN,          E_FORBIDDEN),
	DAPC_SSPM_ATTR("INFRA_AO_SSPM_6",          E_FORBIDDEN,          E_FORBIDDEN),
	DAPC_SSPM_ATTR("INFRA_AO_SSPM_7",          E_FORBIDDEN,          E_FORBIDDEN),
	DAPC_SSPM_ATTR("INFRA_AO_SSPM_8",          E_FORBIDDEN,          E_FORBIDDEN),
};

/* it could be ignored */
static const struct MM_MD_DEVICE_INFO D_APC_MM_Devices[] = {
	/*            module,                       AP permission            */

	/* 0 */
	DAPC_MM_ATTR("MFG_G3D_CONFIG",              E_NO_PROTECTION),
	DAPC_MM_ATTR("RESERVED",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("MFG_DFP",                     E_NO_PROTECTION),
	DAPC_MM_ATTR("RESERVED",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("MFG_MALI",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("MFG_DFD",                     E_NO_PROTECTION),
	DAPC_MM_ATTR("VPUSYS_CFG",                  E_NO_PROTECTION),
	DAPC_MM_ATTR("ADL_CTRL",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("VCORE_CONFIG",                E_NO_PROTECTION),
	DAPC_MM_ATTR("RESERVED",                    E_NO_PROTECTION),

	/* 10 */
	DAPC_MM_ATTR("VPU CoreA DMEM",              E_NO_PROTECTION),
	DAPC_MM_ATTR("VPU CoreA DMEM",              E_NO_PROTECTION),
	DAPC_MM_ATTR("VPU CoreA IMEM",              E_NO_PROTECTION),
	DAPC_MM_ATTR("VPU CoreA control",           E_NO_PROTECTION),
	DAPC_MM_ATTR("VPU CoreA debug",             E_NO_PROTECTION),
	DAPC_MM_ATTR("VPU CoreB DMEM",              E_NO_PROTECTION),
	DAPC_MM_ATTR("VPU CoreB DMEM",              E_NO_PROTECTION),
	DAPC_MM_ATTR("VPU CoreB IMEM",              E_NO_PROTECTION),
	DAPC_MM_ATTR("VPU CoreB control",           E_NO_PROTECTION),
	DAPC_MM_ATTR("VPU CoreB debug",             E_NO_PROTECTION),

	/* 20 */
	DAPC_MM_ATTR("VPU CoreC DMEM",              E_NO_PROTECTION),
	DAPC_MM_ATTR("VPU CoreC DMEM",              E_NO_PROTECTION),
	DAPC_MM_ATTR("VPU CoreC IMEM",              E_NO_PROTECTION),
	DAPC_MM_ATTR("VPU CoreC control",           E_NO_PROTECTION),
	DAPC_MM_ATTR("VPU CoreC debug",             E_NO_PROTECTION),
	DAPC_MM_ATTR("MMSYS_CONFIG",                E_NO_PROTECTION),
	DAPC_MM_ATTR("MDP_RDMA0",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("MDP_RDMA1",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("MDP_RSZ0",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("MDP_RSZ1",                    E_NO_PROTECTION),

	/* 30 */
	DAPC_MM_ATTR("MDP_WROT0",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("MDP_WROT1",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("MDP_TDSHP",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("DISP_OVL0",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("DISP_OVL0_2L",                E_NO_PROTECTION),
	DAPC_MM_ATTR("DISP_OVL1_2L",                E_NO_PROTECTION),
	DAPC_MM_ATTR("DISP_RDMA0",                  E_NO_PROTECTION),
	DAPC_MM_ATTR("DISP_RDMA1",                  E_NO_PROTECTION),
	DAPC_MM_ATTR("DISP_WDMA0",                  E_NO_PROTECTION),
	DAPC_MM_ATTR("DISP_COLOR0",                 E_NO_PROTECTION),

	/* 40 */
	DAPC_MM_ATTR("DISP_CCORR0",                 E_NO_PROTECTION),
	DAPC_MM_ATTR("DISP_AAL0",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("DISP_GAMMA0",                 E_NO_PROTECTION),
	DAPC_MM_ATTR("DISP_DITHER0",                E_NO_PROTECTION),
	DAPC_MM_ATTR("DISP_SPLIT",                  E_NO_PROTECTION),
	DAPC_MM_ATTR("DSI0",                        E_NO_PROTECTION),
	DAPC_MM_ATTR("DSI1",                        E_NO_PROTECTION),
	DAPC_MM_ATTR("MM_MUTEX",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("SMI_LARB0",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("SMI_LARB1",                   E_NO_PROTECTION),

	/* 50 */
	DAPC_MM_ATTR("SMI_COMMON",                  E_NO_PROTECTION),
	DAPC_MM_ATTR("DISP_RSZ",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("MDP_CCORR",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("MDP_AAL",                     E_NO_PROTECTION),
	DAPC_MM_ATTR("SMI_SUB_COMMON",              E_NO_PROTECTION),
	DAPC_MM_ATTR("imgsys_top",                  E_NO_PROTECTION),
	DAPC_MM_ATTR("smi_larb5",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("dip_a0",                      E_NO_PROTECTION),
	DAPC_MM_ATTR("dip_a1",                      E_NO_PROTECTION),
	DAPC_MM_ATTR("dip_a2",                      E_NO_PROTECTION),

	/* 60 */
	DAPC_MM_ATTR("dip_a3",                      E_NO_PROTECTION),
	DAPC_MM_ATTR("dip_a4",                      E_NO_PROTECTION),
	DAPC_MM_ATTR("dip_a5",                      E_NO_PROTECTION),
	DAPC_MM_ATTR("dpe",                         E_NO_PROTECTION),
	DAPC_MM_ATTR("rsc",                         E_NO_PROTECTION),
	DAPC_MM_ATTR("wpe_a",                       E_NO_PROTECTION),
	DAPC_MM_ATTR("wpe_b",                       E_NO_PROTECTION),
	DAPC_MM_ATTR("fdvt",                        E_NO_PROTECTION),
	DAPC_MM_ATTR("owe",                         E_NO_PROTECTION),
	DAPC_MM_ATTR("mfb",                         E_NO_PROTECTION),

	/* 70 */
	DAPC_MM_ATTR("smi_larb2",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("vdec_top_global_con",         E_NO_PROTECTION),
	DAPC_MM_ATTR("vdec_top_smi_larb4",          E_NO_PROTECTION),
	DAPC_MM_ATTR("vdec_top_full_top",           E_NO_PROTECTION),
	DAPC_MM_ATTR("RESERVED",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("RESERVED",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("venc_global_con",             E_NO_PROTECTION),
	DAPC_MM_ATTR("smi_larb3",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("venc",                        E_NO_PROTECTION),
	DAPC_MM_ATTR("jpgenc",                      E_NO_PROTECTION),

	/* 80 */
	DAPC_MM_ATTR("RESERVED",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("RESERVED",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("RESERVED",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("camsys top",                  E_NO_PROTECTION),
	DAPC_MM_ATTR("smi_larb6",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("smi_larb3",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_top",                     E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_a",                       E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_a",                       E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_b",                       E_NO_PROTECTION),

	/* 90 */
	DAPC_MM_ATTR("cam_b",                       E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_c",                       E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_c",                       E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_top_set",                 E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_a_set",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_a_set",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_b_set",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_b_set",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_c_set",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_c_set",                   E_NO_PROTECTION),

	/* 100 */
	DAPC_MM_ATTR("cam_a_ext",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_b_ext",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_c_ext",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_top_inner",               E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_a_inner",                 E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_a_inner",                 E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_b_inner",                 E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_b_inner",                 E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_c_inner",                 E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_c_inner",                 E_NO_PROTECTION),

	/* 110 */
	DAPC_MM_ATTR("cam_a_ext",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_b_ext",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_c_ext",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_top_clr",                 E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_a_clr",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_a_clr",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_b_clr",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_b_clr",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_c_clr",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_c_clr",                   E_NO_PROTECTION),

	/* 120 */
	DAPC_MM_ATTR("cam_a_ext",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_b_ext",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("cam_c_ext",                   E_NO_PROTECTION),
	DAPC_MM_ATTR("Reserved",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("seninf_a",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("seninf_b",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("seninf_c",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("seninf_d",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("seninf_e",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("seninf_f",                    E_NO_PROTECTION),

	/* 130 */
	DAPC_MM_ATTR("seninf_g",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("seninf_h",                    E_NO_PROTECTION),
	DAPC_MM_ATTR("camsv_a",                     E_NO_PROTECTION),
	DAPC_MM_ATTR("camsv_b",                     E_NO_PROTECTION),
	DAPC_MM_ATTR("camsv_c",                     E_NO_PROTECTION),
	DAPC_MM_ATTR("camsv_d",                     E_NO_PROTECTION),
	DAPC_MM_ATTR("MD32 DMEM (0~12KB)",          E_NO_PROTECTION),
	DAPC_MM_ATTR("MD32 PMEM (24KB)",            E_NO_PROTECTION),
	DAPC_MM_ATTR("MD32(ip",                     E_NO_PROTECTION),
	DAPC_MM_ATTR("Reserved",                    E_NO_PROTECTION),

};

static const struct MM_MD_DEVICE_INFO D_APC_MD_Devices[] = {
	/*            module,                       AP permission            */

	/* 0 */
	DAPC_MD_ATTR("MDPERISYS_1",                 E_NO_PROTECTION),
	DAPC_MD_ATTR("MDPERISYS_2",                 E_NO_PROTECTION),
	DAPC_MD_ATTR("MDMCUAPB_MACRO",              E_NO_PROTECTION),
	DAPC_MD_ATTR("MDMCUAPB_NONMACRO",           E_NO_PROTECTION),
	DAPC_MD_ATTR("MDINFRA_APB_1",               E_NO_PROTECTION),
	DAPC_MD_ATTR("MDINFRA_APB_2",               E_NO_PROTECTION),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),
	DAPC_MD_ATTR("LTEL2SYS",                    E_FORBIDDEN),
	DAPC_MD_ATTR("HSPAL2",                      E_FORBIDDEN),

	/* 10 */
	DAPC_MD_ATTR("PSPERI",                      E_FORBIDDEN),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),

	/* 20 */
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),
	DAPC_MD_ATTR("DFESYS_1",                    E_FORBIDDEN),
	DAPC_MD_ATTR("LTXBSYS_1",                   E_FORBIDDEN),
	DAPC_MD_ATTR("MODEM_AO_APB",                E_FORBIDDEN),
	DAPC_MD_ATTR("L1MCUSYS_APB",                E_FORBIDDEN),
	DAPC_MD_ATTR("DFESYS_0",                    E_FORBIDDEN),
	DAPC_MD_ATTR("CSTXBSYS",                    E_FORBIDDEN),

	/* 30 */
	DAPC_MD_ATTR("LTXBSYS",                     E_FORBIDDEN),
	DAPC_MD_ATTR("RXBRP",                       E_FORBIDDEN),
	DAPC_MD_ATTR("RAKE",                        E_FORBIDDEN),
	DAPC_MD_ATTR("EQ1",                         E_FORBIDDEN),
	DAPC_MD_ATTR("EQ2",                         E_FORBIDDEN),
	DAPC_MD_ATTR("EQ3",                         E_FORBIDDEN),
	DAPC_MD_ATTR("TD1",                         E_FORBIDDEN),
	DAPC_MD_ATTR("TD2",                         E_FORBIDDEN),
	DAPC_MD_ATTR("CSSYS",                       E_FORBIDDEN),
	DAPC_MD_ATTR("MPCSYS",                      E_FORBIDDEN),

	/* 40 */
	DAPC_MD_ATTR("IMCSYS",                      E_FORBIDDEN),
	DAPC_MD_ATTR("MMUSYS",                      E_FORBIDDEN),
	DAPC_MD_ATTR("ICCSYS",                      E_FORBIDDEN),
	DAPC_MD_ATTR("Reserved",                    E_FORBIDDEN),
	DAPC_MD_ATTR("Reserved",                    E_FORBIDDEN),
	DAPC_MD_ATTR("Matrix Config",               E_FORBIDDEN),
	DAPC_MD_ATTR("IRAM",                        E_FORBIDDEN),
	DAPC_MD_ATTR("CP peripheral",               E_NO_PROTECTION),
	DAPC_MD_ATTR("Modem peripheral",            E_FORBIDDEN),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),

	/* 50 */
	DAPC_MD_ATTR("Speech Subsystem",            E_FORBIDDEN),
	DAPC_MD_ATTR("-",                           E_FORBIDDEN),
	DAPC_MD_ATTR("BOOTROM",                     E_FORBIDDEN),

};


static void clear_vio_status(enum DAPC_PD_SLAVE_TYPE dapc_pd_slave_type, unsigned int module)
{
	unsigned int apc_index = 0;
	unsigned int apc_bit_index = 0;

	apc_index = module / (MOD_NO_IN_1_DEVAPC * 2);
	apc_bit_index = module % (MOD_NO_IN_1_DEVAPC * 2);

	if (dapc_pd_slave_type == E_DAPC_PD_INFRA_MM_MD_SLAVE && module <= INFRA_MM_MD_MAX_VIO_INDEX) {
		*(unsigned int *)((size_t)DEVAPC_PD_INFRA_VIO_STA_0 + apc_index * 4) = (0x1 << apc_bit_index);
	} else if (dapc_pd_slave_type == E_DAPC_PD_PERI_SLAVE && module <= PERI_MAX_VIO_INDEX) {
		*(unsigned int *)((size_t)DEVAPC_PD_PERI_VIO_STA_0 + apc_index * 4) = (0x1 << apc_bit_index);
	}
}

static void set_master_transaction(enum DAPC_MASTER_TYPE master_type, unsigned int master_index,
	enum E_TRANSACTION transaction_type)
{
	unsigned int *base = NULL;
	unsigned int master_register_index = 0;
	unsigned int master_set_index = 0;

	master_register_index = master_index / (MOD_NO_IN_1_DEVAPC * 2);
	master_set_index = master_index % (MOD_NO_IN_1_DEVAPC * 2);

	if (master_type == E_DAPC_INFRA_MASTER && master_index <= MASTER_INFRA_MAX_INDEX) {
		base = (unsigned int *)((size_t)DEVAPC_INFRA_MAS_SEC_0 + master_register_index * 4);
	} else if (master_type == E_DAPC_PERI_MASTER && master_index <= MASTER_PERI_MAX_INDEX) {
		base = (unsigned int *)((size_t)DEVAPC_PERI_MAS_SEC_0 + master_register_index * 4);
	} else {
		return;
	}

	if (base != NULL) {
		if (transaction_type == NON_SECURE_TRANSACTION) {
			devapc_writel(devapc_readl(base) & (0xFFFFFFFF ^ (0x1 << master_set_index)), base);
		} else if (transaction_type == SECURE_TRANSACTION) {
			devapc_writel(devapc_readl(base) | (0x1 << master_set_index), base);
		}
	}
}

static void set_module_apc(enum DAPC_SLAVE_TYPE slave_type, unsigned int module,
	enum E_MASK_DOM domain_num, enum APC_ATTR permission_control)
{
	unsigned int *base = NULL;
	unsigned int apc_index = 0;
	unsigned int apc_set_index = 0;
	unsigned int clr_bit = 0;
	unsigned int set_bit = 0;

	if (permission_control != E_NO_PROTECTION && permission_control != E_SEC_RW_ONLY
		&& permission_control != E_SEC_RW_NS_R && permission_control != E_FORBIDDEN) {
		return;
	}

	apc_index = module / MOD_NO_IN_1_DEVAPC;
	apc_set_index = module % MOD_NO_IN_1_DEVAPC;
	clr_bit = 0xFFFFFFFF ^ (0x3 << (apc_set_index * 2));
	set_bit = permission_control << (apc_set_index * 2);

	/* Note: "clear_vio_status" is moved outside the function because
	 * config index and violation index may not be the same
	 */

	/* Note: "unmask_module_irq" is changed to set in Kernel to avoid time gap between DEVAPC and EMI_MPU.
	 * Remember: NEVER unmask_module_irq before the Device APC Kernel driver is loaded. Otherwise, nobody can
	 *           handle the violations before Device APC kernel driver is loaded.
	 */

	/* Do boundary check */

	if (slave_type == E_DAPC_INFRA_SLAVE && module <= SLAVE_INFRA_MAX_INDEX && domain_num <= E_DOMAIN_7) {
		base = (unsigned int *)((size_t)DEVAPC_INFRA_D0_APC_0 + domain_num * 0x100 + apc_index * 4);
	} else if (slave_type == E_DAPC_MM_SLAVE && module <= SLAVE_MM_MAX_INDEX && domain_num <= E_DOMAIN_3) {
		base = (unsigned int *)((size_t)DEVAPC_MM_D0_APC_0 + domain_num * 0x100 + apc_index * 4);
	} else if (slave_type == E_DAPC_MD_SLAVE && module <= SLAVE_MD_MAX_INDEX && domain_num <= E_DOMAIN_3) {
		base = (unsigned int *)((size_t)DEVAPC_MD_D0_APC_0 + domain_num * 0x100 + apc_index * 4);
	} else if (slave_type == E_DAPC_PERI_SLAVE && module <= SLAVE_PERI_MAX_INDEX && domain_num <= E_DOMAIN_7) {
		base = (unsigned int *)((size_t)DEVAPC_PERI_D0_APC_0 + domain_num * 0x100 + apc_index * 4);
	} else {
		return;
	}

	if (base != NULL) {
		devapc_writel(devapc_readl(base) & clr_bit, base);
		devapc_writel(devapc_readl(base) | set_bit, base);
	}
}

int start_devapc(void)
{
	unsigned int module_index = 0;

	int i = 0; /* register number */

#if !defined(SPD_no)
	unsigned int infra_tee_size = sizeof(D_APC_INFRA_Devices_TEE)/sizeof(struct INFRA_PERI_DEVICE_INFO);
	unsigned int peri_tee_size = sizeof(D_APC_PERI_Devices_TEE)/sizeof(struct INFRA_PERI_DEVICE_INFO);
#else
	unsigned int infra_size = sizeof(D_APC_INFRA_Devices)/sizeof(struct INFRA_PERI_DEVICE_INFO);
	unsigned int peri_size = sizeof(D_APC_PERI_Devices)/sizeof(struct INFRA_PERI_DEVICE_INFO);
#endif
	unsigned int sspm_size = sizeof(D_APC_SSPM_Devices)/sizeof(struct SSPM_DEVICE_INFO);
	unsigned int mm_size = sizeof(D_APC_MM_Devices)/sizeof(struct MM_MD_DEVICE_INFO);
	unsigned int md_size = sizeof(D_APC_MD_Devices)/sizeof(struct MM_MD_DEVICE_INFO);

	/* Enable Devapc */

	devapc_writel(0x80000000, DEVAPC_PD_INFRA_VIO_DBG0); /* clear apc0 dbg info if any */
	devapc_writel(0x80000000, DEVAPC_PD_PERI_VIO_DBG0); /* clear apc0 dbg info if any */

	devapc_writel(0x80000001, DEVAPC_INFRA_APC_CON);
	devapc_writel(0x80000001, DEVAPC_MM_APC_CON);
	devapc_writel(0x80000001, DEVAPC_MD_APC_CON);
	devapc_writel(0x80000001, DEVAPC_PERI_APC_CON);

	devapc_writel(0x80000000, DEVAPC_PD_INFRA_APC_CON);
	devapc_writel(0x80000000, DEVAPC_PD_PERI_APC_CON);


	/* Set Necessary Masters to Secure Transaction */
	/* SPM_PDN will access MCUSYS in Normal World, but MCUSYS is secure r/w and
	 * non-secure read-only. So this setting is necessary.
	 */
	set_master_transaction(E_DAPC_INFRA_MASTER, MASTER_INFRA_SSPM, SECURE_TRANSACTION);

	/* for big cpu power setting */
	set_master_transaction(E_DAPC_INFRA_MASTER, MASTER_INFRA_SPM, SECURE_TRANSACTION);


	/* Clear violation status */
	for (module_index = 0; module_index <= INFRA_MM_MD_MAX_VIO_INDEX; module_index++)
		clear_vio_status(E_DAPC_PD_INFRA_MM_MD_SLAVE, module_index);
	for (module_index = 0; module_index <= PERI_MAX_VIO_INDEX; module_index++)
		clear_vio_status(E_DAPC_PD_PERI_SLAVE, module_index);


	/* Initial Permission */

#if !defined(SPD_no)
	INFO("[DAPC] Walk TEE\n");
	for (module_index = 0; module_index < infra_tee_size; module_index++) {
		set_module_apc(E_DAPC_INFRA_SLAVE, module_index, E_DOMAIN_0,
				D_APC_INFRA_Devices_TEE[module_index].d0_permission);
		set_module_apc(E_DAPC_INFRA_SLAVE, module_index, E_DOMAIN_1,
				D_APC_INFRA_Devices_TEE[module_index].d1_permission);
		set_module_apc(E_DAPC_INFRA_SLAVE, module_index, E_DOMAIN_4,
				D_APC_INFRA_Devices_TEE[module_index].d4_permission);
		set_module_apc(E_DAPC_INFRA_SLAVE, module_index, E_DOMAIN_7, E_FORBIDDEN);
	}

	for (module_index = 0; module_index < peri_tee_size; module_index++) {
		set_module_apc(E_DAPC_PERI_SLAVE, module_index, E_DOMAIN_0,
			D_APC_PERI_Devices_TEE[module_index].d0_permission);
		set_module_apc(E_DAPC_PERI_SLAVE, module_index, E_DOMAIN_1,
			D_APC_PERI_Devices_TEE[module_index].d1_permission);
		set_module_apc(E_DAPC_PERI_SLAVE, module_index, E_DOMAIN_4,
			D_APC_PERI_Devices_TEE[module_index].d4_permission);
		set_module_apc(E_DAPC_PERI_SLAVE, module_index, E_DOMAIN_7,
			E_FORBIDDEN);
	}
#else
	INFO("[DAPC] Walk Non-TEE (ATF only)\n");
	for (module_index = 0; module_index < infra_size; module_index++) {
		set_module_apc(E_DAPC_INFRA_SLAVE, module_index, E_DOMAIN_0,
			D_APC_INFRA_Devices[module_index].d0_permission);
		set_module_apc(E_DAPC_INFRA_SLAVE, module_index, E_DOMAIN_1,
			D_APC_INFRA_Devices[module_index].d1_permission);
		set_module_apc(E_DAPC_INFRA_SLAVE, module_index, E_DOMAIN_4,
			D_APC_INFRA_Devices[module_index].d4_permission);
		set_module_apc(E_DAPC_INFRA_SLAVE, module_index, E_DOMAIN_7,
			E_FORBIDDEN);
	}

	for (module_index = 0; module_index < peri_size; module_index++) {
		set_module_apc(E_DAPC_PERI_SLAVE, module_index, E_DOMAIN_0,
			D_APC_PERI_Devices[module_index].d0_permission);
		set_module_apc(E_DAPC_PERI_SLAVE, module_index, E_DOMAIN_1,
			D_APC_PERI_Devices[module_index].d1_permission);
		set_module_apc(E_DAPC_PERI_SLAVE, module_index, E_DOMAIN_4,
			D_APC_PERI_Devices[module_index].d4_permission);
		set_module_apc(E_DAPC_PERI_SLAVE, module_index, E_DOMAIN_7,
			E_FORBIDDEN);
	}
#endif

	for (module_index = 0; module_index < sspm_size; module_index++) {
		set_module_apc(E_DAPC_INFRA_SLAVE, SLAVE_INFRA_SSPM_0 + module_index,
			E_DOMAIN_3, D_APC_SSPM_Devices[module_index].d3_permission);
		set_module_apc(E_DAPC_INFRA_SLAVE, SLAVE_INFRA_SSPM_0 + module_index,
			E_DOMAIN_5, D_APC_SSPM_Devices[module_index].d5_permission);
	}

	/* [Important] You must open the access permission of AP=>MD1_PERIPHERALS and AP=>C2K_PERIPHERALS because
	 *             it is the first level protection. Below is the MD 2nd level protection.
	 */
	for (module_index = 0; module_index < mm_size; module_index++) {
		set_module_apc(E_DAPC_MM_SLAVE, module_index, E_DOMAIN_0,
			D_APC_MM_Devices[module_index].d0_permission);
		set_module_apc(E_DAPC_MM_SLAVE, module_index, E_DOMAIN_3,
			E_FORBIDDEN);  /* Set others to forbidden */
	}

	for (module_index = 0; module_index < md_size; module_index++) {
		set_module_apc(E_DAPC_MD_SLAVE, module_index, E_DOMAIN_0,
			D_APC_MD_Devices[module_index].d0_permission);
		set_module_apc(E_DAPC_MD_SLAVE, module_index, E_DOMAIN_3,
			E_FORBIDDEN);  /* Set others to forbidden */
	}


	/* Print logs for debugging */
	/* d: domain, i: register number */
	for (i = 0; i < 6; i++) {
		INFO("[DEVAPC] (INFRA)D0_APC_%d = 0x%x, (INFRA)D1_APC_%d = 0x%x\n",
			i, *(unsigned int *)((size_t)DEVAPC_INFRA_D0_APC_0 + i * 4),
			i, *(unsigned int *)((size_t)DEVAPC_INFRA_D0_APC_0 + 0x100 + i * 4));

		INFO("[DEVAPC] ((INFRA)D3_APC_%d = 0x%x, (INFRA)D4_APC_%d = 0x%x\n",
			i, *(unsigned int *)((size_t)DEVAPC_INFRA_D0_APC_0 + 0x300 + i * 4),
			i, *(unsigned int *)((size_t)DEVAPC_INFRA_D0_APC_0 + 0x400 + i * 4));

		INFO("[DEVAPC] ((INFRA)D5_APC_%d = 0x%x, (INFRA)D7_APC_%d = 0x%x\n",
			i, *(unsigned int *)((size_t)DEVAPC_INFRA_D0_APC_0 + 0x500 + i * 4),
			i, *(unsigned int *)((size_t)DEVAPC_INFRA_D0_APC_0 + 0x700 + i * 4));

	}

	for (i = 0; i < 3; i++) {
		INFO("[DEVAPC] (PERI)D0_APC_%d = 0x%x, (PERI)D1_APC_%d = 0x%x\n",
			i, *(unsigned int *)((size_t)DEVAPC_PERI_D0_APC_0 + i * 4),
			i, *(unsigned int *)((size_t)DEVAPC_PERI_D0_APC_0 + 0x100 + i * 4));

		INFO("[DEVAPC] (PERI)D3_APC_%d = 0x%x, (PERI)D4_APC_%d = 0x%x\n",
			i, *(unsigned int *)((size_t)DEVAPC_PERI_D0_APC_0 + 0x300 + i * 4),
			i, *(unsigned int *)((size_t)DEVAPC_PERI_D0_APC_0 + 0x400 + i * 4));

		INFO("[DEVAPC] (PERI)D5_APC_%d = 0x%x, (PERI)D7_APC_%d = 0x%x\n",
			i, *(unsigned int *)((size_t)DEVAPC_PERI_D0_APC_0 + 0x500 + i * 4),
			i, *(unsigned int *)((size_t)DEVAPC_PERI_D0_APC_0 + 0x700 + i * 4));
	}

	for (i = 0; i < 8; i++) {
		INFO("[DEVAPC] (MM)D0_APC_%d = 0x%x, (MM)D3_APC_%d = 0x%x\n",
			i, *(unsigned int *)((size_t)DEVAPC_MM_D0_APC_0 + i * 4),
			i, *(unsigned int *)((size_t)DEVAPC_MM_D0_APC_0 + 0x300 + i * 4));
	}

	for (i = 0; i < 4; i++) {
		INFO("[DEVAPC] (MD)D0_APC_%d = 0x%x, (MD)D3_APC_%d = 0x%x\n",
			i, *(unsigned int *)((size_t)DEVAPC_MD_D0_APC_0 + i * 4),
			i, *(unsigned int *)((size_t)DEVAPC_MD_D0_APC_0 + 0x300 + i * 4));
	}

	INFO("[DEVAPC] (INFRA)MAS_SEC_0 = 0x%x, (PERI)MAS_SEC_0 = 0x%x\n",
		*DEVAPC_INFRA_MAS_SEC_0, *DEVAPC_PERI_MAS_SEC_0);

	for (i = 0; i < 7; i++)
		INFO("[DEVAPC] (INFRA)VIO_STA_%d=0x%x\n", i,
			*(unsigned int *)((size_t)DEVAPC_PD_INFRA_VIO_STA_0 + i * 4));

	for (i = 0; i < 2; i++)
		INFO("[DEVAPC] (PERI)VIO_STA_%d=0x%x\n", i,
			*((unsigned int *)((size_t)DEVAPC_PD_PERI_VIO_STA_0 + (i * 4))));

	INFO("\n[DEVAPC] MM_SEC_EN_0: 0x%x, MM_SEC_EN_1: 0x%x, MM_SEC_EN_2: 0x%x\n",
		*DEVAPC_MM_SEC_EN_0, *DEVAPC_MM_SEC_EN_1, *DEVAPC_MM_SEC_EN_2);



	/* Lock DAPC to secure access only */

	/* devapc_writel(devapc_readl(DEVAPC0_APC_CON) | (0x1), DEVAPC0_APC_CON); */
	/* devapc_writel(devapc_readl(DEVAPC0_MM_APC_CON) | (0x1), DEVAPC0_MM_APC_CON); */
	/* devapc_writel(devapc_readl(DEVAPC0_MD_APC_CON) | (0x1), DEVAPC0_MD_APC_CON); */
	/* devapc_writel(devapc_readl(DEVAPC0_PERI_APC_CON) | (0x1), DEVAPC0_PERI_APC_CON); */


	/* Set Level 2 Secure CG (INFRACFG_AO) */
	devapc_writel(devapc_readl(INFRA_AO_SEC_CG_CON0) | SEJ_CG_PROTECT_BIT, INFRA_AO_SEC_CG_CON0);
	devapc_writel(devapc_readl(INFRA_AO_SEC_CG_CON0) | DEVMPU_CG_PROTECT_BIT, INFRA_AO_SEC_CG_CON0);
	devapc_writel(devapc_readl(INFRA_AO_SEC_CG_CON1) | TRNG_CG_PROTECT_BIT, INFRA_AO_SEC_CG_CON1);
	devapc_writel(devapc_readl(INFRA_AO_SEC_CG_CON1) | DEVAPC_INFRA_PD_PROTECT_BIT, INFRA_AO_SEC_CG_CON1);

	/* Set Level 2 Secure CG (PERICFG) */
	devapc_writel(devapc_readl(PERICFG_SEC_CG_CON2) | (UFS_AES_CORE_PROTECT_BIT), PERICFG_SEC_CG_CON2);
	devapc_writel(devapc_readl(PERICFG_SEC_CG_CON4) | (DEVAPC_PERI_PD_PROTECT_BIT), PERICFG_SEC_CG_CON4);
	devapc_writel(devapc_readl(PERICFG_SEC_CG_CON4) | (DXCC_AO_CORE_PROTECT_BIT), PERICFG_SEC_CG_CON4);
	devapc_writel(devapc_readl(PERICFG_SEC_CG_CON4) | (DXCC_Secure_CORE_PROTECT_BIT), PERICFG_SEC_CG_CON4);


	INFO("[DEVAPC] INFRA_AO_SEC_CON = 0x%x\n", *INFRA_AO_SEC_CON);

	INFO("[DEVAPC] INFRA_AO_SEC_CG_CON 0: 0x%x, 1: 0x%x, 2: 0x%x, 3: 0x%x, 4: 0x%x\n",
		*INFRA_AO_SEC_CG_CON0, *INFRA_AO_SEC_CG_CON1, *INFRA_AO_SEC_CG_CON2,
		*INFRA_AO_SEC_CG_CON3, *INFRA_AO_SEC_CG_CON4);

	INFO("[DEVAPC] PERICFG_SEC_CG_CON 0: 0x%x, 1: 0x%x, 2: 0x%x, 3: 0x%x, 4: 0x%x, 5: 0x%x\n",
		*PERICFG_SEC_CG_CON0, *PERICFG_SEC_CG_CON1, *PERICFG_SEC_CG_CON2,
		*PERICFG_SEC_CG_CON3, *PERICFG_SEC_CG_CON4, *PERICFG_SEC_CG_CON5);

	INFO("[DEVAPC] Init Done\n");

	INFO("[DEVAPC] INFRA_APC_CON = 0x%x, MM_APC_CON = 0x%x, MD_APC_CON = 0x%x, PERI_APC_CON = 0x%x\n",
		*DEVAPC_INFRA_APC_CON, *DEVAPC_MM_APC_CON, *DEVAPC_MD_APC_CON, *DEVAPC_PERI_APC_CON);


	/* Set PMS(MD devapc) enable */
	devapc_writel(devapc_readl(AP2MD1_PMS_CTRL_EN) | 0x1, AP2MD1_PMS_CTRL_EN);
	devapc_writel(devapc_readl(AP2MD1_PMS_CTRL_EN_LOCK) | 0x1, AP2MD1_PMS_CTRL_EN_LOCK);

	INFO("[PMS(DEVAPC)] AP2MD1_PMS_CTRL_EN = 0x%x\n", *AP2MD1_PMS_CTRL_EN);
	INFO("[PMS(DEVAPC)] AP2MD1_PMS_CTRL_EN_LOCK = 0x%x\n", *AP2MD1_PMS_CTRL_EN_LOCK);

	return 0;
}

int dump_devapc(void)
{
	int i;

	INFO("%s: dump Dx_APC_x register:\n", __func__);

	/* Print logs for debugging */
	/* d: domain, i: register number */
	for (i = 0; i < 6; i++) {
		INFO("[DEVAPC] (INFRA)D0_APC_%d = 0x%x, (INFRA)D1_APC_%d = 0x%x\n",
			i, *(unsigned int *)((size_t)DEVAPC_INFRA_D0_APC_0 + i * 4),
			i, *(unsigned int *)((size_t)DEVAPC_INFRA_D0_APC_0 + 0x100 + i * 4));

		INFO("[DEVAPC] ((INFRA)D3_APC_%d = 0x%x, (INFRA)D4_APC_%d = 0x%x\n",
			i, *(unsigned int *)((size_t)DEVAPC_INFRA_D0_APC_0 + 0x300 + i * 4),
			i, *(unsigned int *)((size_t)DEVAPC_INFRA_D0_APC_0 + 0x400 + i * 4));

		INFO("[DEVAPC] ((INFRA)D5_APC_%d = 0x%x, (INFRA)D7_APC_%d = 0x%x\n",
			i, *(unsigned int *)((size_t)DEVAPC_INFRA_D0_APC_0 + 0x500 + i * 4),
			i, *(unsigned int *)((size_t)DEVAPC_INFRA_D0_APC_0 + 0x700 + i * 4));

	}

	for (i = 0; i < 3; i++) {
		INFO("[DEVAPC] (PERI)D0_APC_%d = 0x%x, (PERI)D1_APC_%d = 0x%x\n",
			i, *(unsigned int *)((size_t)DEVAPC_PERI_D0_APC_0 + i * 4),
			i, *(unsigned int *)((size_t)DEVAPC_PERI_D0_APC_0 + 0x100 + i * 4));

		INFO("[DEVAPC] (PERI)D3_APC_%d = 0x%x, (PERI)D4_APC_%d = 0x%x\n",
			i, *(unsigned int *)((size_t)DEVAPC_PERI_D0_APC_0 + 0x300 + i * 4),
			i, *(unsigned int *)((size_t)DEVAPC_PERI_D0_APC_0 + 0x400 + i * 4));

		INFO("[DEVAPC] (PERI)D5_APC_%d = 0x%x, (PERI)D7_APC_%d = 0x%x\n",
			i, *(unsigned int *)((size_t)DEVAPC_PERI_D0_APC_0 + 0x500 + i * 4),
			i, *(unsigned int *)((size_t)DEVAPC_PERI_D0_APC_0 + 0x700 + i * 4));
	}

	INFO("%s: dump MM_SEC_EN_x register:\n", __func__);
	INFO("[DEVAPC] MM_SEC_EN_0: 0x%x, MM_SEC_EN_1: 0x%x, MM_SEC_EN_2: 0x%x\n",
			*DEVAPC_MM_SEC_EN_0, *DEVAPC_MM_SEC_EN_1, *DEVAPC_MM_SEC_EN_2);

	INFO("%s: dump PMS(DEVAPC) register:\n", __func__);
	INFO("[PMS(DEVAPC)] AP2MD1_PMS_CTRL_EN = 0x%x\n", *AP2MD1_PMS_CTRL_EN);
	INFO("[PMS(DEVAPC)] AP2MD1_PMS_CTRL_EN_LOCK = 0x%x\n", *AP2MD1_PMS_CTRL_EN_LOCK);

	return 0;
}

