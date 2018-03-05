#include <mmio.h>

#define PERISYS_BASE          0x11010000
#define PERI_REG(ofs)         (PERISYS_BASE + ofs)
#define PERI_SW_CG_4_STA      PERI_REG(0x02B8)

#define PERI_SW_CG_4_MASK     0x00060000

uint64_t spm_idle_check_secure_cg(__uint64_t x1, __uint64_t x2, __uint64_t x3)
{
	uint32_t val = 0;

	/* Check DXCC CGs: 0x110102B8[18:17] */
	val = ~mmio_read_32(PERI_SW_CG_4_STA);
	val &= PERI_SW_CG_4_MASK;

	return (uint64_t)val;
}
