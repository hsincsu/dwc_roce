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

static int phd_start(struct dwcroce_dev *dev)
{
	void __iomem* base_addr;
	base_addr = dev->devinfo.base_addr;
	writel(PHD_BASE_0 + PHDSTART, base_addr + MPB_WRITE_ADDR);
	writel(0x1, base_addr + MPB_RW_DATA);
}

static int phd_udp_init(struct dwcroce_dev *dev)
{
	void __iomem* base_addr;
	base_addr = dev->devinfo.base_addr;
	printk("dwcroce: no need to init udp\n");//added by hs
	return 0;
}


static int phd_ipv6_init(struct dwcroce_dev *dev)
{
	void __iomem* base_addr;
	base_addr = dev->devinfo.base_addr;

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
	base_addr = dev->devinfo.base_addr;
	struct net_device *netdev;
	netdev = dev->devinfo.netdev;
	__be32 addr;
	addr = netdev->ip_ptr->ifa_list->ifa_address;	
	u32 addr_k;
	addr_k = __be32_to_cpu(addr);
	printk("ipv4: %x",addr_k);//added by hs for info

        writel(PHD_BASE_0 + PHDIPV4SOURCEADDR, base_addr + MPB_WRITE_ADDR);
        writel(addr_k, base_addr + MPB_RW_DATA);

        writel(PHD_BASE_1 + PHDIPV4SOURCEADDR, base_addr + MPB_WRITE_ADDR);
        writel(addr_k, base_addr + MPB_RW_DATA);

#if 0
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
#endif
	return 0;
}

static int phd_mac_init(struct dwcroce_dev *dev)
{
	void __iomem *base_addr;
	base_addr = dev->devinfo.base_addr;
	u8 *addr;
	addr = dev->devinfo.netdev->dev_addr;	
	printk("mac addr is %x\n",addr[5]);//added by hs for info

/*  added by hs, these code is to read mac reg to get mac address
	unsigned int addh = 0;
	unsigned int addl = 0;
	addh = readl(dev->devinfo->mac_base + MAC_MACA0HR);
	addl = readl(dev->devinfo->mac_base + MAC_MACA0LR);
	printk("addh is %x, addl is %x \n",addh,addl);//added by hs 
*/
	int i =0;
	unsigned int macaddr_l =0;
	unsigned int  macaddr_h = 0;
	macaddr_h = (addr[5]<<8)|(addr[4]<<0);
	macaddr_l = (addr[3]<<24)|(addr[2]<<16)|(addr[1]<<8)|(addr[0]<<0);
	
	/*mac source addr  */
	writel(PHD_BASE_0 + PHDMACSOURCEADDR_H, base_addr + MPB_WRITE_ADDR);
	writel(macaddr_h, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDMACSOURCEADDR_L, base_addr + MPB_WRITE_ADDR);
	writel(macaddr_l, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDMACSOURCEADDR_H, base_addr + MPB_WRITE_ADDR);
	writel(macaddr_h, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDMACSOURCEADDR_L, base_addr + MPB_WRITE_ADDR);
	writel(macaddr_l, base_addr + MPB_RW_DATA);

	/*end*/
#if 0 //added by hs ,no need to init !
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
#endif
	return 0;
}

static int phd_rxdesc_init(struct dwcroce_dev *dev)
{
	void __iomem *base_addr;
	base_addr = dev->devinfo.base_addr;
	struct xlgmac_channel* channel = dev->devinfo.channel_head;
	struct xlgmac_pdata* pdata = channel->pdata;

	int i = 0;
	u32 addr_h = 0;
	u32 addr_l = 0;
	for (i = 0; i < pdata->channel_count; i++) //just for printing channel info
	{

		addr_h = readl(XLGMAC_DMA_REG(channel, DMA_CH_RDLR_HI));
		addr_l = readl(XLGMAC_DMA_REG(channel, DMA_CH_RDLR_LO));
		printk("FOR RXDESC_INIT: addr_h = %x, addr_l = %x \n,channel count is %d", addr_h, addr_l, pdata->channel_count);//added by hs for info
		channel++;
	}
	channel = dev->devinfo.channel_head;
	addr_h = 0;
	addr_l = 0;
	addr_h = readl(XLGMAC_DMA_REG(channel, DMA_CH_RDLR_HI));
	addr_l = readl(XLGMAC_DMA_REG(channel, DMA_CH_RDLR_LO));


	/*rx_desc_tail_lptr_addr start*/
	writel(PHD_BASE_0 + PHDRXDESCTAILPTR_H, base_addr + MPB_WRITE_ADDR);
	writel(addr_h, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_0 + PHDRXDESCTAILPTR_L, base_addr + MPB_WRITE_ADDR);
	writel(addr_l, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDRXDESCTAILPTR_H, base_addr + MPB_WRITE_ADDR);
	writel(addr_h, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDRXDESCTAILPTR_L, base_addr + MPB_WRITE_ADDR);
	writel(addr_l, base_addr + MPB_RW_DATA);
	/*end*/
#if 0 //added by hs , no need to init this
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
#endif
	return 0;
}
static int phd_txdesc_init(struct dwcroce_dev *dev)
{
	/*对Phd的发送描述符进行初始化*/
	void __iomem *base_addr;
	base_addr = dev->devinfo.base_addr;
	struct xlgmac_channel *channel = dev->devinfo.channel_head;
	struct xlgmac_pdata* pdata = channel->pdata;
	int i =0;
	u32 addr_h = 0;
	u32 addr_l = 0;
	for (i = 0; i < pdata->channel_count; i++) //just for printing channel info
	{
		
		addr_h = readl(XLGMAC_DMA_REG(channel, DMA_CH_TDLR_HI));
		addr_l = readl(XLGMAC_DMA_REG(channel, DMA_CH_TDLR_LO));
		printk("addr_h = %x, addr_l = %x \n,channel count is %d", addr_h, addr_l,pdata->channel_count);//added by hs for info
		channel++;
	}
	channel = dev->devinfo.channel_head;
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
#if 0 //no need to init this ,added by hs
	/*tx_desc_tail_ptr_thresdhold*/
	writel(PHD_BASE_0 + PHDTXDESCTAILPTR_THRESDHOLD, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	writel(PHD_BASE_1 + PHDTXDESCTAILPTR_THRESDHOLD, base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);
	/*end*/
#endif

#if 0 // no need to init all this now.added by hs 
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
	status = phd_rxdesc_init(dev);
	if (status)
		goto phdtxrxdesc_err;

	status = phd_mac_init(dev);
	if (status)
		goto mac_err;
	status = phd_ipv4_init(dev);
	if (status)
		goto iperr;
#if 0 // added by hs for debugging,now there is no need to init follow function.
	status = phd_ipv6_init(dev);
	if (status)
		goto iperr;
	status = phd_udp_init(dev);
	if (status)
		goto udperr;
#endif
	status = phd_start(dev);
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
	return status;
}

static int dwcroce_init_cm(struct dwcroce_dev *dev)
{
	printk("cm  init!\n");//added by hs 
	void __iomem *base_addr;
	base_addr = dev->devinfo.base_addr;

	/*write cmcfg*/
	writel(CM_CFG + CMLOGEN,base_addr + MPB_WRITE_ADDR);
	writel(0x7,base_addr + MPB_RW_DATA);

	writel(CM_CFG + CMERREN,base_addr + MPB_WRITE_ADDR);
	writel(0x7,base_addr + MPB_RW_DATA);

	writel(CM_CFG + CMINTEN,base_addr + MPB_WRITE_ADDR);
	writel(0x7,base_addr + MPB_RW_DATA);
	return 0;
}

static int dwcroce_init_dev_attr(struct dwcroce_dev *dev)
{
	printk("dwcroce: dwcroce_init_dev_attr \n");//added by hs 
	int err = 0;
	err = dwcroce_query_device(&dev->ibdev,&dev->attr,NULL);
	if(err)
		goto err1;
	printk("dwcroce: dwcroce_init_dev_attr \n");//added by hs 
	return 0;
err1:
	printk("query device failed\n");//added by hs 
	return err;
}

void dwcroce_init_tlb(void __iomem *base_addr)
{
	u32 busy = 0x1;
	printk("dwcroce: dwcroce_init_tlb start\n");//added by hs 

	while (busy & 0x00000001)//only need the first bit in busy.
	{
		printk("dwcroce: busy cycle \n");//added by hs 
		writel(PGU_BASE + TLBINIT,base_addr + MPB_WRITE_ADDR);
		busy = readl(base_addr + MPB_RW_DATA);
	}
	printk("dwcroce: dwcroce_init_tlb end\n");//added by hs 
}


static int dwcroce_init_pgu_wqe(struct dwcroce_dev *dev)
{
	printk("dwcroce: dwcroce_init_PGU_wqe  start\n");//added by hs
	/*PGU INIT*/
	int err = 0;
	int i = 0;
	u32 count = 0;
	void __iomem *base_addr;
	base_addr = dev->devinfo.base_addr;

	count = 1ull << QPNUM; // 1024.
	count = count -1;
	printk("dwcroce:WQE INIT, count : %d \n",count);//added by hs
	/*socket id*/
	writel(PGU_BASE + SOCKETID,base_addr + MPB_WRITE_ADDR);
	writel(0x0,base_addr + MPB_RW_DATA);//should be MAC Address,but there is only 32bits.
	/*TLB INIT*/
	dwcroce_init_tlb(base_addr); 

	/*init each WQEQueue entry*/
	for (i = 0; i <= count; i = i + 1)
	{
		writel(PGU_BASE + QPLISTREADQPN,base_addr + MPB_WRITE_ADDR);
		writel(i,base_addr + MPB_RW_DATA);
		/*wirtel qp list 0 - 5 start*/
		writel(PGU_BASE + WPFORQPLIST,base_addr + MPB_WRITE_ADDR); // WP virtual
		writel(0x0,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + WPFORQPLIST2,base_addr + MPB_WRITE_ADDR);//WP virtual
		writel(0x0,base_addr + MPB_RW_DATA);
	
		writel(PGU_BASE + RPFORQPLIST,base_addr + MPB_WRITE_ADDR);//RP virtual
		writel(0x0,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + RPFORQPLIST2,base_addr + MPB_WRITE_ADDR);//RP virtual
		writel(0x0,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + QPNANDVALID,base_addr + MPB_WRITE_ADDR);//valid + phypage
		writel(0x0,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + QPNANDVALID2,base_addr + MPB_WRITE_ADDR);//valid + phypage
		writel(0x0,base_addr + MPB_RW_DATA);
		/*write qp list 0 - 5 end*/
		writel(PGU_BASE + WRITEORREADQPLIST,base_addr + MPB_WRITE_ADDR);
		writel(0x1,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + WRITEQPLISTMASK,base_addr + MPB_WRITE_ADDR);
		writel(0x7,base_addr + MPB_RW_DATA); // WRITEL 3'b111

		writel(PGU_BASE + QPLISTWRITEQPN,base_addr + MPB_WRITE_ADDR);
		writel(0x1,base_addr + MPB_RW_DATA); // write qp list 4020  1

		writel(PGU_BASE + WRITEORREADQPLIST,base_addr + MPB_WRITE_ADDR);
		writel(0x0,base_addr + MPB_RW_DATA);
	}
	printk("dwcroce: dwcroce_init_PGU_wqe end\n");//added by hs
	return err;
}

static int dwcroce_init_pgu_cq(struct dwcroce_dev *dev)
{
	printk("dwcroce: dwcroce_init_pgu_cq start \n");//added by hs 
	int err =0;
	int i =0;
	void __iomem *base_addr;
	base_addr = dev->devinfo.base_addr;
	u32 txop = 0;
	u32 rxop = 0;
	u32 xmitop = 0;
	u32 count = 0;

	count = 1ull << QPNUM;
	count = count -1;
	printk("dwcroce:CQ INIT,count: %d \n",count);
	printk("init tx cq start \n");//added by hs 
	for (i = 0; i <= count; i = i + 1) // init tx cq
	{
		txop = 0x0;
		txop = i<<2; // txop = {{(32-QPNUM-2){'b0}},i['QPNUM-1:0],1'b1,1'b1};
//		printk("txop is %x\n",txop);//added by hs 
		txop = txop + 0x3;
//		printk("txop is %x\n",txop);//added by hs 
		writel(PGU_BASE + CQQUEUEUP,base_addr + MPB_WRITE_ADDR);
		writel(0x2000,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + CQQUEUEUP + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(0x0000,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + CQQUEUEDOWN,base_addr + MPB_WRITE_ADDR);
		writel(0x0000,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + CQQUEUEDOWN + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(0x0000,base_addr + MPB_RW_DATA);
		
		writel(PGU_BASE + CQREADPTR,base_addr + MPB_WRITE_ADDR);
		writel(0x0,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + CQREADPTR + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(0x0,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + CQESIZE,base_addr + MPB_WRITE_ADDR);
		writel(txop,base_addr + MPB_RW_DATA);
	//	printk("dwcroce: tx cq end \n");//added by hs 
		while (txop & 0x00000001) //QPNUM = 10,SO 32 -10 -2 = 20
		{
	//		printk("dwcroce: txcqcycle \n");//added by hs 
			writel(PGU_BASE + CQESIZE,base_addr + MPB_WRITE_ADDR);
			txop = readl(base_addr + MPB_RW_DATA);
		}
			
	}

	printk("dwcroce: rx cq start \n");//added by hs 
	for (i = 0; i <= count; i = i + 1) // init rx cq
	{
		rxop = 0;
		rxop = i << 2; // the same to upper one
		rxop = rxop + 0x3;
	//	printk("rxop is %x \n",rxop);//added by hs 
		writel(PGU_BASE + RxUpAddrCQE,base_addr + MPB_WRITE_ADDR);
		writel(0x2000,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + RxUpAddrCQE + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(0x0000,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + RxBaseAddrCQE,base_addr + MPB_WRITE_ADDR);
		writel(0x0000,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + RxBaseAddrCQE + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(0x0000,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + RxCQEWP,base_addr + MPB_WRITE_ADDR);
		writel(0x0000,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + RxCQEWP + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(0x0000,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + RxCQEOp,base_addr + MPB_WRITE_ADDR);
		writel(rxop,base_addr + MPB_RW_DATA);
	//	printk("dwcroce: rx cq end \n");//added by hs 
		while(rxop & 0x00000001)
		{
			//rxop = 0;
	//		printk("dwcroce: rx cq cycle \n");//added by hs 
			writel(PGU_BASE + RxCQEOp,base_addr + MPB_WRITE_ADDR);
			rxop = readl(base_addr + MPB_RW_DATA);
		//	if(rxop)
		//		printk("rxop cycle is %x\n",rxop);//added by hs 
		}

	}

	printk("dwcroce: xmit cq start \n");//added by hs 
	for (i = 0; i <= count; i = i + 1)
	{
		xmitop =0;
		xmitop = i<<2; // the same to uppper one
		xmitop = xmitop + 0x3;
	//	printk("dwcroce: xmitop is %x\n",xmitop);//added by hs 
		writel(PGU_BASE + XmitUpAddrCQE,base_addr + MPB_WRITE_ADDR);
		writel(0x2000,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + XmitUpAddrCQE + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(0x0000,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + XmitBaseAddrCQE,base_addr + MPB_WRITE_ADDR);
		writel(0x0000,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + XmitBaseAddrCQE + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(0x0000,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + XmitCQEWP,base_addr + MPB_WRITE_ADDR);
		writel(0x0000,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + XmitCQEWP + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(0x0000,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + XmitCQEOp,base_addr + MPB_WRITE_ADDR);
		writel(xmitop,base_addr + MPB_RW_DATA);

		while (xmitop & 0x00000001)
		{
		//	printk("dwcroce: xmitop cycle \n");//added by hs 
			//xmitop = 0;
			writel(PGU_BASE + XmitCQEOp,base_addr + MPB_WRITE_ADDR);
			xmitop = readl(base_addr + MPB_RW_DATA);
	//		printk("dwcroce: xmitop cycle  is %x\n",xmitop);//added by hs 
		}
	}

	/*init wqe retrycount and timeout*/
	writel(PGU_BASE + WQERETRYCOUNT,base_addr + MPB_WRITE_ADDR);
	writel(0xffffffff,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + WQERETRYTIMER,base_addr + MPB_WRITE_ADDR);
	writel(0xffffffff,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + WQERETRYTIMER + 0x4,base_addr + MPB_WRITE_ADDR);
	writel(0xffffffff,base_addr + MPB_RW_DATA);

	printk("dwcroce: dwcroce_init_pgu_cq end \n");//added by hs
	return err;
	 
}

static int dwcroce_init_qp(struct dwcroce_dev *dev)
{
	int err;
	void __iomem *base_addr;
	base_addr = dev->devinfo.base_addr;
	/*init psn*/
	writel(PGU_BASE + STARTINITPSN,base_addr + MPB_WRITE_ADDR); // INIT PSN
	writel(0x0000,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + STARTINITPSN + 0x4,base_addr + MPB_WRITE_ADDR);
	writel(0x0000,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + STARTINITPSN + 0x8,base_addr + MPB_WRITE_ADDR);
	writel(0x0000,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + STARTINITPSN + 0xc,base_addr + MPB_WRITE_ADDR);
	writel(0x10000,base_addr + MPB_RW_DATA);
	
/*for some reason,need init these registers*/
	writel(PGU_BASE + GENRSP,base_addr + MPB_WRITE_ADDR);
        writel(0x0,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + CFGRNR,base_addr + MPB_WRITE_ADDR);
        writel(0x0,base_addr + MPB_RW_DATA);

}

int dwcroce_init_hw(struct dwcroce_dev *dev)
{
	int status;
	status = dwcroce_init_phd(dev);
	if (status)
		goto errphd;
	status = dwcroce_init_cm(dev);
	if (status)
		goto errcm;
	status = dwcroce_init_pgu_wqe(dev);
	if (status)
		goto errcm;
	status = dwcroce_init_pgu_cq(dev);
	if (status)
		goto errcm;
	status = dwcroce_init_qp(dev);
	if (status)
		goto errcm;
	status = dwcroce_init_dev_attr(dev);
	if(status)
		goto errcm;
	return 0;
errcm:
	printk("cm init err!\n");//added by hs
errphd:
	printk("phd init err!\n");//added by hs
	return status;
}

static int dwcroce_read_phd(struct dwcroce_dev *dev)
{
	u32 regval;
	void __iomem* base_addr;
	base_addr = dev->devinfo.base_addr;
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

int dwcroce_hw_create_cq(struct dwcroce_dev *dev, struct dwcroce_cq *cq, int entries, u16 pd_id)
{
	printk("dwcroce: dwcroce_hw_create_cq start\n");//added by hs 
	int max_hw_cqe;
	u32 hw_pages,cqe_size,cqe_count;
	struct pci_dev *pdev = dev->devinfo.pcidev;
	int status;
	/*For kernel*/
	cq->max_hw_cqe= dev->attr.max_cqe;
	max_hw_cqe = dev->attr.max_cqe;
	cqe_size = sizeof(struct dwcroce_cqe);
	
	cq->len = roundup(max_hw_cqe*cqe_size,DWCROCE_MIN_Q_PAGE_SIZE);
	/*tx cq*/
	cq->txva = dma_alloc_coherent(&pdev->dev,cq->len,&cq->txpa,GFP_KERNEL); // allocate memory for tx cq
	if (!cq->txva) {
		status = -ENOMEM;
		goto mem_err;
	}
	cq->txwp = cq->txrp = 0; // because wp,rp means the offset of the phypage, shoule be 0 at first.
	/*rx cq*/
	cq->rxva = dma_alloc_coherent(&pdev->dev,cq->len,&cq->rxpa,GFP_KERNEL);//allocate memory for rx cq
	if (!cq->rxva) {
		status = -ENOMEM;
		goto mem_err;
	}
	cq->rxwp = cq->rxrp = 0;
	/*xmit cq*/
	cq->xmitva = dma_alloc_coherent(&pdev->dev,cq->len,&cq->xmitpa,GFP_KERNEL);//allocate memory for xmit cq
	if (!cq->xmitva) {
		status = -ENOMEM;
		goto mem_err;
	}
	cq->xmitwp = cq->xmitrp = 0;
	cqe_count = cq->len / cqe_size;
	cq->cqe_cnt = cqe_count;
	if(cqe_count > 256)
		printk("dwcroce: cqe_count over 256\n");//added by hs 
	
	printk("dwcroce: dwcroce_hw_create_cq end\n");//added by hs 
	return 0;
mem_err:
	printk("dwcroce:mem_err\n");//added by hs
	return 0;
}

int dwcroce_alloc_cqqpresource(struct dwcroce_dev *dev, unsigned long *resource_array, u32 max_resources, u32 *req_resource_num, u32 *next)
{
	u32 resource_num;
	unsigned long flags;
	spin_lock_irqsave(&dev->resource_lock,flags);
	resource_num = find_next_zero_bit(resource_array,max_resources,*next);
	if (resource_num >= max_resources) {
		resource_num = find_first_zero_bit(resource_array,max_resources);
		if (resource_num >= max_resources) {
			spin_unlock_irqrestore(&dev->resource_lock,flags);
			return -EOVERFLOW;
		}
	}
	set_bit(resource_num,resource_array);
	*next = resource_num + 1;
	if(*next == max_resources)
		*next = 0;
	*req_resource_num = resource_num;
	spin_unlock_irqrestore(&dev->resource_lock,flags);
	return 0;
}

/*allocate memory , create qp & cq in hw*/
int dwcroce_hw_create_qp(struct dwcroce_dev *dev, struct dwcroce_qp *qp, struct dwcroce_cq *cq, struct dwcroce_pd *pd , struct ib_qp_init_attr *attrs)
{
	printk("dwcroce: dwcroce_hw_create_qp start \n");//added by hs
	int status;
	struct pci_dev *pdev = dev->devinfo.pcidev;
	u32 len;
	dma_addr_t pa = 0;
	/*For rq*/
	u32 max_rqe_allocated = attrs->cap.max_recv_wr + 1;
	max_rqe_allocated = min_t(u32,attrs->cap.max_recv_wr +1,dev->attr.max_qp_wr); // to sure the rqe num is under 256.
	qp->rq.max_cnt = max_rqe_allocated;
	len = sizeof(struct dwcroce_rqe) * max_rqe_allocated;
	qp->rq.max_cnt= max_rqe_allocated;
	qp->rq.max_wqe_idx= max_rqe_allocated - 1;
	qp->rq.va = dma_alloc_coherent(&pdev->dev,len,&pa,GFP_KERNEL); // allocate memory for rq.
	if(!qp->rq.va)
		return -EINVAL;
	qp->rq.len = len;
	qp->rq.pa = pa;
	qp->rq.entry_size = sizeof(struct dwcroce_rqe);
	u32 pa_l = 0;
	u32 pa_h = 0;
	/*init pa ,len*/
	pa = 0;
	len = 0;
	/*For sq*/
	u32 max_wqe_allocated;
	u32 max_sges = attrs->cap.max_send_sge;
	max_wqe_allocated = min_t(u32,attrs->cap.max_send_wr +1,dev->attr.max_qp_wr);
	max_sges = min_t(u32,max_wqe_allocated,max_sges); // For a sge need a wqe, so sglist 'lenghth can't over wqe 's mounts.
	len = sizeof(struct dwcroce_wqe) * max_wqe_allocated;
	qp->sq.max_cnt= max_wqe_allocated;
	qp->sq.max_wqe_idx = max_wqe_allocated -1;
	qp->sq.va = dma_alloc_coherent(&pdev->dev,len,&pa,GFP_KERNEL);
	if(!qp->sq.va)
		return -EINVAL;
	qp->sq.len = len;
	qp->sq.pa = pa;
	qp->sq.entry_size = sizeof(struct dwcroce_wqe);

	printk("dwcroce:----------------Create QP checking ---------------\n");//added by hs
	printk("dwcroce:SQ va:0x%x , pa 0x%x , len:%d \n",qp->sq.va,qp->sq.pa,qp->sq.len);
	printk("dwcroce:RQ va:0x%x , pa 0x%x , len:%d \n",qp->rq.va,qp->rq.pa,qp->sq.len);//added by hs
																		   /*ACCESS HardWare register*/
	u32 qpn = qp->id;
	printk("dwcroce:QPN:%d \n",qp->id);//added by hs
	void __iomem* base_addr;
	base_addr = dev->devinfo.base_addr;
	/*init psn*/
	writel(PGU_BASE + STARTINITPSN,base_addr + MPB_WRITE_ADDR); // INIT PSN
	writel(0x0000,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + STARTINITPSN + 0x4,base_addr + MPB_WRITE_ADDR);
	writel(0x0000,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + STARTINITPSN + 0x8,base_addr + MPB_WRITE_ADDR);
	writel(0x0000,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + STARTINITPSN + 0xc,base_addr + MPB_WRITE_ADDR);
	writel(0x10000,base_addr + MPB_RW_DATA);
	/*init qpn*/
	writel(PGU_BASE + INITQP,base_addr + MPB_WRITE_ADDR);
	writel(qpn,base_addr + MPB_RW_DATA);

	/*set psn*/
	writel(PGU_BASE + INITQPTABLE,base_addr + MPB_WRITE_ADDR);
	writel(0x1,base_addr + MPB_RW_DATA);

	/*writel receive queue START*/
	/*RECVQ DIL*/
	pa = qp->rq.pa;
	printk("dwcroce: create_qp pa_a is %0llx\n",pa);//added by hs
	pa = pa >> 12;
	pa_l = pa; // rigth move 12 bits
	pa_h = pa >> 32;
	printk("dwcroce: create_qp pa is %0llx\n",pa);//added by hs
	printk("dwcroce: create_qp pa_l is %0lx\n",pa_l);//added by hs
	printk("dwcroce: create_qp pa_h is %0lx\n",pa_h);//added by hs 
	writel(PGU_BASE + RCVQ_INF,base_addr + MPB_WRITE_ADDR);
	writel(qpn,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + RCVQ_DI,base_addr + MPB_WRITE_ADDR); //write rq base addr to revq
	writel(pa_l , base_addr + MPB_RW_DATA);
	/*RECVQ DIH*/
	writel(PGU_BASE + RCVQ_DI + 0x4,base_addr + MPB_WRITE_ADDR);
	writel(pa_h, base_addr + MPB_RW_DATA);

	/*Write RCVQ_WR*/
	writel(PGU_BASE + RCVQ_WRRD,base_addr + MPB_WRITE_ADDR);
	writel(0x1, base_addr + MPB_RW_DATA);//means base addr is written.
	/*writel receive queue END*/
	/*write wp for recevice queue*/
	writel(PGU_BASE + RCVQ_INF,base_addr + MPB_WRITE_ADDR);
	writel(qpn,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + RCVQ_DI,base_addr + MPB_WRITE_ADDR); //write rq base addr to revq
	writel(0x0 , base_addr + MPB_RW_DATA);
	/*RECVQ DIH*/
	writel(PGU_BASE + RCVQ_DI + 0x4,base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	/*Write RCVQ_WR*/
	writel(PGU_BASE + RCVQ_WRRD,base_addr + MPB_WRITE_ADDR);
	writel(0x2, base_addr + MPB_RW_DATA);//means wp is written.
	/*writel receive queue for wp end*/
	/*writel receive queue for wp start*/
	writel(PGU_BASE + RCVQ_INF,base_addr + MPB_WRITE_ADDR);
	writel(qpn,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + RCVQ_DI,base_addr + MPB_WRITE_ADDR); //write rq base addr to revq
	writel(0x0, base_addr + MPB_RW_DATA);
	/*RECVQ DIH*/
	writel(PGU_BASE + RCVQ_DI + 0x4,base_addr + MPB_WRITE_ADDR);
	writel(0x0, base_addr + MPB_RW_DATA);

	/*Write RCVQ_WR*/
	writel(PGU_BASE + RCVQ_WRRD,base_addr + MPB_WRITE_ADDR);
	writel(0x4, base_addr + MPB_RW_DATA);//means wp for readding is written.
	/*writel receive queue for wp end*/
	
	pa = 0;
	pa = qp->sq.pa;
	printk("dwcroce: create_qp sqpa_a is %0llx\n",pa);//added by hs
	pa = pa >>12;
	pa_l = pa;//SendQAddr[43:12]
	pa_h = pa >> 32 + 0x10000; // {1'b1,SendQAddr[63:44]}
	printk("dwcroce: create_qp sqpa is %0llx\n",pa);//added by hs
	printk("dwcroce: create_qp sqpa_l is %0lx\n",pa_l);//added by hs
	printk("dwcroce: create_qp sqpa_h is %0lx\n",pa_h);//added by hs 
	/*writel send queue START*/
	writel(PGU_BASE + QPLISTREADQPN,base_addr + MPB_WRITE_ADDR);
	writel(qpn,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + WPFORQPLIST,base_addr + MPB_WRITE_ADDR); //WP,RP should be zero in the page.
	writel(0x0,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + WPFORQPLIST2,base_addr + MPB_WRITE_ADDR);
	writel(0x0,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + RPFORQPLIST,base_addr + MPB_WRITE_ADDR);
	writel(pa_l,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + RPFORQPLIST2,base_addr + MPB_WRITE_ADDR);
	writel(pa_h,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + WRITEORREADQPLIST,base_addr + MPB_WRITE_ADDR);
	writel(0x1,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + WRITEQPLISTMASK,base_addr + MPB_WRITE_ADDR);
	writel(0x7,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + QPLISTWRITEQPN,base_addr + MPB_WRITE_ADDR);
	writel(0x1,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + CFGSIZEOFWRENTRY,base_addr + MPB_WRITE_ADDR);
	writel(64,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + CFGSIZEOFWRENTRY + 0x4,base_addr + MPB_WRITE_ADDR);
	writel(0x0,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + WRITEORREADQPLIST,base_addr + MPB_WRITE_ADDR);
	writel(0x0,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + UPLINKDOWNLINK,base_addr + MPB_WRITE_ADDR);
	writel(0x00080100,base_addr + MPB_RW_DATA);//LINKMTU {4'h0,14'h20,14'h100}
	/*sq write end*/

	/*hw access for cq*/
	u32 txop;
	u32 rxop;
	u32 xmitop;
	len = 0;
	len = cq->len;
	pa = 0;
	pa = cq->txpa;
	pa_l = pa;
	pa_h = pa >> 32;
	printk("dwcroce: create_qp txcqpa is %0llx\n",pa);//added by hs
	printk("dwcroce: create_qp txcqpa_l is %0lx\n",pa_l);//added by hs
	printk("dwcroce: create_qp txcqpa_h is %0lx\n",pa_h);//added by hs 
	/*1. writel TXCQ,because CQ need qpn,that's why we access hw here.for getting qpn.*/
	    txop = qpn<<2;
	    txop = txop + 0x3; // txop should be {{32-'QPNUM-2){1'b0}},LQP,1'b1,1'b1};
		writel(PGU_BASE + CQQUEUEUP,base_addr + MPB_WRITE_ADDR);
		writel(pa_l + len,base_addr + MPB_RW_DATA); // upaddr  = baseaddr + len //len is the length of cq memory.

		writel(PGU_BASE + CQQUEUEUP + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(pa_h,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + CQQUEUEDOWN,base_addr + MPB_WRITE_ADDR);
		writel(pa_l,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + CQQUEUEDOWN + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(pa_h,base_addr + MPB_RW_DATA);
		
		writel(PGU_BASE + CQREADPTR,base_addr + MPB_WRITE_ADDR);
		writel(pa_l,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + CQREADPTR + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(pa_h,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + CQESIZE,base_addr + MPB_WRITE_ADDR);
		writel(txop,base_addr + MPB_RW_DATA);

	/*2.write RXCQ.*/
		pa = 0;
		pa = cq->rxpa;
		pa_l = pa;
		pa_h = pa >> 32;
		rxop = qpn << 2;
		rxop = rxop + 0x3;
		printk("dwcroce: create_qp rxcqpa is %0llx\n",pa);//added by hs
		printk("dwcroce: create_qp rxcqpa_l is %0lx\n",pa_l);//added by hs
		printk("dwcroce: create_qp rxcqpa_h is %0lx\n",pa_h);//added by hs 
		writel(PGU_BASE + RxUpAddrCQE,base_addr + MPB_WRITE_ADDR);
		writel(pa_l + len,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + RxUpAddrCQE + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(pa_h,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + RxBaseAddrCQE,base_addr + MPB_WRITE_ADDR);
		writel(pa_l,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + RxBaseAddrCQE + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(pa_h,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + RxCQEWP,base_addr + MPB_WRITE_ADDR);
		writel(pa_l,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + RxCQEWP + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(pa_h,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + RxCQEOp,base_addr + MPB_WRITE_ADDR);
		writel(rxop,base_addr + MPB_RW_DATA);

	/*3. write XMIT CQ.*/
		pa = 0;
		pa = cq->xmitpa;
		pa_l = pa;
		pa_h = pa >> 32;
		xmitop = txop;
		printk("dwcroce: create_qp xmitcqpa is %0llx\n",pa);//added by hs
		printk("dwcroce: create_qp xmitcqpa_l is %0lx\n",pa_l);//added by hs
		printk("dwcroce: create_qp xmitcqpa_h is %0lx\n",pa_h);//added by hs 
		writel(PGU_BASE + XmitUpAddrCQE,base_addr + MPB_WRITE_ADDR);
		writel(pa_l + len,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + XmitUpAddrCQE + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(pa_h,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + XmitBaseAddrCQE,base_addr + MPB_WRITE_ADDR);
		writel(pa_l,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + XmitBaseAddrCQE + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(pa_h,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + XmitCQEWP,base_addr + MPB_WRITE_ADDR);
		writel(pa_l,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + XmitCQEWP + 0x4,base_addr + MPB_WRITE_ADDR);
		writel(pa_h,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + XmitCQEOp,base_addr + MPB_WRITE_ADDR);
		writel(xmitop,base_addr + MPB_RW_DATA);
	/*hw access for cq end*/
	printk("dwcroce: dwcroce_hw_create_qp end \n");//added by hs 
	return 0;
}


enum ib_qp_state get_ibqp_state(enum dwcroce_qp_state qps) {
	 switch (qps) {
        case DWCROCE_QPS_RST:
                return IB_QPS_RESET;
        case DWCROCE_QPS_INIT:
                return IB_QPS_INIT;
        case DWCROCE_QPS_RTR:
                return IB_QPS_RTR;
        case DWCROCE_QPS_RTS:
                return IB_QPS_RTS;
        case DWCROCE_QPS_SQD:
        case DWCROCE_QPS_SQ_DRAINING:
                return IB_QPS_SQD;
        case DWCROCE_QPS_SQE:
                return IB_QPS_SQE;
        case DWCROCE_QPS_ERR:
                return IB_QPS_ERR;
        }
        return IB_QPS_ERR;


}

enum dwcroce_qp_state get_dwcroce_qp_state(enum ib_qp_state qps) {
	switch (qps) {
        case IB_QPS_RESET:
                return DWCROCE_QPS_RST;
        case IB_QPS_INIT:
                return DWCROCE_QPS_INIT;
        case IB_QPS_RTR:
                return DWCROCE_QPS_RTR; 
        case IB_QPS_RTS:
                return DWCROCE_QPS_RTS;
        case IB_QPS_SQD:
                return DWCROCE_QPS_SQD;
        case IB_QPS_SQE:
                return DWCROCE_QPS_SQE;
        case IB_QPS_ERR:
                return DWCROCE_QPS_ERR;
        }
        return DWCROCE_QPS_ERR;

}

static int dwcroce_resolve_dmac(struct dwcroce_dev *dev, struct rdma_ah_attr *ah_attr, u8 *mac_addr)
{
	struct in6_addr in6;
	memcpy(&in6,rdma_ah_read_grh(ah_attr)->dgid.raw,sizeof(in6));
	if(rdma_is_multicast_addr(&in6))
		rdma_get_mcast_mac(&in6,mac_addr);
	else if (rdma_link_local_addr(&in6))
		rdma_get_ll_mac(&in6,mac_addr);
	else
		memcpy(mac_addr,ah_attr->roce.dmac,ETH_ALEN);
	return 0;
}

static int dwcroce_set_av_params(struct dwcroce_qp *qp, struct ib_qp_attr *attrs, int attr_mask)
{
	int status;
	struct rdma_ah_attr *ah_attr = &attrs->ah_attr;
	const struct ib_gid_attr *sgid_attr;
	u32 vlan_id =0xFFFF;
	union {
		struct sockaddr		_sockaddr;
		struct sockaddr_in	_sockaddr_in;
		struct sockaddr_in6 _sockaddr_in6;
	}sgid_addr,dgid_addr;
	struct dwcroce_dev *dev = get_dwcroce_dev(qp->ibqp.device);
	const struct ib_global_route *grh;
	if((rdma_ah_get_ah_flags(ah_attr)& IB_AH_GRH) == 0)
				return -EINVAL;
	grh = rdma_ah_read_grh(ah_attr);
	sgid_attr = ah_attr->grh.sgid_attr;
	vlan_id = rdma_vlan_dev_vlan_id(sgid_attr->ndev);
	memcpy(qp->mac_addr,sgid_attr->ndev->dev_addr,ETH_ALEN);

	qp->sgid_idx = grh->sgid_index;
	status = dwcroce_resolve_dmac(dev,ah_attr,&qp->mac_addr[0]);
	return status;
}

 int dwcroce_qp_state_change(struct dwcroce_qp *qp, enum ib_qp_state new_ib_state, enum ib_qp_state *old_ib_state) {
	unsigned long flags;
	enum dwcroce_qp_state new_state;
	new_state = get_dwcroce_qp_state(new_ib_state);
	printk("dwcroce:%s start \n",__func__);//added by hs
	if(old_ib_state)
		*old_ib_state = get_ibqp_state(qp->qp_state);
	if (new_state == qp->qp_state) {
		return 1;
	}
	if (new_state == DWCROCE_QPS_INIT) {
		printk("dwcroce: modify_qp INIT_STATE \n");//added by hs 
		qp->sq.head= 0;
		qp->sq.tail= 0;
		qp->rq.head= 0;
		qp->rq.tail= 0;
		qp->sq.qp_foe = DWCROCE_Q_EMPTY;
		qp->rq.qp_foe = DWCROCE_Q_EMPTY;
	}
	else if (new_state == DWCROCE_QPS_ERR) {
		printk("dwcroce: modify_qp ERR_STATE \n");//added by hs 
	
	}
	qp->qp_state = new_state;
	printk("dwcroce:%s end\n",__func__);//added by hs
	return 0;
}


int dwcroce_set_qp_params(struct dwcroce_qp *qp, struct ib_qp_attr *attrs, int attr_mask) {
	int status = 0;
	struct dwcroce_dev *dev;
	dev = get_dwcroce_dev(qp->ibqp.device);
	printk("dwcroce:%s start \n",__func__);//added by hs
	if (attr_mask & IB_QP_PKEY_INDEX) {
		qp->pkey_index = attrs->pkey_index;
	}
	if (attr_mask & IB_QP_QKEY) {
		qp->qkey = attrs->qkey;
	}
	if (attr_mask & IB_QP_DEST_QPN) { // get dest qpn.
		qp->destqp = attrs->dest_qp_num;
	}
	if (attr_mask & IB_QP_AV) {
		status = dwcroce_set_av_params(qp,attrs,attr_mask);
	}
	else if (qp->qp_type == IB_QPT_GSI || qp->qp_type == IB_QPT_UD)
	{
		memcpy(qp->mac_addr,dev->devinfo.netdev->dev_addr,ETH_ALEN);
		//GET LOCAL MAC
	}
	if (attr_mask & IB_QP_PATH_MTU) {
		if (attrs->path_mtu < IB_MTU_512 ||
			attrs->path_mtu > IB_MTU_4096) {
			printk("dwcroce: not supported \n");//added by hs
			status = -EINVAL;
			}
	}
	if (attr_mask & IB_QP_TIMEOUT) {
		  printk("dwcroce:timeout: %x \n",attrs->timeout);//added by hs
	}
	if (attr_mask & IB_QP_RETRY_CNT) {
		  printk("dwcroce:retry_cnt: %x \n",attrs->retry_cnt);//added by hs
	}
	if (attr_mask & IB_QP_MIN_RNR_TIMER) {
		  printk("dwcroce:min_rnr_timer: %x \n",attrs->min_rnr_timer);//added by hs
	}
	if (attr_mask & IB_QP_RNR_RETRY) {
		printk("dwcroce:rnr_retry: %x \n",attrs->rnr_retry);//added by hs
	}
	if (attr_mask & IB_QP_SQ_PSN) {
		printk("dwcroce:sq_psn: %x \n",attrs->sq_psn);//added by hs
	}
	if (attr_mask & IB_QP_RQ_PSN) {
		printk("dwcroce:rq_psn: %x \n",attrs->rq_psn);//added by hs
	}
	if (attr_mask & IB_QP_MAX_QP_RD_ATOMIC) {
		printk("dwcroce:max_qp_rd_atomic: %x \n",attrs->max_rd_atomic);//added by hs
	}
	if (attr_mask & IB_QP_MAX_DEST_RD_ATOMIC) {
		printk("dwcroce:max_dest_rd_atomic: %x \n",attrs->max_dest_rd_atomic);//added by hs
	}
	
	printk("dwcroce:%s end \n",__func__);//added by hs
	return status;

}
