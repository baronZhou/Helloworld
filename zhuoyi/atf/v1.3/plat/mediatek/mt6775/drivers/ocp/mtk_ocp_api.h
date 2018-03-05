#ifndef __MTK_OCP_API_H
#define __MTK_OCP_API_H

/**
 * @file    mtk_ocp_api.h
 * @brief   Driver header for Over Current Protect
 *
 */
#include <debug.h>
#include <mmio.h>

#define OCP_DVT		(0)

/*
 * BIT Operation
 */
#undef  BIT
#define BIT(_bit_)                    (unsigned)(1 << (_bit_))
#define BITS(_bits_, _val_)           \
	((((unsigned) -1 >> (31 - ((1) ? _bits_))) & ~((1U << ((0) ? _bits_)) - 1)) & ((_val_)<<((0) ? _bits_)))
#define BITMASK(_bits_)               (((unsigned) -1 >> (31 - ((1) ? _bits_))) & ~((1U << ((0) ? _bits_)) - 1))
#define GET_BITS_VAL(_bits_, _val_)   (((_val_) & (BITMASK(_bits_))) >> ((0) ? _bits_))

/**
 * Read/Write a field of a register.
 * @addr:       Address of the register
 * @range:      The field bit range in the form of MSB:LSB
 * @val:        The value to be written to the field
 */
#define ocp_read(addr)				mmio_read_32(addr)
#define ocp_read_field(addr, range)		GET_BITS_VAL(range, ocp_read(addr))
#define ocp_write(addr, val)			mmio_write_32(addr, val)
#define ocp_write_field(addr, range, val)	ocp_write(addr, (ocp_read(addr) & ~(BITMASK(range))) | BITS(range, val))

/* LOG */
#define OCP_DEBUG_LOG_ON	1
#define TAG	"[OCP]"
#define ocp_err(fmt, ...)	ERROR(TAG""fmt, __VA_ARGS__)
#define ocp_warn(fmt, ...)	WARN(TAG""fmt, __VA_ARGS__)
#define ocp_info(fmt, ...)	INFO(TAG""fmt, __VA_ARGS__)
#if OCP_DEBUG_LOG_ON
#define ocp_dbg(...)		ocp_info(__VA_ARGS__)
#else
#define ocp_dbg(...)
#endif


/* enum */
enum ocp_unit {
	OCP_MA = 0,
	OCP_MW,

	NR_OCP_UNIT,
};

enum ocp_cluster {
	OCP_LL = 0,
	OCP_L,
	OCP_B,

	NR_OCP_CLUSTER,
};

/* Set OCPCFG bits {VoltBypassEn, LkgShare, LkgBypassEn, MHzBypassEn} = ConfigMode */
enum ocp_mode {
	NO_BYPASS = 0,
	MHZ_BYPASS,
	LKG_BYPASS,
	MHZ_LKG_BYPASS,
	LKG_SHARE = 4,
	SHARE_MHZ_BYPASS,
	SHARE_LKG_BYPASS,
	SHARE_MHZ_LKG_BYPASS,
	VOLT_BYPASS,
	VOLT_MHZ_BYPASS = 9,
	VOLT_LKG_BYPASS,
	VOLT_MHZ_LKG_BYPASS = 11,
	VOLT_BYPASS_LKG_SHARE,
	VOLT_MHZ_BYPASS_SHARE,
	VOLT_LKG_BYPASS_SHARE,
	VOLT_MHZ_LKG_BYPASS_SHARE = 15,

	NR_OCP_MODE,
};

enum ocp_int_select {
	IRQ_CLK_PCT_MIN = 0,
	IRQ_WA_MAX,
	IRQ_WA_MIN,
};

enum ocp_value_select {
	CLK_AVG = 0,
	WA_AVG,
	TOP_RAW_LKG,
	CPU0_RAW_LKG,
	CPU1_RAW_LKG,
	CPU2_RAW_LKG,
	CPU3_RAW_LKG,
};

#define OCP_CLK_PCT_MIN			(625)
#define OCP_TARGET_MAX			(65535)
#define OCP_FREQ_PCT_100_L			(2350)
#define OCP_FREQ_PCT_100_LL			(1640)
#define OCP_LL_DEFAULT_FREQ			(1248)	/* 1248M */
#define OCP_LL_DEFAULT_VOLT			(950)
#define OCP_LL_DEFAULT_LKG			(100)	/* OCPv3 only */
#define OCP_L_DEFAULT_FREQ			(793)	/* 793M */
#define OCP_L_DEFAULT_VOLT			(950)
#define OCP_L_DEFAULT_LKG			(100)	/* OCPv3 only */
#define OCP_DEFAULT_AVG_WINDOW		(1)
#define OCP_B_DEFAULT_FREQ			(1885)	/* 1885 M */
#define OCP_B_DEFAULT_VOLT			(973)
#define OCP_B_DEFAULT_LKG			(0)
#define OCP_B_DEFAULT_AVG_WINDOW	(8)

/* eFuse */
#define PTP3_OD0	(0x10206660)

/**
 * OCP control register
 */
#define OCP_REG_BASE_ADDR	LITTLE_OCP_BASE_ADDR
#define OCP_REG_BANK_SIZE	(0x4000)

/* Big */
#define BIG_OCP_BASE_ADDR	((unsigned int) 0x10202000)

#define MP2_OCPAPB00	(BIG_OCP_BASE_ADDR + 0x500)
#define MP2_OCPAPB01	(BIG_OCP_BASE_ADDR + 0x504)
#define MP2_OCPAPB02	(BIG_OCP_BASE_ADDR + 0x508)
#define MP2_OCPAPB03	(BIG_OCP_BASE_ADDR + 0x50C)
#define MP2_OCPAPB04	(BIG_OCP_BASE_ADDR + 0x510)
#define MP2_OCPAPB05	(BIG_OCP_BASE_ADDR + 0x514)
#define MP2_OCPAPB06	(BIG_OCP_BASE_ADDR + 0x518)
#define MP2_OCPAPB07	(BIG_OCP_BASE_ADDR + 0x51C)
#define MP2_OCPAPB08	(BIG_OCP_BASE_ADDR + 0x520)
#define MP2_OCPAPB09	(BIG_OCP_BASE_ADDR + 0x524)
#define MP2_OCPAPB0A	(BIG_OCP_BASE_ADDR + 0x528)
#define MP2_OCPAPB0B	(BIG_OCP_BASE_ADDR + 0x52C)
#define MP2_OCPAPB0C	(BIG_OCP_BASE_ADDR + 0x530)
#define MP2_OCPAPB0D	(BIG_OCP_BASE_ADDR + 0x534)
#define MP2_OCPAPB0E	(BIG_OCP_BASE_ADDR + 0x538)
#define MP2_OCPAPB0F	(BIG_OCP_BASE_ADDR + 0x53C)
#define MP2_OCPAPB10	(BIG_OCP_BASE_ADDR + 0x540)
#define MP2_OCPAPB11	(BIG_OCP_BASE_ADDR + 0x544)
#define MP2_OCPAPB12	(BIG_OCP_BASE_ADDR + 0x548)
#define MP2_OCPAPB13	(BIG_OCP_BASE_ADDR + 0x54C)
#define MP2_OCPAPB14	(BIG_OCP_BASE_ADDR + 0x550)
#define MP2_OCPAPB15	(BIG_OCP_BASE_ADDR + 0x554)
#define MP2_OCPAPB16	(BIG_OCP_BASE_ADDR + 0x558)

/* Little */
#define LITTLE_OCP_BASE_ADDR	((unsigned int) 0x10200000)

#define MP0_OCPCPUPOST_CTRL0	(LITTLE_OCP_BASE_ADDR + 0x1030)
#define MP0_OCPCPUPOST_CTRL1	(LITTLE_OCP_BASE_ADDR + 0x1034)
#define MP0_OCPCPUPOST_CTRL2	(LITTLE_OCP_BASE_ADDR + 0x1038)
#define MP0_OCPCPUPOST_CTRL3	(LITTLE_OCP_BASE_ADDR + 0x103C)
#define MP0_OCPCPUPOST_CTRL4	(LITTLE_OCP_BASE_ADDR + 0x1040)
#define MP0_OCPCPUPOST_CTRL5	(LITTLE_OCP_BASE_ADDR + 0x1044)
#define MP0_OCPCPUPOST_CTRL6	(LITTLE_OCP_BASE_ADDR + 0x1048)
#define MP0_OCPCPUPOST_CTRL7	(LITTLE_OCP_BASE_ADDR + 0x104C)

#define MP0_OCPNCPUPOST_CTRL	(LITTLE_OCP_BASE_ADDR + 0x1070)

#define MP0_OCPAPB00	(LITTLE_OCP_BASE_ADDR + 0x1500)
#define MP0_OCPAPB01	(LITTLE_OCP_BASE_ADDR + 0x1504)
#define MP0_OCPAPB02	(LITTLE_OCP_BASE_ADDR + 0x1508)
#define MP0_OCPAPB03	(LITTLE_OCP_BASE_ADDR + 0x150C)
#define MP0_OCPAPB04	(LITTLE_OCP_BASE_ADDR + 0x1510)
#define MP0_OCPAPB05	(LITTLE_OCP_BASE_ADDR + 0x1514)
#define MP0_OCPAPB06	(LITTLE_OCP_BASE_ADDR + 0x1518)
#define MP0_OCPAPB07	(LITTLE_OCP_BASE_ADDR + 0x151C)
#define MP0_OCPAPB08	(LITTLE_OCP_BASE_ADDR + 0x1520)
#define MP0_OCPAPB09	(LITTLE_OCP_BASE_ADDR + 0x1524)
#define MP0_OCPAPB0A	(LITTLE_OCP_BASE_ADDR + 0x1528)
#define MP0_OCPAPB0B	(LITTLE_OCP_BASE_ADDR + 0x152C)
#define MP0_OCPAPB0C	(LITTLE_OCP_BASE_ADDR + 0x1530)
#define MP0_OCPAPB0D	(LITTLE_OCP_BASE_ADDR + 0x1534)
#define MP0_OCPAPB0E	(LITTLE_OCP_BASE_ADDR + 0x1538)
#define MP0_OCPAPB0F	(LITTLE_OCP_BASE_ADDR + 0x153C)
#define MP0_OCPAPB10	(LITTLE_OCP_BASE_ADDR + 0x1540)
#define MP0_OCPAPB11	(LITTLE_OCP_BASE_ADDR + 0x1544)
#define MP0_OCPAPB12	(LITTLE_OCP_BASE_ADDR + 0x1548)
#define MP0_OCPAPB13	(LITTLE_OCP_BASE_ADDR + 0x154C)
#define MP0_OCPAPB14	(LITTLE_OCP_BASE_ADDR + 0x1550)
#define MP0_OCPAPB15	(LITTLE_OCP_BASE_ADDR + 0x1554)
#define MP0_OCPAPB16	(LITTLE_OCP_BASE_ADDR + 0x1558)
#define MP0_OCPAPB17	(LITTLE_OCP_BASE_ADDR + 0x155C)
#define MP0_OCPAPB18	(LITTLE_OCP_BASE_ADDR + 0x1560)
#define MP0_OCPAPB19	(LITTLE_OCP_BASE_ADDR + 0x1564)
#define MP0_OCPAPB1A	(LITTLE_OCP_BASE_ADDR + 0x1568)
#define MP0_OCPAPB1B	(LITTLE_OCP_BASE_ADDR + 0x156C)
#define MP0_OCPAPB1C	(LITTLE_OCP_BASE_ADDR + 0x1570)
#define MP0_OCPAPB1D	(LITTLE_OCP_BASE_ADDR + 0x1574)
#define MP0_OCPAPB1E	(LITTLE_OCP_BASE_ADDR + 0x1578)
#define MP0_OCPAPB1F	(LITTLE_OCP_BASE_ADDR + 0x157C)
#define MP0_OCPAPB20	(LITTLE_OCP_BASE_ADDR + 0x1580)
#define MP0_OCPAPB21	(LITTLE_OCP_BASE_ADDR + 0x1584)
#define MP0_OCPAPB22	(LITTLE_OCP_BASE_ADDR + 0x1588)
#define MP0_OCPAPB23	(LITTLE_OCP_BASE_ADDR + 0x158C)
#define MP0_OCPAPB24	(LITTLE_OCP_BASE_ADDR + 0x1590)
#define MP0_OCPAPB25	(LITTLE_OCP_BASE_ADDR + 0x1594)

#define MP0_OCP_GENERAL_CTRL	(LITTLE_OCP_BASE_ADDR + 0x17FC)

#define MP1_OCPCPUPOST_CTRL0	(LITTLE_OCP_BASE_ADDR + 0x3030)
#define MP1_OCPCPUPOST_CTRL1	(LITTLE_OCP_BASE_ADDR + 0x3034)
#define MP1_OCPCPUPOST_CTRL2	(LITTLE_OCP_BASE_ADDR + 0x3038)
#define MP1_OCPCPUPOST_CTRL3	(LITTLE_OCP_BASE_ADDR + 0x303C)
#define MP1_OCPCPUPOST_CTRL4	(LITTLE_OCP_BASE_ADDR + 0x3040)
#define MP1_OCPCPUPOST_CTRL5	(LITTLE_OCP_BASE_ADDR + 0x3044)
#define MP1_OCPCPUPOST_CTRL6	(LITTLE_OCP_BASE_ADDR + 0x3048)
#define MP1_OCPCPUPOST_CTRL7	(LITTLE_OCP_BASE_ADDR + 0x304C)

#define MP1_OCPNCPUPOST_CTRL	(LITTLE_OCP_BASE_ADDR + 0x3070)

#define MP1_OCPAPB00	(LITTLE_OCP_BASE_ADDR + 0x3500)
#define MP1_OCPAPB01	(LITTLE_OCP_BASE_ADDR + 0x3504)
#define MP1_OCPAPB02	(LITTLE_OCP_BASE_ADDR + 0x3508)
#define MP1_OCPAPB03	(LITTLE_OCP_BASE_ADDR + 0x350C)
#define MP1_OCPAPB04	(LITTLE_OCP_BASE_ADDR + 0x3510)
#define MP1_OCPAPB05	(LITTLE_OCP_BASE_ADDR + 0x3514)
#define MP1_OCPAPB06	(LITTLE_OCP_BASE_ADDR + 0x3518)
#define MP1_OCPAPB07	(LITTLE_OCP_BASE_ADDR + 0x351C)
#define MP1_OCPAPB08	(LITTLE_OCP_BASE_ADDR + 0x3520)
#define MP1_OCPAPB09	(LITTLE_OCP_BASE_ADDR + 0x3524)
#define MP1_OCPAPB0A	(LITTLE_OCP_BASE_ADDR + 0x3528)
#define MP1_OCPAPB0B	(LITTLE_OCP_BASE_ADDR + 0x352C)
#define MP1_OCPAPB0C	(LITTLE_OCP_BASE_ADDR + 0x3530)
#define MP1_OCPAPB0D	(LITTLE_OCP_BASE_ADDR + 0x3534)
#define MP1_OCPAPB0E	(LITTLE_OCP_BASE_ADDR + 0x3538)
#define MP1_OCPAPB0F	(LITTLE_OCP_BASE_ADDR + 0x353C)
#define MP1_OCPAPB10	(LITTLE_OCP_BASE_ADDR + 0x3540)
#define MP1_OCPAPB11	(LITTLE_OCP_BASE_ADDR + 0x3544)
#define MP1_OCPAPB12	(LITTLE_OCP_BASE_ADDR + 0x3548)
#define MP1_OCPAPB13	(LITTLE_OCP_BASE_ADDR + 0x354C)
#define MP1_OCPAPB14	(LITTLE_OCP_BASE_ADDR + 0x3550)
#define MP1_OCPAPB15	(LITTLE_OCP_BASE_ADDR + 0x3554)
#define MP1_OCPAPB16	(LITTLE_OCP_BASE_ADDR + 0x3558)
#define MP1_OCPAPB17	(LITTLE_OCP_BASE_ADDR + 0x355C)
#define MP1_OCPAPB18	(LITTLE_OCP_BASE_ADDR + 0x3560)
#define MP1_OCPAPB19	(LITTLE_OCP_BASE_ADDR + 0x3564)
#define MP1_OCPAPB1A	(LITTLE_OCP_BASE_ADDR + 0x3568)
#define MP1_OCPAPB1B	(LITTLE_OCP_BASE_ADDR + 0x356C)
#define MP1_OCPAPB1C	(LITTLE_OCP_BASE_ADDR + 0x3570)
#define MP1_OCPAPB1D	(LITTLE_OCP_BASE_ADDR + 0x3574)
#define MP1_OCPAPB1E	(LITTLE_OCP_BASE_ADDR + 0x3578)
#define MP1_OCPAPB1F	(LITTLE_OCP_BASE_ADDR + 0x357C)
#define MP1_OCPAPB20	(LITTLE_OCP_BASE_ADDR + 0x3580)
#define MP1_OCPAPB21	(LITTLE_OCP_BASE_ADDR + 0x3584)
#define MP1_OCPAPB22	(LITTLE_OCP_BASE_ADDR + 0x3588)
#define MP1_OCPAPB23	(LITTLE_OCP_BASE_ADDR + 0x358C)
#define MP1_OCPAPB24	(LITTLE_OCP_BASE_ADDR + 0x3590)
#define MP1_OCPAPB25	(LITTLE_OCP_BASE_ADDR + 0x3594)

#define MP1_OCP_GENERAL_CTRL	(LITTLE_OCP_BASE_ADDR + 0x37FC)


/************************************/
#if OCP_DVT
extern int OCPRegWrite(unsigned int addr, unsigned int val);
extern int OCPRegRead(unsigned int addr);
#endif
extern int OCPEnDis(enum ocp_cluster cluster, unsigned int enable, enum ocp_mode mode);
extern int OCPTarget(enum ocp_cluster cluster, unsigned int target);
extern int OCPFreq(enum ocp_cluster cluster, unsigned int freq);
extern int OCPVoltage(enum ocp_cluster cluster, unsigned int volt);
extern int OCPIntClr(enum ocp_cluster cluster, int value2, int value1, int value0);
extern int OCPIntEnDis(enum ocp_cluster cluster, int value2, int value1, int value0);
extern int OCPIntLimit(enum ocp_cluster cluster, enum ocp_int_select select, int limit);
extern int OCPValueStatus(enum ocp_cluster cluster, enum ocp_value_select select);
extern int OCPIRQHoldoff(enum ocp_cluster cluster, enum ocp_int_select select, int window);
extern int OCPConfigMode(enum ocp_cluster cluster, enum ocp_mode mode);
extern int OCPLeakage(enum ocp_cluster cluster, unsigned int leakage);
/************************************/

#endif /* end of __MTK_OCP_API_H */

