/*
 * Copyright (c) 2013-2015, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <debug.h>
#include <mmio.h>
#include <platform.h>
#include <platform_def.h>

/* allow dump once by one CPU to avoid dummy dump by all other CPUs */

void plat_rgu_dump_reg(void)
{
	unsigned int wdt_status;

	/* dump RGU registers */

	INFO("[RGU-ATF] MODE:0x%x\n", mmio_read_32(MTK_WDT_MODE));
	INFO("[RGU-ATF] LENGTH:0x%x\n", mmio_read_32(MTK_WDT_LENGTH));

	wdt_status = mmio_read_32(MTK_WDT_STATUS);

	INFO("[RGU-ATF] STATUS:0x%x\n", wdt_status);
	INFO("[RGU-ATF] INTERVAL:0x%x\n", mmio_read_32(MTK_WDT_INTERVAL));
	INFO("[RGU-ATF] NONRST_REG:0x%x\n", mmio_read_32(MTK_WDT_NONRST_REG));
	INFO("[RGU-ATF] NONRST_REG2:0x%x\n", mmio_read_32(MTK_WDT_NONRST_REG2));
	INFO("[RGU-ATF] REQ_MODE:0x%x\n", mmio_read_32(MTK_WDT_REQ_MODE));
	INFO("[RGU-ATF] REQ_IRQ_EN:0x%x\n", mmio_read_32(MTK_WDT_REQ_IRQ_EN));
	INFO("[RGU-ATF] EXT_REQ_CON:0x%x\n", mmio_read_32(MTK_WDT_EXT_REQ_CON));
	INFO("[RGU-ATF] DEBUG_CTL:0x%x\n", mmio_read_32(MTK_WDT_DEBUG_CTL));
	INFO("[RGU-ATF] LATCH_CTL:0x%x\n", mmio_read_32(MTK_WDT_LATCH_CTL));
	INFO("[RGU-ATF] DEBUG_CTL2:0x%x\n", mmio_read_32(MTK_WDT_DEBUG_CTL2));
	INFO("[RGU-ATF] COUNTER:0x%x\n", mmio_read_32(MTK_WDT_COUNTER));

	/* analyze interrupt source */

	INFO("[RGU-ATF] Status:\n");

	if (wdt_status & MTK_WDT_STATUS_SPM_THERMAL_RST)
		INFO("[RGU-ATF]   SPM_THERMAL\n");

	if (wdt_status & MTK_WDT_STATUS_SPM_RST)
		INFO("[RGU-ATF]   SPM\n");

	if (wdt_status & MTK_WDT_STATUS_EINT_RST)
		INFO("[RGU-ATF]   EINT\n");

	if (wdt_status & MTK_WDT_STATUS_SYSRST_RST)
		INFO("[RGU-ATF]   SYSRST\n");

	if (wdt_status & MTK_WDT_STATUS_DVFSP_RST)
		INFO("[RGU-ATF]   DVFSP\n");

	if (wdt_status & MTK_WDT_STATUS_PMCU_RST)
		INFO("[RGU-ATF]   SSPM\n");

	if (wdt_status & MTK_WDT_STATUS_MDDBG_RST)
		INFO("[RGU-ATF]   MDDBG\n");

	if (wdt_status & MTK_WDT_STATUS_THERMAL_DIRECT_RST)
		INFO("[RGU-ATF]   THERMAL_DIRECT\n");

	if (wdt_status & MTK_WDT_STATUS_DEBUG_RST)
		INFO("[RGU-ATF]   DBG\n");

	if (wdt_status & MTK_WDT_STATUS_SECURITY_RST)
		INFO("[RGU-ATF]   SEC\n");

	if (wdt_status & MTK_WDT_STATUS_IRQ_ASSERT)
		INFO("[RGU-ATF]   IRQ\n");

	if (wdt_status & MTK_WDT_STATUS_SW_WDT_RST)
		INFO("[RGU-ATF]   SW_WDT\n");

	if (wdt_status & MTK_WDT_STATUS_HW_WDT_RST)
		INFO("[RGU-ATF]   HW_WDT\n");
}

