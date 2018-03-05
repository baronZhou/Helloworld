#ifndef __MT_SPM_VCOREFS__H__
#define __MT_SPM_VCOREFS__H__

__uint64_t spm_vcorefs_args(__uint64_t x1, __uint64_t x2, __uint64_t x3);

enum vcorefs_smc_cmd {
	VCOREFS_SMC_CMD_0, /* init setting */
	VCOREFS_SMC_CMD_1, /* load fw */
	VCOREFS_SMC_CMD_2, /* set opp */
	VCOREFS_SMC_CMD_3, /* write */
	VCOREFS_SMC_CMD_4, /* read */
	NUM_VCOREFS_SMC_CMD,
};

enum dvfsrc_channel {
	DVFSRC_CHANNEL_1 = 1,
	DVFSRC_CHANNEL_2,
	DVFSRC_CHANNEL_3,
	DVFSRC_CHANNEL_4,
	NUM_DVFSRC_CHANNEL,
};

enum dvfs_opp {
	OPP_0 = 0,
	OPP_1,
	OPP_2,
	OPP_3,
	NUM_OPP,
	OPP_UNREQ = 0xf, /* as -1 in kernel */
};
enum dvfs_kicker_group {
	KIR_GROUP_FIX = 0,
	KIR_GROUP_HPM,
	KIR_GROUP_HPM_NON_FORCE,
	NUM_KIR_GROUP,
};
enum vcorefs_setclr_reg_id {
	ID_SPM_DVFS_INITIAL_MASK = 0,
	ID_SPM_DVFS_FLOOR_MASK,
	ID_SPM_DVFS_FLOOR_MASK2,
	ID_SPM_DVFS_FLOOR_MASK3,
	ID_SPM_DVFS_FLOOR_MASK4,
	ID_SPM_DVFS_FORCE_MASK0,
	ID_SPM_DVFS_FORCE,
	ID_SETCLR_NUM,
};

struct spm_setclr_reg {
	unsigned int id;
	char* name;
	__uint32_t set_addr;
	__uint32_t clr_addr;
	__uint32_t sta_addr;
};

#define CFG_SPM_SETCLR_REG(_id, _name, _set_addr, _clr_addr, _sta_addr) {	\
				.id = _id,					\
				.name = _name,					\
				.set_addr = _set_addr,				\
				.clr_addr = _clr_addr,				\
				.sta_addr = _sta_addr,				\
			}

#endif /* __MT_SPM_VCOREFS__H__ */
