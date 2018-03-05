#include <arch_helpers.h>
#include <assert.h>
#include <bl_common.h>
#include <debug.h>
#include <emi_drv.h>
#include <interrupt_mgmt.h>
#include <mmio.h>
#include <mtk_sip_svc.h>
//#include <plat_config.h>
//#include <plat_def.h>
#include <plat_private.h>
#include <platform.h>
#include <platform_def.h>
#include <stdint.h>
#include <stdio.h>

static unsigned char R[EMIMPU_REGION_NUM];

uint32_t sip_emimpu_set_region_protection_a32(unsigned int start,
			unsigned int end, unsigned int apc8, unsigned int apc0)
{
	unsigned long long region_permission;
	unsigned long long start_a64;
	unsigned long long end_a64;

	start_a64 = (unsigned long long) start |
			((unsigned long long) (apc8 & 0xF0000000) << 4);
	end_a64 = (unsigned long long) end |
			((unsigned long long) (apc8 & 0x0F000000) << 8);
	region_permission = (((unsigned long long) apc8 & 0x00FFFFFF) << 32) | apc0;
	sip_emimpu_set_region_protection(start_a64, end_a64, region_permission);

	return MTK_SIP_E_SUCCESS;
}

uint32_t sip_emimpu_set_region_protection(unsigned long long start,
				unsigned long long end, unsigned long long region_permission)
{

	unsigned int region;
	unsigned int access_permission0, access_permission8, locked;

	region = (region_permission >> 27) & 0x1F;
	access_permission0 = region_permission & 0xFFFFFF;
	access_permission8 = (region_permission >> 32) & 0xFFFFFF;
	locked = (region_permission >> 26) & 0x1;

	if (R[region] == 1)
		return MTK_SIP_E_PERMISSION_DENY;
	else {
		if (locked)
			R[region] = 1;
	}

	if((end != 0) || (start != 0))
	{
		/* if not 4GB mode need offset 0x4000000 */
		start -= 0x40000000;
		end -= 0x40000000;

		/*Address 64KB alignment*/
		start = (start >> 16) & 0x1FFFF;
		end = (end >> 16) & 0x1FFFF;

		if (end < start)
		{
			return MTK_SIP_E_INVALID_RANGE;
		}
	}

	mmio_write_32(EMI_MPU_SA(region), start);
	mmio_write_32(EMI_MPU_EA(region), end);
	mmio_write_32(EMI_MPU_APC(region, 8), access_permission8);
	mmio_write_32(EMI_MPU_APC(region, 0), access_permission0);

	return MTK_SIP_E_SUCCESS;
}


static int is_reg_access_forbidden(unsigned int offset)
{
	if ((offset >= EMI_MPU_START) && (offset <= EMI_MPU_END))
		return 0;
	else
		return 1;
}


uint64_t sip_emimpu_write(unsigned int offset, unsigned int reg_value)
{
	unsigned int addr;

	if (is_reg_access_forbidden(offset))
		return MTK_SIP_E_INVALID_RANGE;

	addr = EMI_MPU_BASE + offset;
	mmio_write_32(addr, reg_value);
	dsb();

	return MTK_SIP_E_SUCCESS;
}

uint32_t sip_emimpu_read(unsigned int offset)
{
	unsigned int addr;

	if (is_reg_access_forbidden(offset))
		return MTK_SIP_E_INVALID_RANGE;

	addr = EMI_MPU_BASE + offset;
	return mmio_read_32(addr);
}

/*
 * emi_mpu_set_region_protection: protect a region.
 * @start: start address of the region
 * @end: end address of the region
 * @region: EMI MPU region id
 * @access_permission: EMI MPU access permission
 * Return 0 for success, otherwise negative status code.
 */

uint32_t emi_mpu_set_region_protection(unsigned long long start,
unsigned long long end, int region, unsigned long long access_permission)
{
	int ret = 0;

	access_permission = access_permission | ((region & 0x1F) << 27);
	sip_emimpu_set_region_protection(start, end, access_permission);

	return ret;
}

