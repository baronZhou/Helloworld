#include <mmio.h>
#include <delay_timer.h>
#include <bakery_lock.h>
#include <platform.h>
#include <sspm_reg.h>
#include <debug.h>

enum {
	SEM_MCDI = 0x0,
	SEM_MAX = 0x10,
};

#define NF_CPU      8
#define TIMEOUT_US  5

#define SEM_ID_HOST_CPU(id) (2 * (id) + 1)
#define SEM_ID_MCDI         SEM_ID_HOST_CPU(SEM_MCDI)

#define DEBUG_TIMEOUT_EN 0

DEFINE_BAKERY_LOCK(mcdi_lock);

void mcdi_acquire_irq_ctrl(void)
{
	unsigned int val, cnt;

	val = (mmio_read_32(SSPM_HW_SEM) >> SEM_ID_MCDI) & 0x1;
	cnt = TIMEOUT_US;

	if (val == 0) {
		while (cnt > 0) {
			mmio_write_32(SSPM_HW_SEM, (1 << SEM_ID_MCDI));
			val = (mmio_read_32(SSPM_HW_SEM) >> SEM_ID_MCDI) & 0x1;

			if (val == 1)
				return;
			udelay(1);
#if DEBUG_TIMEOUT_EN
			cnt--;
#endif
		}
	}
	ERROR("[MCDI] can NOT get SSPM HW SEMAPHORE!\n");
}

void mcdi_release_irq_ctrl(void)
{
	unsigned int val, cnt;

	val = (mmio_read_32(SSPM_HW_SEM) >> SEM_ID_MCDI) & 0x1;
	cnt = TIMEOUT_US;

	if (val == 1) {
		while (cnt > 0) {
			mmio_write_32(SSPM_HW_SEM, (1 << SEM_ID_MCDI));
			val = (mmio_read_32(SSPM_HW_SEM) >> SEM_ID_MCDI) & 0x1;

			if (val == 0)
				return;
			udelay(1);
#if DEBUG_TIMEOUT_EN
			cnt--;
#endif
		}
	} else {
		INFO("[MCDI] has NOT acquired SPM HW SEMAPHORE\n");
	}
	ERROR("[MCDI] release SPM HW SEMAPHORE fail!\n");
}

void sspm_cluster_pwr_off_notify(unsigned long cluster)
{
    mmio_write_32(SSPM_MBOX_3_BASE + 4 * (MCDI_MBOX_CLUSTER_0_ATF_ACTION_DONE + cluster), 1);
}

void sspm_cluster_pwr_on_notify(unsigned long cluster)
{
    mmio_write_32(SSPM_MBOX_3_BASE + 4 * (MCDI_MBOX_CLUSTER_0_ATF_ACTION_DONE + cluster), 0);
}

void sspm_standbywfi_irq_enable(unsigned long cpu)
{
	unsigned int reg;

	if (cpu >= NF_CPU)
		return;

	bakery_lock_get(&mcdi_lock);
	mcdi_acquire_irq_ctrl();

	/* disable interrupt */
	reg = mmio_read_32(SSPM_INTC_IRQ_EN3);
	mmio_write_32(SSPM_INTC_IRQ_EN3, reg & ~(1 << cpu));

	/* set polarity : 1'b0 high, 1'b1 low */
	reg = mmio_read_32(SSPM_INTC_IRQ_POL3);
	mmio_write_32(SSPM_INTC_IRQ_POL3, reg & ~(1 << cpu));

	/* set group */
	reg = mmio_read_32(SSPM_INTC_GRP_MCDI);
	mmio_write_32(SSPM_INTC_GRP_MCDI, reg | (1 << cpu));

	/* enable interrupt */
	reg = mmio_read_32(SSPM_INTC_IRQ_EN3);
	mmio_write_32(SSPM_INTC_IRQ_EN3, reg | (1 << cpu));

	mcdi_release_irq_ctrl();
	bakery_lock_release(&mcdi_lock);
}

void mcdi_init(void)
{
	bakery_lock_init(&mcdi_lock);
}
