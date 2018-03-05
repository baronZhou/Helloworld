#include <mmio.h>

#define MCU_ALL_PWR_ON_CTRL    0x0C530B58
#define PLAT_SRAM_FLAG0 (0x11dbf0)
#define PLAT_SRAM_FLAG1 (0x11dbf4)
#define PLAT_SRAM_FLAG2 (0x11dbf8)
/*
 * PLAT_FLAG0:
 * bit[0:0] = lastpc_valid,
 * bit[1:1] = lastpc_valid_before_reboot,
 * bit[2:4] = user_id_of_multi_user_etb_0,
 * bit[5:7] = user_id_of_multi_user_etb_1,
 * bit[8:10] = user_id_of_multi_user_etb_2,
 * bit[11:13] = user_id_of_multi_user_etb_3,
 * bit[14:16] = user_id_of_multi_user_etb_4,
 * bit[17:19] = user_id_of_multi_user_etb_5,
 * bit[20:22] = user_id_of_multi_user_etb_6,
 * bit[23:25] = user_id_of_multi_user_etb_7,
 * bit[26:28] = user_id_of_multi_user_etb_8,
 * bit[29:31] = user_id_of_multi_user_etb_9,
 */
#define OFFSET_LASTPC_VALID                     0
#define OFFSET_LASTPC_VALID_BEFORE_REBOOT       1
#define sync_writel(addr, val)  do { mmio_write_32((addr), (val)); dsbsy(); } while (0)

#ifdef CFG_PLAT_SRAM_FLAG
/* return negative integer if fails */
static int set_sram_flag_lastpc_valid(void)
{
	int config;

	config = mmio_read_32(PLAT_SRAM_FLAG0) | (1 << OFFSET_LASTPC_VALID);
	sync_writel(PLAT_SRAM_FLAG0, config);
	dsb();
	return 0;
}
#endif

void lastpc_postinit(void)
{
       unsigned int ctrl = mmio_read_32(MCU_ALL_PWR_ON_CTRL);

       /*
        * clear
        * MCU_ALL_PWR_ON_CTRL[1] mcu_pwr_on and
        * MCU_ALL_PWR_ON_CTRL[2] mcu_pwr_iso_dis
        * to let non-booting cores pwr off
        */
       ctrl &= ~(0x3<<1);

       mmio_write_32(MCU_ALL_PWR_ON_CTRL, ctrl);
#ifdef CFG_PLAT_SRAM_FLAG
       set_sram_flag_lastpc_valid();
#endif
}
