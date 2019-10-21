/*
*	this file to define the detail operation to hardware
*									--edited by hs
*/

#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/log2.h>
#include <linux/dma-mapping.h>

#include <rdma/ib_verbs.h>
#include <rdma/ib_user_verbs.h>
#include <rdma/ib_cache.h>

#include "dwcroce.h"
#include "dwcroce_hw.h"
#include "dwcroce_ah.h"
#include "dwcroce_verbs.h"

static int phd_udp_init(struct dwcroce_dev *dev)
{
	void __iomem* base_addr;
	base_addr = dev->devinfo->base_addr;
	printk("dwcroce: no need to init udp\n");//added by hs
	return 0;
}


static int phd_ipv6_init(struct dwcroce_dev *dev)
{
	void __iomem* base_addr;
	base_addr = dev->devinfo->base_addr;

	/*ipv6 init phd 0*/
	writel(PHD_BASE_0 + PHDIPV6VERSION, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 +	PHDIPV6CLASS, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDIPV6FLOWLABLE, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDIPV6NEXTHEADER, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDIPV6HOPLIMIT, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDIPV6SOURCEADDR_0, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDIPV6SOURCEADDR_1, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDIPV6SOURCEADDR_2, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDIPV6SOURCEADDR_3, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*END*/
	/*IPV6 INIT PHD 1*/
	writel(PHD_BASE_1 + PHDIPV6VERSION, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV6CLASS, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV6FLOWLABLE, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV6NEXTHEADER, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV6HOPLIMIT, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV6SOURCEADDR_0, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV6SOURCEADDR_1, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV6SOURCEADDR_2, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV6SOURCEADDR_3, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*END*/
}

static int phd_ipv4_init(struct dwcroce_dev *dev)
{
	void __iomem *base_addr;
	base_addr = dev->devinfo->base_addr;

	/*ipv4 version*/
	writel(PHD_BASE_0 + PHDIPV4VERSION, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDIPV4HEADER_LEN, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDIPV4TOS, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDIPV4ID, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDIPV4FLAG, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDIPV4OFFSET, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDIPV4TTL, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDIPV4PROTOCOL, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDIPV4SOURCEADDR, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*END*/
	/*phd 1 ipv4 init*/
	writel(PHD_BASE_1 + PHDIPV4VERSION, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV4HEADER_LEN, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV4TOS, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV4ID, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV4FLAG, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV4OFFSET, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV4TTL, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV4PROTOCOL, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDIPV4SOURCEADDR, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*END*/

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
	writel(PHD_BASE_0 + PHDMACTYPEIPV4, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDMACTYPEIPV4, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDMACTYPEIPV6, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDMACTYPEIPV6, base_addr + MPB_WRITE_ADDR);
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
	struct xlgmac_channel *channel = dev->devinfo->channel_head;
	struct xlgmac_pdata* pdata = channel->pdata;
	int i =0;
	u32 addr_h = 0;
	u32 addr_l = 0;
	for (i = 0; i < pdata->channel_count; i++)
	{
		
		addr_h = readl(XLGMAC_DMA_REG(channel, DMA_CH_TDLR_HI));
		addr_l = readl(XLGMAC_DMA_REG(channel, DMA_CH_TDLR_LO));
		printk("addr_h = %x, addr_l = %x \n,channel count is %d", addr_h, addr_l,pdata->channel_count);//added by hs for info
		channel++;
	}
	channel = dev->devinfo->channel_head;
	addr_h = 0;
	addr_l = 0;
	addr_h = readl(XLGMAC_DMA_REG(channel, DMA_CH_TDLR_HI));
	addr_l = readl(XLGMAC_DMA_REG(channel, DMA_CH_TDLR_LO));

	/*tx_desc_tail_lptr_addr start*/
	writel(PHD_BASE_0 + PHDTXDESCTAILPTR_H, base_addr + MPB_WRITE_ADDR);
	writel(addr_h, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDTXDESCTAILPTR_L, base_addr + MPB_WRITE_ADDR);
	writel(addr_l, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDTXDESCTAILPTR_H, base_addr + MPB_WRITE_ADDR);
	writel(addr_h, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDTXDESCTAILPTR_L, base_addr + MPB_WRITE_ADDR);
	writel(addr_l, base_addr + MPB_RW_DATA);
	/*end*/
#if 0 // added by hs for debugging
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
#endif
	return 0;

}


static int dwcroce_init_phd(struct dwcroce_dev *dev)
{
	int status;
	status = phd_txdesc_init(dev);
	if (status)
		goto phdtxrxdesc_err;
#if 0
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
#endif
	return 0;
udperr:
	printk("err: udperr\n");
iperr:
	printk("err: iperr\n");
mac_err:
	printk("err: macerr\n");

phdtxrxdesc_err:
	printk("err: phd txrxdescerr!\n");//added by hs for info
	return status;
}

int dwcroce_init_hw(struct dwcroce_dev *dev)
{
	int status;
	status = dwcroce_init_phd(dev);
	if (status)
		goto errphd;
	return 0;
errphd:
	printk("phd init err!\n");//added by hs
	return status;
}

static int dwcroce_read_phd(struct dwcroce_dev *dev)
{
	u32 regval;
	void __iomem* base_addr;
	base_addr = dev->devinfo->base_addr;
	int i = 0;
	u32 phd_base_addr = 0;
	for (i = 0; i <= 1; i++) {
		phd_base_addr = i == 0 ? PHD_BASE_0: PHD_BASE_1;
		printk("------------------dwcroce read phd %d------------------\n",i);//added by hs for info
			/*read phd order*/
		writel(phd_base_addr + PHDBYTEORDER, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_byte_order: %x\n", regval);//added by hs for info 

			/*read tx desc tail ptr_l order*/
		writel(phd_base_addr + PHDTXDESCTAILPTR_L, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_tx_desc_tail_ptr_l: %x\n", regval);//added by hs for info 

			/*read tx desc tail ptr_h order*/
		writel(phd_base_addr + PHDTXDESCTAILPTR_H, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_tx_desc_tail_ptr_h: %x\n", regval);//added by hs for info 

			/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDTXDESCTAILPTR_THRESDHOLD, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_tx_desc_tail_ptr_thresdhold: %x\n", regval);//added by hs for info 

				/*read phd rx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDRXDESCTAILPTR_THRESDHOLD, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_rx_desc_tail_ptr_thresdhold: %x\n", regval);//added by hs for info 

				/*read phd tx desc tail ptr incr step order*/
		writel(phd_base_addr + PHDRXDESCTAILPTR_INCR_STEP, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_tx_desc_tail_ptr_incr_step: %x\n", regval);//added by hs for info 

				/*read phd rx desc tail ptr_l order*/
		writel(phd_base_addr + PHDRXDESCTAILPTR_L, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_rx_desc_tail_ptr_l: %x\n", regval);//added by hs for info 

				/*read phd rx desc tail ptr_h thresdhold order*/
		writel(phd_base_addr + PHDRXDESCTAILPTR_H, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_rx_desc_tail_ptr_h: %x\n", regval);//added by hs for info 

				/*read phd mac source addr_l*/
		writel(phd_base_addr + PHDMACSOURCEADDR_L, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_mac_source_addr_l: %x\n", regval);//added by hs for info 

				/*read phd mac source addr_h*/
		writel(phd_base_addr + PHDMACSOURCEADDR_H, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_mac_source_addr_h: %x\n", regval);//added by hs for info 

				/*read phd mac type ipv4*/
		writel(phd_base_addr + PHDMACTYPEIPV4, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_mac_type_ipv4: %x\n", regval);//added by hs for info 

				/*read phd mac type ipv6*/
		writel(phd_base_addr + PHDMACTYPEIPV6, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_mac_type_ipv6: %x\n", regval);//added by hs for info 

				/*read phd ipv4 version*/
		writel(phd_base_addr + PHDIPV4VERSION, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: : phd_ipv4_version: %x\n", regval);//added by hs for info 

				/*read phd ipv4 header len*/
		writel(phd_base_addr + PHDIPV4HEADER_LEN, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv4_header_len: %x\n", regval);//added by hs for info 

				/*read phd ipv4 tos */
		writel(phd_base_addr + PHDIPV4TOS, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv4_tos: %x\n", regval);//added by hs for info 

				/*read phd ipv4 id*/
		writel(phd_base_addr + PHDIPV4ID, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv4_id: %x\n", regval);//added by hs for info 

				/*read phd ipv4 flag*/
		writel(phd_base_addr + PHDIPV4FLAG, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv4_flag: %x\n", regval);//added by hs for info 

				/*read phd ipv4 offset*/
		writel(phd_base_addr + PHDIPV4OFFSET, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv4_offset: %x\n", regval);//added by hs for info 

				/*read phd ipv4 ttl*/
		writel(phd_base_addr + PHDIPV4TTL, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv4_ttl: %x\n", regval);//added by hs for info 

				/*read phd ipv4 protocol*/
		writel(phd_base_addr + PHDIPV4PROTOCOL, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv4_protocol: %x\n", regval);//added by hs for info 

				/*read phd ipv4 source addr*/
		writel(phd_base_addr + PHDIPV4SOURCEADDR, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv4_source_addr: %x\n", regval);//added by hs for info 

				/*read phd ipv6 version*/
		writel(phd_base_addr + PHDIPV6VERSION, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv6_version: %x\n", regval);//added by hs for info 

				/*read phd ipv6_class*/
		writel(phd_base_addr + PHDIPV6CLASS, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv6_class: %x\n", regval);//added by hs for info 

				/*read phd ipv6 flow label*/
		writel(phd_base_addr + PHDIPV6FLOWLABLE, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv6_flow_label: %x\n", regval);//added by hs for info 

				/*read phd ipv6 next header*/
		writel(phd_base_addr + PHDIPV6NEXTHEADER, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv6_next_header: %x\n", regval);//added by hs for info 

				/*read phd ipv6 hop limit*/
		writel(phd_base_addr + PHDIPV6HOPLIMIT, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv6_hop_limit: %x\n", regval);//added by hs for info 

				/*read phd ipv6 source addr 0*/
		writel(phd_base_addr + PHDIPV6SOURCEADDR_0, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv6_source_addr_0: %x\n", regval);//added by hs for info 

					/*read phd ipv6 source addr 1 order*/
		writel(phd_base_addr + PHDIPV6SOURCEADDR_1, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv6_source_addr_1: %x\n", regval);//added by hs for info 

					/*read phd ipv6 source addr 2*/
		writel(phd_base_addr + PHDIPV6SOURCEADDR_2, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv6_source_addr_2: %x\n", regval);//added by hs for info 

					/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDIPV6SOURCEADDR_3, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_ipv6_source_addr_3: %x\n", regval);//added by hs for info 

					/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDUDPSOURCEPORT, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_udp_source_port: %x\n", regval);//added by hs for info 

					/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDUDPDESTPORT, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_udp_dest_port: %x\n", regval);//added by hs for info 

					/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDUDPCHECKSUM, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_udp_checksum: %x\n", regval);//added by hs for info 

					/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDCONTEXT_TDES0, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_context_tdes0: %x\n", regval);//added by hs for info 

					/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDCONTEXT_TDES1, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_context_tdes1: %x\n", regval);//added by hs for info 

					/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDCONTEXT_TDES2, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_context_tdes2: %x\n", regval);//added by hs for info 

					/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDCONTEXT_TDES3, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_context_tdes3: %x\n", regval);//added by hs for info 

					/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDNORMAL_TDES1, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_normal_tdes1: %x\n", regval);//added by hs for info 

						/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDNORMAL_TDES2, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_normal_tdes2: %x\n", regval);//added by hs for info 

						/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDNORMAL_TDES3, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_normal_tdes3: %x\n", regval);//added by hs for info 

						/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDNORMAL_RDES1, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_normal_rdes1: %x\n", regval);//added by hs for info 

						/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDNORMAL_RDES2, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_normal_rdes2: %x\n", regval);//added by hs for info 

						/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDNORMAL_RDES3, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_normal_rdes3: %x\n", regval);//added by hs for info 

						/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDSRAM_RMC, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_sram_rmc: %x\n", regval);//added by hs for info 

						/*read phd tx desc tail ptr thresdhold order*/
		writel(phd_base_addr + PHDMACTYPEIPV6RECV, base_addr + MPB_WRITE_ADDR);
		regval = readl(base_addr + MPB_RW_DATA);
		printk("dwcroce: phd_mac_type_ipv6_recv: %x\n", regval);//added by hs for info 
		printk("-------------------------end of phd%d--------------------------\n",i);//added by hs 
	}
	return 0;
}
int dwcroce_get_hwinfo(struct dwcroce_dev *dev)
{
	int status;
	status = dwcroce_read_phd(dev);
	if (status)
		goto errphd;
	return 0;
errphd:
	printk("read phd failed!\n");//added by hs
	return status;
}
