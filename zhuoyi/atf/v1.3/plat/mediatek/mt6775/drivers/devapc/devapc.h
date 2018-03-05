#ifndef __ATF_DAPC_H__
#define __ATF_DAPC_H__

#include <platform_def.h>

/******************************************************************************
 *
 * FUNCTION DEFINATION

 ******************************************************************************/
int start_devapc(void);
int dump_devapc(void);


/******************************************************************************
 *
 * STRUCTURE DEFINATION

 ******************************************************************************/
enum E_TRANSACTION {
	NON_SECURE_TRANSACTION = 0,
	SECURE_TRANSACTION,
	E_TRANSACTION_RESERVRD = 0x7FFFFFFF  /* force enum to use 32 bits */
};

enum APC_ATTR {
	E_NO_PROTECTION = 0,
	E_SEC_RW_ONLY,
	E_SEC_RW_NS_R,
	E_FORBIDDEN,
	E_MAX_APC_ATTR,
	E_APC_ATTR_RESERVRD = 0x7FFFFFFF  /* force enum to use 32 bits */
};

enum E_MASK_DOM {
	E_DOMAIN_0 = 0,
	E_DOMAIN_1 ,
	E_DOMAIN_2 ,
	E_DOMAIN_3 ,
	E_DOMAIN_4 ,
	E_DOMAIN_5 ,
	E_DOMAIN_6 ,
	E_DOMAIN_7 ,
	E_MAX_MASK_DOM,
	E_MASK_DOM_RESERVRD = 0x7FFFFFFF  /* force enum to use 32 bits */
};

enum DAPC_MASTER_TYPE {
	E_DAPC_INFRA_MASTER = 0,
	E_DAPC_PERI_MASTER,
	E_DAPC_MASTER_TYPE_RESERVRD = 0x7FFFFFFF  /* force enum to use 32 bits */
};

enum DAPC_SLAVE_TYPE {
	E_DAPC_INFRA_SLAVE = 0,
	E_DAPC_MM_SLAVE,
	E_DAPC_MD_SLAVE,
	E_DAPC_PERI_SLAVE,
	E_DAPC_SLAVE_TYPE_RESERVRD = 0x7FFFFFFF  /* force enum to use 32 bits */
};

enum DAPC_PD_SLAVE_TYPE {
	E_DAPC_PD_INFRA_MM_MD_SLAVE = 0,
	E_DAPC_PD_PERI_SLAVE,
	E_DAPC_PD_SLAVE_TYPE_RESERVRD = 0x7FFFFFFF  /* force enum to use 32 bits */
};

struct INFRA_PERI_DEVICE_INFO {
	unsigned char       d0_permission;
	unsigned char       d1_permission;
	unsigned char       d4_permission;
};

#define DAPC_INFRA_ATTR(DEV_NAME, PERM_ATTR1, PERM_ATTR2, PERM_ATTR3) \
{(unsigned char)PERM_ATTR1, (unsigned char)PERM_ATTR2, (unsigned char)PERM_ATTR3}
#define DAPC_PERI_ATTR(DEV_NAME, PERM_ATTR1, PERM_ATTR2, PERM_ATTR3) \
{(unsigned char)PERM_ATTR1, (unsigned char)PERM_ATTR2, (unsigned char)PERM_ATTR3}

struct SSPM_DEVICE_INFO {
	unsigned char       d3_permission;
	unsigned char       d5_permission;
};

#define DAPC_SSPM_ATTR(DEV_NAME, PERM_ATTR1, PERM_ATTR2) {(unsigned char)PERM_ATTR1, (unsigned char)PERM_ATTR2}

struct MM_MD_DEVICE_INFO {
	unsigned char       d0_permission;
};

#define DAPC_MM_ATTR(DEV_NAME, PERM_ATTR1) {(unsigned char)PERM_ATTR1}
#define DAPC_MD_ATTR(DEV_NAME, PERM_ATTR1) {(unsigned char)PERM_ATTR1}

/******************************************************************************
 *
 * UTILITY DEFINATION

 ******************************************************************************/

#define READ_REGISTER_UINT32(reg) \
	(*(unsigned int * const)(uintptr_t)(reg))

#define WRITE_REGISTER_UINT32(reg, val) \
	((*(unsigned int * const)(uintptr_t)(reg)) = (val))

#define devapc_writel(VAL,REG)  WRITE_REGISTER_UINT32(REG, VAL)
#define devapc_readl(REG)       READ_REGISTER_UINT32(REG)

/******************************************************************************
 *
 * REGISTER ADDRESS DEFINATION
 *
 ******************************************************************************/
#define DEVAPC_AO_INFRA_BASE        0x10070000
#define DEVAPC_AO_MM_BASE           0x10071000
#define DEVAPC_AO_MD_BASE           0x10072000
#define DEVAPC_AO_PERI_BASE         0x111A0000

#define DEVAPC_PD_INFRA_BASE        0x10222000
#define DEVAPC_PD_PERI_BASE         0x111A1000

/*******************************************************************************************/
/* Device APC AO for INFRA */
#define DEVAPC_INFRA_D0_APC_0         ((unsigned int *)(DEVAPC_AO_INFRA_BASE+0x0000))

#define DEVAPC_INFRA_MAS_SEC_0        ((unsigned int *)(DEVAPC_AO_INFRA_BASE+0x0B00))

#define DEVAPC_INFRA_APC_CON          ((unsigned int *)(DEVAPC_AO_INFRA_BASE+0x0F00))

/*******************************************************************************************/
/* Device APC AO for MM */
#define DEVAPC_MM_D0_APC_0            ((unsigned int *)(DEVAPC_AO_MM_BASE+0x0000))

#define DEVAPC_MM_SEC_EN_0            ((unsigned int *)(DEVAPC_AO_MM_BASE+0x0E00))
#define DEVAPC_MM_SEC_EN_1            ((unsigned int *)(DEVAPC_AO_MM_BASE+0x0E04))
#define DEVAPC_MM_SEC_EN_2            ((unsigned int *)(DEVAPC_AO_MM_BASE+0x0E08))

#define DEVAPC_MM_APC_CON             ((unsigned int *)(DEVAPC_AO_MM_BASE+0x0F00))

/*******************************************************************************************/
/* Device APC AO for MD */
#define DEVAPC_MD_D0_APC_0            ((unsigned int *)(DEVAPC_AO_MD_BASE+0x0000))

#define DEVAPC_MD_APC_CON             ((unsigned int *)(DEVAPC_AO_MD_BASE+0x0F00))

/*******************************************************************************************/
/* Device APC AO for PERI */
#define DEVAPC_PERI_D0_APC_0          ((unsigned int *)(DEVAPC_AO_PERI_BASE+0x0000))

#define DEVAPC_PERI_MAS_SEC_0         ((unsigned int *)(DEVAPC_AO_PERI_BASE+0x0B00))

#define DEVAPC_PERI_APC_CON           ((unsigned int *)(DEVAPC_AO_PERI_BASE+0x0F00))


/*******************************************************************************************/
/*******************************************************************************************/
/* Device APC PD for Infra/MM/MD */
#define DEVAPC_PD_INFRA_VIO_MASK_0    ((unsigned int *)(DEVAPC_PD_INFRA_BASE+0x0000))

#define DEVAPC_PD_INFRA_VIO_STA_0     ((unsigned int *)(DEVAPC_PD_INFRA_BASE+0x0400))

#define DEVAPC_PD_INFRA_VIO_DBG0      ((unsigned int *)(DEVAPC_PD_INFRA_BASE+0x0900))
#define DEVAPC_PD_INFRA_VIO_DBG1      ((unsigned int *)(DEVAPC_PD_INFRA_BASE+0x0904))

#define DEVAPC_PD_INFRA_APC_CON       ((unsigned int *)(DEVAPC_PD_INFRA_BASE+0x0F00))

/*******************************************************************************************/
/* Device APC PD for PERI */
#define DEVAPC_PD_PERI_VIO_MASK_0     ((unsigned int *)(DEVAPC_PD_PERI_BASE+0x0000))

#define DEVAPC_PD_PERI_VIO_STA_0      ((unsigned int *)(DEVAPC_PD_PERI_BASE+0x0400))

#define DEVAPC_PD_PERI_VIO_DBG0       ((unsigned int *)(DEVAPC_PD_PERI_BASE+0x0900))
#define DEVAPC_PD_PERI_VIO_DBG1       ((unsigned int *)(DEVAPC_PD_PERI_BASE+0x0904))

#define DEVAPC_PD_PERI_APC_CON        ((unsigned int *)(DEVAPC_PD_PERI_BASE+0x0F00))

/*******************************************************************************************/

#define INFRA_AO_SEC_CON               ((unsigned int *)(INFRACFG_AO_BASE+0x0F80))

/* INFRACFG AO */
#define INFRA_AO_SEC_CG_CON0           ((unsigned int *)(INFRACFG_AO_BASE+0x0F84))
#define INFRA_AO_SEC_CG_CON1           ((unsigned int *)(INFRACFG_AO_BASE+0x0F88))
#define INFRA_AO_SEC_CG_CON2           ((unsigned int *)(INFRACFG_AO_BASE+0x0F9C))
#define INFRA_AO_SEC_CG_CON3           ((unsigned int *)(INFRACFG_AO_BASE+0x0FA4))
#define INFRA_AO_SEC_CG_CON4           ((unsigned int *)(INFRACFG_AO_BASE+0x0FA8))

/* PERICFG */
#define PERICFG_SEC_CG_CON0            ((unsigned int *)(PERI_BASE+0x0400))
#define PERICFG_SEC_CG_CON1            ((unsigned int *)(PERI_BASE+0x0404))
#define PERICFG_SEC_CG_CON2            ((unsigned int *)(PERI_BASE+0x0408))
#define PERICFG_SEC_CG_CON3            ((unsigned int *)(PERI_BASE+0x040C))
#define PERICFG_SEC_CG_CON4            ((unsigned int *)(PERI_BASE+0x0410))
#define PERICFG_SEC_CG_CON5            ((unsigned int *)(PERI_BASE+0x0414))

/* PMS(MD devapc) */
#define AP2MD1_PMS_CTRL_EN             ((unsigned int *)0x100008AC)
#define AP2MD1_PMS_CTRL_EN_LOCK        ((unsigned int *)0x100008A8)
/******************************************************************************
 *
 * Variable DEFINATION
 *
 ******************************************************************************/
/* If you config register INFRA_AO_SEC_CON(address 0x10000F80) bit[4] = 1,
 * the domain comes from device_apc. By default this register is 0,
 * the domain comes form MD1 */
#define FORCE_MD1_SIGNAL_FROM_DAPC      ((0x1) << 4)


/* PROTECT BIT FOR INFRACFG AO */
#define SEJ_CG_PROTECT_BIT              ((0x1) << 5)
#define DEVMPU_CG_PROTECT_BIT           ((0x1) << 20)

#define TRNG_CG_PROTECT_BIT             ((0x1) << 9)
#define DEVAPC_INFRA_PD_PROTECT_BIT     ((0x1) << 20)


/* PROTECT BIT FOR PERICFG */
#define UFS_AES_CORE_PROTECT_BIT        ((0x1) << 12)

#define DEVAPC_PERI_PD_PROTECT_BIT      ((0x1) << 9)
#define DXCC_AO_CORE_PROTECT_BIT        ((0x1) << 16)
#define DXCC_Secure_CORE_PROTECT_BIT    ((0x1) << 18)


/*******************************************************************************************/

#define MOD_NO_IN_1_DEVAPC              16

#define MASTER_INFRA_SPM                3
#define MASTER_INFRA_SSPM               4

#define SLAVE_INFRA_SSPM_0              20
#define SLAVE_INFRA_SSPM_8              28

#define MASTER_INFRA_MAX_INDEX          15
#define MASTER_PERI_MAX_INDEX           27

#define SLAVE_INFRA_MAX_INDEX           80
#define SLAVE_MM_MAX_INDEX              139
#define SLAVE_MD_MAX_INDEX              52
#define SLAVE_PERI_MAX_INDEX            47

#define INFRA_MM_MD_MAX_VIO_INDEX       237
#define PERI_MAX_VIO_INDEX              55

#endif //__ATF_DAPC_H__