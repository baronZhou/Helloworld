#ifndef __SSPM_REG_H__
#define __SSPM_REG_H__

#include "platform_def.h"

#define SSPM_REG_BASE           (IO_PHYS + 0x940000)     /* SSPM view: 0x30040000 */
#define SSPM_REG_ADDR(ofs)      (SSPM_REG_BASE + (ofs))

#define SSPM_MBOX_3_BASE        (IO_PHYS + 0x980000)

#define SSPM_HW_SEM             SSPM_REG_ADDR(0x0048)
#define SSPM_INTC_IRQ_EN3       SSPM_REG_ADDR(0x202C)
#define SSPM_INTC_IRQ_POL3      SSPM_REG_ADDR(0x204C)
#define SSPM_INTC_IRQ_GRP4_3    SSPM_REG_ADDR(0x209C)
#define SSPM_INTC_IRQ_GRP6_3    SSPM_REG_ADDR(0x20BC)

#define SSPM_INTC_GRP_MCDI      SSPM_INTC_IRQ_GRP6_3

#define NF_MCDI_MBOX                            16
#define MCDI_MBOX_CLUSTER_0_CAN_POWER_OFF       0
#define MCDI_MBOX_CLUSTER_1_CAN_POWER_OFF       1
#define MCDI_MBOX_CLUSTER_2_CAN_POWER_OFF       2
#define MCDI_MBOX_CLUSTER_0_ATF_ACTION_DONE     3
#define MCDI_MBOX_CLUSTER_1_ATF_ACTION_DONE     4
#define MCDI_MBOX_CLUSTER_2_ATF_ACTION_DONE     5
#define MCDI_MBOX_PAUSE_ACTION                  6
#define MCDI_MBOX_CPU_CLUSTER_PWR_STAT          7
#define MCDI_MBOX_ACTION_STAT                   8

#endif /* __SSPM_REG_H__ */
