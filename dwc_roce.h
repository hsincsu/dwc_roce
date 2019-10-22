/* *
 *  *
 *   *
 *    *
 *     *      This is a header file for dwc driver.It connect dwc driver and dwcroce driver
 *      *
 *       *
 *        *                                                                      ---------------edited by hs in 2019/9/18
 *         *
 *          *
 *           *
 * * * * * *  */

#ifndef DWC_ROCE_H
#define DWC_ROCE_H

#include<linux/pci.h>
#include<linux/netdevice.h>
#include<linux/phy.h>
struct dwcroce_dev;

struct dwc_dev_info {
       // u8 mac_addr[ETH_ALEN];
        struct device *dev;
        struct net_device *netdev;
		struct xlgmac_channel *channel_head;
		/*mac registers base addr*/
		void __iomem *mac_base;
		void __iomem *base_addr;
		int phy_speed;
		unsigned char  *mac_addr;
		int dev_irq;

        //not finished ,need added later.
};


struct dwcroce_driver{
        unsigned char name[32];
        struct dwcroce_dev *(*add) (struct dwc_dev_info *dev_info);
        void (*remove) (struct dwcroce_dev *);
        // not finished,need added later.
	struct dwcroce_dev *dev;//To pass the dev for debuging,later change  hs 2019/6/20
};

int dwc_roce_register_driver(struct dwcroce_driver *drv);//add this in dwc_roce.c
void dwc_roce_unregister_driver(struct dwcroce_driver *drv);

#endif

