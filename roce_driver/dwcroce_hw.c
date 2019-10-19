/*
*	this file to define the detail operation to hardware
*									--edited by hs
*/

#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/log2.h>
#include <linux/dma_mapping.h>

#include <rdma/ib_verbs.h>
#include <rdma/ib_user_verbs.h>
#include <rdma/ib_cache.h>

#include "dwcroce.h"
#include "dwcroce_hw.h"
#include "dwcroce_ah.h"
#include "dwcroce_verbs.h"

static int phd_ipv4_init(struct dwcroce_dev *dev)
{
	void __iomem *base_addr;
	base_addr = dev->devinfo->base_addr;

	/*ipv4 version*/
	writel(PHD_BASE_0 + PHDIPV4VERSION, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	
	return 0;
}

static int phd_mac_init(struct dwcroce_dev *dev)
{
	void __iomem *base_addr;
	base_addr = dev->devinfo->base_addr;
	/*mac source addr  */
	writel(PHD_BASE_0 + PHDMACSOURCEADDR_H, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDMACSOURCEADDR_L, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDMACSOURCEADDR_H, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDMACSOURCEADDR_L, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*end*/

	/*mac type */
	writel(PHD_BASE_0 + PHDMACTYPE, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDMACTYPE, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*end*/
	return 0;
}

static int phd_rxdesc_init(struct dwcroce_dev *dev)
{
	void __iomem *base_addr;
	base_addr = dev->devinfo->base_addr;
	
	/*rx_desc_tail_lptr_addr start*/
	writel(PHD_BASE_0 + PHDRXDESCTAILPTR_H, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDRXDESCTAILPTR_L, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDRXDESCTAILPTR_H, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDRXDESCTAILPTR_L, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*end*/

	/*rx_desc_tail_thresdhold & tail ptr incr step*/
	writel(PHD_BASE_0 + PHDRXDESCTAILPTR_THRESDHOLD, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDRXDESCTAILPTR_INCR_STEP, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDRXDESCTAILPTR_THRESDHOLD, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDRXDESCTAILPTR_INCR_STEP, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*end*/

	/*rx normal init*/
	writel(PHD_BASE_0 + PHDNORMAL_RDES1, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDNORMAL_RDES2, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDNORMAL_RDES3, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDNORMAL_RDES1, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDNORMAL_RDES2, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDNORMAL_RDES3, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*end*/
	return 0;
}
static int phd_txdesc_init(struct dwcroce_dev *dev)
{
	/*对Phd的发送描述符进行初始化*/
	void __iomem *base_addr;
	base_addr = dev->devinfo->base_addr;
	/*tx_desc_tail_lptr_addr start*/
	writel(PHD_BASE_0 + PHDTXDESCTAILPTR_H, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDTXDESCTAILPTR_L, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDTXDESCTAILPTR_H, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDTXDESCTAILPTR_L, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*end*/

	/*tx_desc_tail_ptr_thresdhold*/
	writel(PHD_BASE_0 + PHDTXDESCTAILPTR_THRESDHOLD, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDTXDESCTAILPTR_THRESDHOLD, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*end*/

	/*tx context PHD0 desc init*/
	writel(PHD_BASE_0 + PHDCONTEXT_TDES0, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDCONTEXT_TDES1, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDCONTEXT_TDES2, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDCONTEXT_TDES3, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*end*/

	/*tx context PHD1 desc init*/
	writel(PHD_BASE_1 + PHDCONTEXT_TDES0, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDCONTEXT_TDES1, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDCONTEXT_TDES2, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDCONTEXT_TDES3, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*end*/

	/*tx normal PHD0 desc init*/
	writel(PHD_BASE_0 + PHDNORMAL_TDES1, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDNORMAL_TDES2, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDNORMAL_TDES3, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*end*/

	/*tx normal PHD1 desc init*/
	writel(PHD_BASE_1 + PHDNORMAL_TDES1, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDNORMAL_TDES2, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDNORMAL_TDES3, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*end*/
	
	return 0;

}


static int dwcroce_init_phd(struct dwcroce_dev *dev)
{
	int status;
	status = phd_txdesc_init(dev);
	if (status)
		goto phdtxrxdesc_err;
	status = phd_rxdesc_init(dev);
	if (status)
		goto phdtxrxdesc_err;
	status = phd_mac_init(dev);
	if (status)
		goto mac_err;
	status = phd_ipv4_init(dev);
	if (status)
		goto iperr;
	status = phd_ipv6_init(dev);
	if (status)
		goto iperr;
	status = phd_udp_init(dev);
	if (status)
		goto udperr;
	return 0;
udperr:
	printk("err: udperr\n");
iperr:
	printk("err: iperr\n");
mac_err:
	printk("err: macerr\n");
phdtxrxdesc_err:
	printk("err: phd txrxdescerr!\n");//added by hs for info
	return status
}

int dwcroce_init_hw(struct dwcroce_dev *dev)
{
	int status;

}