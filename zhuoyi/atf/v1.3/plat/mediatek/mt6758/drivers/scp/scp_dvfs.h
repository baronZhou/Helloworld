#ifndef __MT_SCP_DVFS__H__
#define __MT_SCP_DVFS__H__

typedef enum  {
	CLK_OPP0 = 82,
	CLK_OPP1 = 250,
	CLK_OPP2 = 328,
	CLK_INVALID_OPP,
} clk_opp_enum;

void scp_set_hwdvfs(__uint64_t x1, __uint64_t x2, __uint64_t x3);

#endif /* __MT_SCP_DVFS__H__ */
