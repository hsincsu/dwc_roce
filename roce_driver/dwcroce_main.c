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
#include "dwcroce_hw.h"
#include <rdma/ocrdma-abi.h>
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
	struct ib_port_attr attr;
	struct dwcroce_dev *dev;
	int err;

	dev = get_dwcroce_dev(ibdev);
	immutable->core_cap_flags = RDMA_CORE_PORT_IBA_ROCE;
	/*support udp encap ?*/
	immutable->core_cap_flags |= RDMA_CORE_CAP_PROT_ROCE_UDP_ENCAP;
	err = ib_query_port(ibdev,port_num, &attr);
	if(err)
			return err;
	/*not sure , need verified*/
	immutable->pkey_tbl_len = attr.pkey_tbl_len;
	immutable->gid_tbl_len = attr.gid_tbl_len;
	immutable->max_mad_size = IB_MGMT_MAD_SIZE;

	/*wait to add*/
	printk("dwcroce:dwcroce_port_immutable end\n");//added by hs for end info
	return 0;
}

/*dev_attr_hw_rev 's show function*/
static ssize_t hw_rev_show(struct device *device, struct device_attribute *attr, char *buf)
{
	printk("dwcroce: hw_rev_show start\n");//added by hs
	struct dwcroce_dev *dev;
	struct xlgmac_pdata *pdata;
	dev =  container_of(device, struct dwcroce_dev, ibdev.dev);
	pdata = dev->devinfo.pdata;
	if(pdata)
	printk("dwcroce: hw_rev is 0x\n");//added by hs
	printk("dwcroce: hw_rev_show end\n");//added by hs	
	return scnprintf(buf,PAGE_SIZE,"0x%x\n",dev->devinfo.pcidev->vendor);
	return 0;
}
static DEVICE_ATTR_RO(hw_rev);

/*dev_attr_hca_type 's show function*/
static ssize_t hca_type_show(struct device *device, struct device_attribute *attr, char *buf)
{
	printk("dwcroce: hca_type_show start \n");//added by hs
	struct  dwcroce_dev *dev; 
	struct 	xlgmac_pdata *pdata;
	dev = container_of(device, struct dwcroce_dev, ibdev.dev);
 	pdata = dev->devinfo.pdata;
	if(pdata)
	printk("dwcroce: hca_type is DWC\n");//added by hs
	printk("dwcroce: hca_type_show end \n");//added by hs
	return sprintf(buf,"DWC%d\n",dev->devinfo.pcidev->device);
	return 0;
}
static DEVICE_ATTR_RO(hca_type);

/*attribute file*/
static struct attribute *dwcroce_attributes[] = {
	&dev_attr_hw_rev.attr,
	&dev_attr_hca_type.attr,
	NULL

};

/*attribute group*/
static const struct attribute_group dwcroce_attr_group = {
	.attrs = dwcroce_attributes,
};

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
	//strlcpy(dev->ibdev.name, "dwcroce%d", IB_DEVICE_NAME_MAX);
//	printk("dwcroce:ibdev.name = %s",dev->ibdev.name);//added by hs for name info
	dwcroce_get_guid(dev,(u8 *)&dev->ibdev.node_guid);
	printk("dwcroce: node_guid is %0lx\n",dev->ibdev.node_guid);//added by hs
	dev->ibdev.owner = THIS_MODULE;	
	dev->ibdev.uverbs_abi_ver = 2;//temple value.added by hs
	//added later.now have no this information.
	dev->ibdev.node_type = RDMA_NODE_IB_CA;
	dev->ibdev.phys_port_cnt = 1;//not finished ,add later! hs 2019/6/22
	dev->ibdev.uverbs_cmd_mask = 
		(1ull << IB_USER_VERBS_CMD_GET_CONTEXT)			|
		(1ull << IB_USER_VERBS_CMD_QUERY_DEVICE)		|
		(1ull << IB_USER_VERBS_CMD_QUERY_PORT)			|
		(1ull << IB_USER_VERBS_CMD_ALLOC_PD)			|
		(1ull << IB_USER_VERBS_CMD_DEALLOC_PD)			|
		(1ull << IB_USER_VERBS_CMD_CREATE_AH)			|
		(1ull << IB_USER_VERBS_CMD_DESTROY_AH)			|
		(1ull << IB_USER_VERBS_CMD_REG_MR)				|
		(1ull << IB_USER_VERBS_CMD_REREG_MR)			|
		(1ull << IB_USER_VERBS_CMD_DEREG_MR)			|
		(1ull << IB_USER_VERBS_CMD_CREATE_COMP_CHANNEL) |
		(1ull << IB_USER_VERBS_CMD_CREATE_CQ)			|
		(1ull << IB_USER_VERBS_CMD_RESIZE_CQ)			|
		(1ull << IB_USER_VERBS_CMD_DESTROY_CQ)			|
		(1ull << IB_USER_VERBS_CMD_CREATE_QP)			|
		(1ull << IB_USER_VERBS_CMD_MODIFY_QP)			|
		(1ull << IB_USER_VERBS_CMD_QUERY_QP)			|
		(1ull << IB_USER_VERBS_CMD_DESTROY_QP)			|
		(1ull << IB_USER_VERBS_CMD_ATTACH_MCAST)		|
		(1ull << IB_USER_VERBS_CMD_DETACH_MCAST)		|
		(1ull << IB_USER_VERBS_CMD_CREATE_SRQ)			|
		(1ull << IB_USER_VERBS_CMD_MODIFY_SRQ)			|
		(1ull << IB_USER_VERBS_CMD_QUERY_SRQ)			|
		(1ull << IB_USER_VERBS_CMD_DESTROY_SRQ)			|
		(1ull << IB_USER_VERBS_CMD_CREATE_XSRQ)			|
		(1ull << IB_USER_VERBS_CMD_OPEN_QP);

	/*mandatory verbs. */
	ib_set_device_ops(&dev->ibdev, &dwcroce_dev_ops);
	dev->ibdev.dev.parent =&dev->devinfo.pcidev->dev;
	/*create device attr file*/
	rdma_set_device_sysfs_group(&dev->ibdev,&dwcroce_attr_group);
	/*end*/
	dev->ibdev.driver_id = RDMA_DRIVER_UNKNOWN;
#endif
	printk("dwcroce:dwcroce_register_ibdev succeed end\n");//added by hs for info
	return ib_register_device(&dev->ibdev,"dwcroce%d", NULL);//wait a moment
//	return 0;
}
static int dwcroce_init_pools(struct dwcroce_dev *dev)
{
	printk("dwcroce: dwcroce_init_pools start\n");//added by hs 
	int err;
	err = dwcroce_pool_init(dev,&dev->pd_pool,DWCROCE_TYPE_PD,
							dev->attr.max_pd);
	
	if (err)
		goto err1;
	err = dwcroce_pool_init(dev,&dev->mr_pool,DWCROCE_TYPE_MR,
							dev->attr.max_mr);
	if (err)
		goto err2;
	printk("dwcroce: dwcroce_init_pools end\n");//added by hs 
	return 0;
err2:
	dwcroce_pool_cleanup(&dev->mr_pool);
err1:
	dwcroce_pool_cleanup(&dev->pd_pool);
	return err;
}

static int dwcroce_alloc_resource(struct dwcroce_dev *dev)
{
	printk("dwcroce: dwcroce_alloc_resource start\n");//added by hs
	int status;
	status = dwcroce_init_pools(dev);
	if(status)
		goto err1;
	return 0;
err1:
	printk("init pool failed\n");//added by hs
	return status;
}

static int dwcroce_alloc_hw_resources(struct dwcroce_dev *dev)
{
	printk("dwcroce:-----------------------test start------------------------\n");//added by hs 
	printk("dwcroce:test ib_alloc_pd:\n");//added by hs 
	struct ib_cq_init_attr cq_attr = {.cqe = 1};
	struct ib_pd *ibpd = ib_alloc_pd(&dev->ibdev,NULL);
	struct ib_port_attr attr;
	if(!ibpd)
		goto err_pd;
	printk("dwcroce:ib_alloc_pd succeed!\n");//added by hs
	struct ib_cq *ibcq = ib_create_cq(&dev->ibdev,NULL,NULL,NULL,&cq_attr);
	if(!ibcq)
		goto err_cq;
	printk("dwcroce: ib_alloc_pd succeed! \n");//added by hs
	printk("dwcroce:------------------------test end --------------------\n");//added by hs 

	ib_query_port(&dev->ibdev, 1, &attr);	
	ib_dealloc_pd(ibpd);
	ib_destroy_cq(ibcq);
	return 0;
err_cq:
	printk("create_cq");//added by hs 
err_pd:
	printk("alloc_pd failed\n");//added by hs 
	return ERR_PTR(-ENOMEM);	
}

static struct dwcroce_dev *dwc_add(struct dwc_dev_info *dev_info)
{
	printk("dwcroce:dwc_add start\n");//added by hs for printing info
	int status = 0;
	u8 lstate = 0;
	struct dwcroce_dev *dev;
#if HSDEBUG //added by hs for debug
	dev = (struct dwcroce_dev *)ib_alloc_device(sizeof(struct dwcroce_dev));
	if(!dev) {
		printk("dwcroce:Unable to allocate ib device\n");//to show the err information.
		return NULL;
	}	
	memcpy(&dev->devinfo, dev_info, sizeof(*dev_info));
	printk("dwcroce:get the mac address is:%x,base addr is %x\n", dev->devinfo.mac_base,dev_info->base_addr);
	printk("dwcroce: get vendor is 0x%x,pci device is DWC%d\n",dev->devinfo.pcidev->vendor,dev->devinfo.pcidev->device);//added by hs
	mutex_init(&dev->pd_mutex);
	status = dwcroce_init_hw(dev);// init hw
	if (status)
		goto err_inithw;
	status = dwcroce_get_hwinfo(dev);//read hw
	if (status)
		goto err_getinfo;
	status = dwcroce_alloc_resource(dev);//alloc some resources
	if (status)
		goto err_alloc;
	status = dwcroce_register_ibdev(dev);//register ib_device
	if (status)
		goto alloc_err;
	 printk("dwcroce:get the mac address is:%x,base addr is %x\n", dev->devinfo.mac_base,dev_info->base_addr);
        printk("dwcroce: get vendor is 0x%x,pci device is DWC%d\n",dev->devinfo.pcidev->vendor,dev->devinfo.pcidev->device);//added by hs

#endif
	printk("dwcroce:dwc_add succeed end\n");//added by hs for printing info

//	/*test ibdev*/
//	status = dwcroce_alloc_hw_resources(dev);
//	if (status)
//		goto alloc_hwres;
	return dev;//turn back the ib dev
alloc_hwres:
	printk("alloc hw res failed\n");//added by hs for info
err_alloc:
	printk("alloc failed\n");//added by hs for info
alloc_err:
	ib_dealloc_device(&dev->ibdev);
	printk("dwcroce:error!alloc_err as dwcroce_register_ibdev failed\n");//added by hs for info
err_getinfo:
	printk("read hw failed\n");//added by hs;
err_inithw:
	printk("init hw failed\n");//added by hs for info
	return NULL;
}

static void dwc_remove(struct dwcroce_dev *dev)
{
	printk("dwcrove:dwc_remove start\n");//added by hs for printing dwc_remove info
	ib_unregister_device(&dev->ibdev);
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
	status = dwcroce_cache_init();
	if(status){
		printk("unable to init object pools\n");//added by hs 
		return status;
	}
		
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
	dwcroce_cache_exit();
	printk("dwcroce:exit module succeed!\n");//added by hs for print exit info
}

module_init(dwc_init_module);
module_exit(dwc_exit_module);
