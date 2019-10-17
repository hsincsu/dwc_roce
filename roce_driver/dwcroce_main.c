/*
 *
 * This is the main file with the main funciton.All start here!
 *
 *
 *
 * 						------edited by hs in 2019/6/18
 *
 */

#include<linux/module.h>
#include <linux/idr.h>

#include <rdma/rdma_netlink.h>
#include <rdma/ib_verbs.h>
#include <rdma/ib_user_verbs.h>
#include <rdma/ib_addr.h>
#include <rdma/ib_mad.h>

#include <linux/netdevice.h>
#include <net/addrconf.h>

#include "dwcroce.h"
#include "dwcroce_verbs.h"
#include "dwcroce_ah.h"
#define HSDEBUG 1

MODULE_VERSION(DWCROCEDRV_VER);
MODULE_AUTHOR("HS");
MODULE_LICENSE("Dual BSD/GPL");

/*
 *rdma link_layer
 */
static enum rdma_link_layer dwcroce_get_link_layer(struct ib_device *device,
                                              u8 port_num)
{
        printk("dwcroce:dwcroce_link_layer start!\n");//added by hs for printing stat info
        printk("dwcroce:dwcroce_link_layer end!\n");//added by hs for printing end info
        return IB_LINK_LAYER_ETHERNET;
}

/*
 * dwcroce_port_immutable
 */
static int dwcroce_port_immutable(struct ib_device *ibdev, u8 port_num, 
			struct ib_port_immutable *immutable)
{
	printk("dwcroce:dwcroce_port_immutable start\n");//added by hs for start info
	/*wait to add*/

	/*wait to add*/
	printk("dwcroce:dwcroce_port_immutable end\n");//added by hs for end info
	return 0;
}
/*ib_device_ops definition for roce*/
const struct ib_device_ops dwcroce_dev_ops = {
	.query_device = dwcroce_query_device,
	.query_port = dwcroce_query_port,
	.modify_port = dwcroce_modify_port,
	.query_gid = dwcroce_query_gid,
	.get_netdev = dwcroce_get_netdev,
	.add_gid = dwcroce_add_gid,
	.del_gid = dwcroce_del_gid,
	.get_link_layer = dwcroce_get_link_layer,
	.alloc_pd = dwcroce_alloc_pd,
	.dealloc_pd = dwcroce_dealloc_pd,

	.create_cq = dwcroce_create_cq,
	.destroy_cq = dwcroce_destroy_cq,
	.resize_cq = dwcroce_resize_cq,

	.create_qp = dwcroce_create_qp,
	.modify_qp = dwcroce_modify_qp,
	.query_qp = dwcroce_query_qp,
	.destroy_qp = dwcroce_destroy_qp,

	.query_pkey = dwcroce_query_pkey,
	.create_ah = dwcroce_create_ah,
	.destroy_ah = dwcroce_destroy_ah,
	.query_ah = dwcroce_query_ah,
	.modify_ah = dwcroce_modify_ah,

	.poll_cq = dwcroce_poll_cq,
	.post_send = dwcroce_post_send,
	.post_recv = dwcroce_post_recv,
	.req_notify_cq = dwcroce_arm_cq,

	.get_dma_mr = dwcroce_get_dma_mr,
	.dereg_mr = dwcroce_dereg_mr,
	.reg_user_mr = dwcroce_reg_user_mr,

	.alloc_mr = dwcroce_alloc_mr,
	.map_mr_sg = dwcroce_map_mr_sg,

	.alloc_ucontext = dwcroce_alloc_ucontext,
	.dealloc_ucontext = dwcroce_dealloc_ucontext,
	.mmap = dwcroce_mmap,

	.process_mad = dwcroce_process_mad,
	.get_port_immutable = dwcroce_port_immutable,
};


/*
 *dwcroce_register_ibdev.To register the ibdev to kernel.must exec it before unregister_ibdev.
 *@struct dwcroce_dev  *dev. the struct describe the ibdev attribute.
 */
static int dwcroce_register_ibdev(struct dwcroce_dev *dev)
{
	printk("dwcroce:dwcroce_register_ibdev start \n");//added by hs for info
#if HSDEBUG //added by hs for debug
	strlcpy(dev->ibdev.name, "dwcroce%d", IB_DEVICE_NAME_MAX);
	printk("dwcroce:ibdev.name = %s",dev->ibdev.name);//added by hs for name info
	dev->ibdev.owner = THIS_MODULE;	
	//added later.now have no this information.
	dev->ibdev.node_type = RDMA_NODE_IB_CA;
	dev->ibdev.phys_port_cnt = 1;//not finished ,add later! hs 2019/6/22

	/*mandatory verbs. */
	ib_set_device_ops(&dev->ibdev, &dwcroce_dev_ops);
	dev->ibdev.dev.parent = NULL;
	dev->ibdev.driver_id = RDMA_DRIVER_UNKNOWN;
#endif
	printk("dwcroce:dwcroce_register_ibdev succeed end\n");//added by hs for info
	//return ib_register_device(&dev->ibdev,"dwcroce%d", NULL);//wait a moment
	return 0;
}

static struct dwcroce_dev *dwc_add(struct dwc_dev_info *dev_info)
{
	printk("dwcroce:dwc_add start\n");//added by hs for printing info
	int status = 0;
	u8 lstate = 0;
	struct dwcroce_dev *dev;
	u32 downlinkmtu;
	u32 uplinkmtu;
#if HSDEBUG //added by hs for debug
	dev = (struct dwcroce_dev *)ib_alloc_device(sizeof(struct dwcroce_dev));
	if(!dev) {
		printk("dwcroce:Unable to allocate ib device\n");//to show the err information.
		return NULL;
	}	
	printk("dwcroce:get the mac address is:%x,base addr is %x\n", dev_info->mac_base,dev_info->base_addr);
	writel(dev_info->base_addr + 0x0, UPLINKDOWNLINK + DOWNLINKMTU_POS);

	downlinkmtu = readl(dev_info->base_addr + 0x100);

	writel(dev_info->base_addr + 0x100, UPLINKDOWNLINK + UPLINKMTU_POS);

	uplinkmtu = real(dev_info->base_addr + 0x100);

	printk("dwcroce: get downlinkmtu %l,get uplinkmut %l \n", downlinkmtu, uplinkmtu);//added by hs

	status = dwcroce_register_ibdev(dev);
	if (status)
		goto alloc_err;
#endif
	printk("dwcroce:dwc_add succeed end\n");//added by hs for printing info
	return dev;//turn back the ib dev
alloc_err:
	ib_dealloc_device(&dev->ibdev);
	printk("dwcroce:error!alloc_err as dwcroce_register_ibdev failed\n");//added by hs for info
	return NULL;
}

static void dwc_remove(struct dwcroce_dev *dev)
{
	printk("dwcrove:dwc_remove start\n");//added by hs for printing dwc_remove info
	//ib_unregister_device(&dev->ibdev);
	ib_dealloc_device(&dev->ibdev);
	printk("dwcroce:dwc_remove succeed end \n");//added by hs for printing dwc_remove info
}

struct dwcroce_driver dwc_drv = {
	.name 		="dwcroce_driver",
	.add  		=dwc_add,
	.remove 	=dwc_remove,
	//not finished , added later
};

static int __init dwc_init_module(void)
{
	int status;
	printk("dwcroce:init module start!\n");//added by hs for printing init info
	status = dwc_roce_register_driver(&dwc_drv);	
	if(status)
		goto err_reg;
	printk("dwcroce:init module exit succeed!\n");//added by hs for printing init info
	return 0;
	
err_reg:
	return status;
}

static void __exit dwc_exit_module(void)
{
	printk("dwcroce:exit module start\n");//added by hs for printing info
	dwc_roce_unregister_driver(&dwc_drv);
	printk("dwcroce:exit module succeed!\n");//added by hs for print exit info
}

module_init(dwc_init_module);
module_exit(dwc_exit_module);
