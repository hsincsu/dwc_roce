/*
 *
 *
 *	this file is for verbs.operation function is almost here!
 *						-------edited by hs 2019/6/22
 *
 *
 */
#include <linux/dma-mapping.h>
#include <rdma/ib_verbs.h>
#include <rdma/ib_user_verbs.h>
#include <rdma/iw_cm.h>
#include <rdma/ib_umem.h>
#include <rdma/ib_addr.h>
#include <rdma/ib_cache.h>
#include <linux/netdevice.h>

#include "dwcroce.h"
#include "dwcroce_verbs.h"
#include "dwcroce_hw.h"

int dwcroce_post_send(struct ib_qp *ibqp,const struct ib_send_wr *wr,const struct ib_send_wr **bad_wr)
{
	printk("dwcroce:dwcroce_post_send start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_qp *qp;
	

	qp = get_dwcroce_qp(ibqp);


	/*wait to add end!*/
	printk("dwcroce:dwcroce_post_send succeed end!\n");//added by hs for printing end info
	return 0;
}

int dwcroce_post_recv(struct ib_qp *ibqp,const struct ib_recv_wr *wr,const struct ib_recv_wr **bad_wr)
{
	printk("dwcroce:dwcroce_post_recv start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_qp *qp;
	

	qp = get_dwcroce_qp(ibqp);
	
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_post_recv succeed end!\n");//added by hs for printing end info
	return 0;
}

int dwcroce_poll_cq(struct ib_cq *ibcq, int num_entries, struct ib_wc *wc)
{
	printk("dwcroce:dwcroce_poll_cq start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_poll_cq succeed end!\n");//added by hs for printing end info	
	return 0;
}

int dwcroce_arm_cq(struct ib_cq *ibcq,enum ib_cq_notify_flags flags)
{
	printk("dwcroce:dwcroce_arm_cq start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_arm_cq succeed end!\n");//added by hs for printing end info
	return 0;
}

int dwcroce_query_device(struct ib_device *ibdev, struct ib_device_attr *props,struct ib_udata *uhw)
{
	printk("dwcroce:dwcroce_query_device start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_dev *dev;
	dev = get_dwcroce_dev(ibdev);

//	if(uhw->inlen || uhw->outlen)
//		return -EINVAL;
	memset(props,0,sizeof *props);
	dwcroce_get_guid(dev,(u8 *)&props->sys_image_guid);
	props->vendor_id = dev->devinfo.pcidev->vendor;
	props->vendor_part_id = dev->devinfo.pcidev->device;
	
	props->atomic_cap = 0;
	props->max_fmr = 0;
	props->max_map_per_fmr = 0;



	/*wait to add end!*/	
	printk("dwcroce:dwcroce_query_device succeed end!\n");//added by hs for printing end info	
	return 0;
}

int dwcroce_query_port(struct ib_device *ibdev, u8 port, struct ib_port_attr *props)
{
	printk("dwcroce:dwcroce_query_port start!\n");//added by hs for printing start info
	enum ib_port_state port_state;
	struct dwcroce_dev *dev;
	struct net_device *netdev;
	/*wait to add 2019/6/24*/
	dev = get_dwcroce_dev(ibdev);
	netdev = dev->devinfo.netdev;
	printk("dwcrpce:query_port next is netif_running\n");//added by hs
	if(netif_running(netdev) && netif_oper_up(netdev)){
		printk("dwcroce:query_port in active\n");//added by hs 
			port_state = IB_PORT_ACTIVE;
			props->phys_state = 5;
	}
	else {
		printk("dwcroce:query_port in down\n");//added by hs 
		port_state = IB_PORT_DOWN;
		props->phys_state = 3;
	}
	printk("dwcroce:query_port next is lid .. \n");//added by hs 
	props->max_mtu = IB_MTU_4096;
	props->active_mtu = iboe_get_mtu(netdev->mtu);
	props->lid = 0;
	props->lmc = 0;
	props->sm_lid = 0;
	props->sm_sl = 0;
	props->state = port_state;
	props->port_cap_flags = IB_PORT_CM_SUP | IB_PORT_REINIT_SUP |
							IB_PORT_DEVICE_MGMT_SUP |
							IB_PORT_VENDOR_CLASS_SUP;
	/* not sure,need verified!*/
	props->ip_gids = true;
	props->gid_tbl_len = 0;
	props->pkey_tbl_len = 1;
	props->bad_pkey_cntr = 0;
	props->qkey_viol_cntr = 0;
	props->active_speed = 0;
	props->active_width = 0;
	props->max_msg_sz = 0x80000000;
	props->max_vl_num = 4;
	/*end */
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_query_port succeed end!\n");//added by hs for printing end info
	return 0;
}

int dwcroce_modify_port(struct ib_device *ibdev, u8 port, int mask,
		       struct ib_port_modify *props)
{
	printk("dwcroce:dwcroce_modify_port start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_modify_port succeed end!\n");//added by hs for printing end info
	return 0;
}

#if 0	/*this funtion is not need  right now*/
enum rdma_protocol_type
dwcroce_query_protocol(struct ib_device *device, u8 port_num)
{
	printk("dwcroce:dwcroce_query_protocol start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_query_protocol succeed end!\n");//added by hs for printing end info
}
#endif

void dwcroce_get_guid(struct dwcroce_dev *dev, u8 *guid)
{
	printk("dwcroce:dwcroce_get_guid start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
        u8 *addr;
        addr = dev->devinfo.netdev->dev_addr;
        guid[0] = addr[0]^2;
        guid[1] = addr[1];
        guid[2] = addr[2];
        guid[3] = 0xff;
        guid[4] = 0xfe;
        guid[5] = addr[3];
        guid[6] = addr[4];
        guid[7] = addr[5];
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_get_guid succeed end!\n");//added by hs for printing end info
}

int dwcroce_query_gid(struct ib_device *ibdev, u8 port,
		     int index, union ib_gid *gid)
{
	/*variable declaration*/
	int ret;
	/*variable declaration*/
	printk("dwcroce:dwcroce_query_gid start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	

	ret = 0;//alter this later, detail function not added 2019/6/24
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_query_gid succeed end!\n");//added by hs for printing end info
	return ret;
}

struct net_device *dwcroce_get_netdev(struct ib_device *device, u8 port_num)
{
	printk("dwcroce:dwcroce_get_netdev start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_dev *dev;
	struct net_device * ndev;
	/*rcu make sure that the shared file is safe*/
	rcu_read_lock();
	dev = get_dwcroce_dev(device);
	if(dev)
	ndev = dev->devinfo.netdev;
	if(ndev)
		dev_hold(ndev);
	rcu_read_unlock();
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_get_netdev succeed end!\n");//added by hs for printing end info
	return ndev;
}

int dwcroce_add_gid(const struct ib_gid_attr *attr,
		   void **context)
{
	printk("dwcroce:dwcroce_add_gid start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_add_gid succeed end!\n");//added by hs for printing end info
	return 0;
}

int  dwcroce_del_gid(const struct ib_gid_attr *attr,
		    void **context)
{
	printk("dwcroce:dwcroce_del_gid start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_del_gid succeed end!\n");//added by hs for printing end info
	return 0;
}

int dwcroce_query_pkey(struct ib_device *ibdev, u8 port, u16 index, u16 *pkey)
{
	printk("dwcroce:dwcroce_query_pkey start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_query_pkey succeed end!\n");//added by hs for printing end info
	return 0;
}

struct ib_ucontext *dwcroce_alloc_ucontext(struct ib_device *ibdev,
					  struct ib_udata *udata)
{
	printk("dwcroce:dwcroce_alloc_ucontext start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_alloc_ucontext succeed end!\n");//added by hs for printing end info
	return NULL;
}

int dwcroce_dealloc_ucontext(struct ib_ucontext *ibctx)
{
	/*variable declaration*/
	int status;
	/*variable declaration*/
	printk("dwcroce:dwcroce_dealloc_ucontext start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	
	status = 0;
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_dealloc_ucontext succeed end!\n");//added by hs for printing end info
	return status;
}

int dwcroce_mmap(struct ib_ucontext *ibctx, struct vm_area_struct *vma)
{
	/*variable declaration*/
	int status;
	/*variable declaration*/
	printk("dwcroce:dwcroce_mmap start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	
	
	status = 0;
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_mmap succeed end!\n");//added by hs for printing end info
	return status;
}

struct ib_pd *dwcroce_alloc_pd(struct ib_device *ibdev,
			  struct ib_ucontext *ibctx, struct ib_udata *udata)
{
	printk("dwcroce:dwcroce_alloc_pd start!\n");//added by hs for printing start info
	struct dwcroce_pd *pd;
	struct dwcroce_dev *dev = get_dwcroce_dev(ibdev);
	/*wait to add 2019/6/24*/
	if(dev)	
	pd = dwcroce_alloc(&dev->pd_pool);
	if(pd)
	printk("pd is exist\n");//added by hs	
//	mutex_lock(&dev->pd_mutex); // 利用位图来唯一分配PDN。
//	bitmap_idx = find_first_zero_bit(dev->pd_id,32);
//	pd->id = bitmap_idx;
//	__set_bit(bitmap_idx,dev->pd_id);
//	mutex_unlock(&dev->pd_mutex);
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_alloc_pd succeed end!\n");//added by hs for printing end info
	return pd ? &pd->ibpd:ERR_PTR(-ENOMEM);
}

int dwcroce_dealloc_pd(struct ib_pd *pd)
{
	printk("dwcroce:dwcroce_dealloc_pd start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_pd *dwcpd = get_dwcroce_pd(pd);
	
//	mutex_lock(&dev->pd_mutex);
//	__clear_bit(dwcpd->id,dev->pd_id);
//	mutex_unlock(&dev->pd_mutex);
	/*wait to add end!*/	
	dwcroce_drop_ref(dwcpd);
	printk("dwcroce:dwcroce_dealloc_pd succeed end!\n");//added by hs for printing end info
	return 0;
}

struct ib_cq *dwcroce_create_cq(struct ib_device *ibdev,
			       const struct ib_cq_init_attr *attr,
			       struct ib_ucontext *ib_ctx,
			       struct ib_udata *udata)
{
	printk("dwcroce:dwcroce_create_cq start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	int entries = attr->cqe;
	int vector = attr->comp_vector;
	struct dwcroce_cq *cq;
	struct dwcroce_dev *dev;
	u16 pd_id = 0;
	int status;
	u32 cq_num = 0;

	dev = get_dwcroce_dev(ibdev);
//	if(attr->flags)
//		return ERR_PTR(-EINVAL);
//	if(entries > dev->attr.max_cqe)
//		return ERR_PTR(-EINVAL);
	printk("dwcroce: entries is %d, flags is %d\n",entries,attr->flags);//added by hs
	if (udata) {
		printk("dwcroce:create_cq by user space\n");//added by hs
	}else
		printk("dwcroce:create_cq in kernel\n");//added by hs 
	cq = kzalloc(sizeof(*cq),GFP_KERNEL);
	if(!cq)
		return ERR_PTR(-ENOMEM);

	spin_lock_init(&cq->lock);
	/*All have 3 types of CQ*/
	//INIT_LIST_HEAD(&cq->sq_head); // cq for sq ,when send wqe is sent by card.
	//INIT_LIST_HEAD(&cq->rq_head);// cq for rq, when recv wqe is processed by card.
	//INIT_LIST_HEAD(&cq->xmit_head);//cq for xmit. when all wqe is ended

	/*wait to add end!*/	
	if (ib_ctx) {
		printk("dwcroce:some function for user space");//added by hs
	}
	status = dwcroce_alloc_cqqpresource(dev,dev->allocated_cqs,dev->attr.max_cq,&cq_num,&dev->next_cq);
	if (status)
	{
		printk("dwcroce_alloc_resource failed\n");//added by hs 
		return ERR_PTR(status);
	}
	cq->id = cq_num;
	printk("dwcroce: create_cq for cq_num is %d \n",cq_num);//added by hs 
	/*create cq -- access hw for these*/
	status = dwcroce_hw_create_cq(dev,cq,entries,pd_id);
	if (status) {
		kfree(cq);
		return ERR_PTR(status); 
	}

	dev->cq_table[cq->id] = cq;//for storing cq.
	printk("dwcroce:dwcroce_create_cq succeed end!\n");//added by hs for printing end info
	return &cq->ibcq;
}

int dwcroce_resize_cq(struct ib_cq *ibcq, int cqe, struct ib_udata *udata)
{
	printk("dwcroce:dwcroce_resize_cq start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_resize_cq succeed end!\n");//added by hs for printing end info
	return 0;
}

/*free resources*/
static void dwcroce_free_cqqpresource(struct dwcroce_dev *dev, struct dwcroce_cq *cq)
{
	struct pci_dev *pdev = dev->devinfo.pcidev;
	unsigned long flags;
	/*free kernel dma memory*/
	dma_free_coherent(&pdev->dev,cq->len,cq->txva,(dma_addr_t)cq->txpa);
	dma_free_coherent(&pdev->dev,cq->len,cq->rxva,(dma_addr_t)cq->rxpa);
	dma_free_coherent(&pdev->dev,cq->len,cq->xmitva,(dma_addr_t)cq->xmitpa);

	/*free va*/
	cq->txva = NULL;
	cq->rxva= NULL;
	cq->xmitva = NULL;

	/*free resources*/
	spin_lock_irqsave(&dev->resource_lock,flags);
	clear_bit(cq->id,dev->allocated_cqs);
	spin_unlock_irqrestore(&dev->resource_lock,flags);
}

/*destroy cq*/
int dwcroce_destroy_cq(struct ib_cq *ibcq)
{
	printk("dwcroce:dwcroce_destroy_cq start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_cq *cq;
	struct dwcroce_dev *dev;
	cq = get_dwcroce_cq(ibcq);
	dev = get_dwcroce_dev(ibcq->device);
	if (!ibcq) {
		printk("ibcq == NULL \n");//added by hs 
		return 0;
	}
	dwcroce_free_cqqpresource(dev,cq);
	kfree(cq);
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_destroy_cq succeed end!\n");//added by hs for printing end info
	return 0;
}

static int dwcroce_check_qp_params(struct ib_pd *ibpd, struct dwcroce_dev *dev,
								   struct ib_qp_init_attr *attrs, struct ib_udata *udata)
{
	if ((attrs->qp_type != IB_QPT_GSI) &&
		(attrs->qp_type != IB_QPT_RC) &&
		(attrs->qp_type != IB_QPT_UC) &&
		(attrs->qp_type != IB_QPT_UD)) {
		printk("%s unsupported qp type = 0x%x requested \n",__func__,attrs->qp_type);
		return -EINVAL;
	}

	 if ((attrs->qp_type != IB_QPT_GSI) &&
            (attrs->cap.max_send_wr > dev->attr.max_qp_wr)) {
                printk("dwcroce: %s unsupported send_wr =0x%x requested\n",__func__,attrs->cap.max_send_wr);//added by hs
				printk("dwcroce: %s unsupported send_wr = 0x%x\n",__func__,dev->attr.max_qp_wr);//added by hs 
                return -EINVAL;
        }

	 return 0;

}

/*dwcroce_set_qp_init_params. To get init params to private qp dwcroce_qp*/
static void dwcroce_set_qp_init_params(struct dwcroce_qp *qp, struct dwcroce_pd *pd, struct ib_qp_init_attr *attrs)
{
	qp->pd = pd;

	qp->qp_type = attrs->qp_type;
	qp->max_inline_data = attrs->cap.max_inline_data;
	qp->sq.max_sges = attrs->cap.max_send_sge;
	qp->rq.max_sges = attrs->cap.max_recv_sge;
	qp->qp_state = DWCROCE_QPS_RST;
	qp->signaled = (attrs->sq_sig_type == IB_SIGNAL_ALL_WR) ? true:false;

}

struct ib_qp *dwcroce_create_qp(struct ib_pd *ibpd,
			       struct ib_qp_init_attr *attrs,
			       struct ib_udata *udata)
{
	printk("dwcroce: dwcroce_create_qp start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_dev *dev;
	struct dwcroce_qp *qp;
	struct dwcroce_pd *pd;
	struct dwcroce_cq *cq;
	int status;
	u32 qp_num = 0;
	int sq_size;
	int rq_size;
	/*get some kernel private data*/
	sq_size = attrs->cap.max_send_wr;
	rq_size = attrs->cap.max_recv_wr;
	pd = get_dwcroce_pd(ibpd);
	dev = get_dwcroce_dev(ibpd->device);

	cq = get_dwcroce_cq(attrs->send_cq); // To get cq? but Most important that is send_cq && recv_cq  the same one.
	if(!cq){
		printk("dwcroce: cq is null \n");//added by hs 
		return -ENOMEM;
	}
	/*check attrs is valid or not*/
	status = dwcroce_check_qp_params(ibpd,dev,attrs,udata);
	if (status) {
		printk("dwcroce: check qp error \n");//added by hs 
		return status;
	}
	/*allocate memory for private qp*/
	qp = kzalloc(sizeof(*qp),GFP_KERNEL);
	if (!qp) {
		printk("dwcroce: qp is null \n");//added by hs 
		return -ENOMEM;
	}
	
	/*get attrs to private qp */
	dwcroce_set_qp_init_params(qp,pd,attrs);

	/*alloate id for qp*/
	status = dwcroce_alloc_cqqpresource(dev,dev->allocated_qps,dev->attr.max_qp,&qp_num,&dev->next_qp);
	qp->id = qp_num;
	printk("dwcroce: create_qp for qp_num is %d\n",qp_num);//added by hs 
	/*kenrel create qp*/
	status = dwcroce_hw_create_qp(dev,qp,cq,pd,attrs);
		if (status) {
			kfree(qp);
			return ERR_PTR(status);
	}
				 /*wait to add end!*/	
	dev->qp_table[qp->id] = qp;

	/*test wqe wether wqe is 48 byte*/
	printk("dwcroce: test wqe to get or read start\n");//added by hs
	struct dwcroce_wqe *wqe;
	wqe = kzalloc(sizeof(*wqe),GFP_KERNEL);
	wqe->pkey = 0x1110;
	printk("dwcroce:wqe's size is %d",sizeof(struct dwcroce_wqe));//added by hs 
	printk("dwcroce:wqe's pkey is %x\n",wqe->pkey);//added by hs
	printk("dwcroce:wqe's pkey is %x\n",*(u32 *)((u8 *)wqe +4 ));//added by hs 
	printk("dwcroce:wqe's eecnxt1's addr is %x\n",&wqe->eecntx);//added by hs 
	printk("dwcroce:wqe's destsocket1's addr is %x\n",&wqe->destsocket1);//added by hs 
	kfree(wqe);
	/*test end*/
	printk("dwcroce: dwcroce_create_qp succeed end!\n");//added by hs for printing end info
	return &qp->ibqp;
}

int _dwcroce_modify_qp(struct ib_qp *ibqp, struct ib_qp_attr *attr,
		      int attr_mask)
{
	printk("dwcroce:dwcroce_modify_qp start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	int status = 0;
	struct dwcroce_qp *qp;
	struct dwcroce_dev *dev;
	enum ib_qp_state cur_state,new_state;
	qp = get_dwcroce_qp(ibqp);
	dev = get_dwcroce_dev(ibqp->device);
	new_state = get_dwcroce_qp_state(attr->qp_state);
	u32 lqp = qp->id;
	
	if(attr_mask & IB_QP_STATE)
		status = dwcroce_qp_state_change(qp,attr->qp_state,&cur_state);
	if(status < 0)
		return status;
	/*wait to add end!*/	
	status = dwcroce_set_qp_params(qp,attr,attr_mask);
	if(status)
		return status;
	if(qp->qp_state == DWCROCE_QPS_RTR)//now we may have got dest qp.we should map destqp and srcqp.
	{
		/*access hw for map destqp and srcqp*/
		printk("dwcroce: modify_qp in RTR,map destqp and srcqp\n");//added by hs 
		void __iomem* base_addr;
		base_addr = dev->devinfo.base_addr;
		u32 destqp = qp->destqp;
		writel(PGU_BASE + SRCQP,base_addr + MPB_WRITE_ADDR); // INIT PSN
		writel(lqp,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + DESTQP,base_addr + MPB_WRITE_ADDR);
		writel(destqp,base_addr + MPB_RW_DATA);

		writel(PGU_BASE + RC_QPMAPPING,base_addr + MPB_WRITE_ADDR);
		writel(0x1,base_addr + MPB_RW_DATA);
	}
	printk("dwcroce:dwcroce_modify_qp succeed end!\n");//added by hs for printing end info
	return status;
}

int dwcroce_modify_qp(struct ib_qp *ibqp, struct ib_qp_attr *attr,
		     int attr_mask, struct ib_udata *udata)
{
	printk("dwcroce:dwcroce_modify_qp start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_qp *qp;
	struct dwcroce_dev *dev;
	enum ib_qp_type qp_type;
	enum ib_qp_state cur_state, new_state;
	int status = -EINVAL;

	qp = get_dwcroce_qp(ibqp);
	dev = get_dwcroce_dev(ibqp->device);
	mutex_lock(&dev->dev_lock);

	cur_state = get_ibqp_state(qp->qp_state);
	if(attr_mask & IB_QP_STATE)
		new_state = attr->qp_state;
	else
		new_state = cur_state;

	if (!ib_modify_qp_is_ok(cur_state, new_state, ibqp->qp_type, attr_mask)) {
		printk("%s invalid attribute mask=0x%x specified for\n"
               "qpn=0x%x of type=0x%x old_qps=0x%x, new_qps=0x%x\n",
               __func__,attr_mask,qp->id,ibqp->qp_type,cur_state,new_state);//added by hs 
	}

	status = _dwcroce_modify_qp(ibqp,attr,attr_mask);
	if(status > 0)
		status = 0;
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_modify_qp succeed end!\n");//added by hs for printing end info
	mutex_unlock(&dev->dev_lock);
	return status;
}

int dwcroce_query_qp(struct ib_qp *ibqp,
		    struct ib_qp_attr *qp_attr,
		    int qp_attr_mask, struct ib_qp_init_attr *init_attr)
{
	printk("dwcroce:dwcroce_query_qp start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_query_qp succeed end!\n");//added by hs for printing end info
	return 0;
}

int dwcroce_destroy_qp(struct ib_qp *ibqp)
{
	printk("dwcroce:dwcroce_destroy_qp start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_qp *qp;
	struct dwcroce_dev *dev;
	struct pci_dev *pdev;
	dev = get_dwcroce_dev(ibqp->device);
	qp = get_dwcroce_qp(ibqp);
	pdev = dev->devinfo.pcidev;
	if(qp->sq.va)
		dma_free_coherent(&pdev->dev,qp->sq.len,qp->sq.va,qp->sq.pa);
	if(qp->rq.va)
		dma_free_coherent(&pdev->dev,qp->rq.len,qp->rq.va,qp->rq.pa);
	kfree(qp);
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_destroy_qp succeed end!\n");//added by hs for printing end info
	return 0;
}
//void dwcroce_del_flush_qp(struct ocrdma_qp *qp);

struct ib_srq *dwcroce_create_srq(struct ib_pd *ibpd, struct ib_srq_init_attr *init_attr,struct ib_udata *udata)
{
	printk("dwcroce:dwcroce_create_srq start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_create_srq succeed end!\n");//added by hs for printing end info
	return NULL;
}

int dwcroce_modify_srq(struct ib_srq *ibsrq, struct ib_srq_attr *srq_attr,
		      enum ib_srq_attr_mask srq_attr_mask, struct ib_udata *udata)
{
	printk("dwcroce:dwcroce_modify_srq start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_modify_srq succeed end!\n");//added by hs for printing end info
	return 0;
}

int dwcroce_query_srq(struct ib_srq *ibsrq, struct ib_srq_attr *srq_attr)
{
	printk("dwcroce:dwcroce_query_srq start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_query_srq succeed end!\n");//added by hs for printing end info
	return 0;
}

int dwcroce_destroy_srq(struct ib_srq *ibsrq)
{
	printk("dwcroce:dwcroce_destroy_srq start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_destroy_srq succeed end!\n");//added by hs for printing end info
	return 0;
}

int dwcroce_post_srq_recv(struct ib_srq *ibsrq, struct ib_recv_wr *wr,
			 struct ib_recv_wr **bad_recv_wr)
{
	printk("dwcroce:dwcroce_post_srq_recv start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_post_srq_recv succeed end!\n");//added by hs for printing end info
	return 0;
}

int dwcroce_dereg_mr(struct ib_mr *ibmr)
{
	printk("dwcroce:dwcroce_dereg_mr start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	//struct dwcroce_dev *dev;
	struct dwcroce_mr *mr;

	mr = get_dwcroce_mr(ibmr);
	if(mr)
	printk("mr exist\n");//added by hs 
	else 
		return 0;
	mr->state = DWCROCE_MEM_STATE_ZOMBIE;
	dwcroce_drop_ref(mr->pd);
	dwcroce_drop_index(mr);
	dwcroce_drop_ref(mr);
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_dereg_mr succeed end!\n");//added by hs for printing end info
	return 0;
}
static int dwcroce_alloc_lkey(struct dwcroce_dev *dev, struct dwcroce_mr *mr, u32 pdid, int acc)
{
	int status;
	printk("dwcroce: dwcroce_alloc_lkey start\n");//added by hs
	
	printk("dwcroce: dwcroce_alloc_lkey end\n");//added by hs 
	return 0;
}

struct ib_mr *dwcroce_get_dma_mr(struct ib_pd *ibpd, int acc)
{
	printk("dwcroce:dwcroce_get_dma_mr start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	int status;
	struct dwcroce_pd *pd;
	struct dwcroce_mr *mr;
	struct dwcroce_dev *dev;
	u32 pdn = 0;
	pd = get_dwcroce_pd(ibpd);
	dev = get_dwcroce_dev(ibpd->device);
	int err;
	if (acc & IB_ACCESS_REMOTE_WRITE && !(acc & IB_ACCESS_LOCAL_WRITE)){
		pr_err("%s err, invalid access rights \n",__func__);
		return ERR_PTR(-EINVAL);
	}
	
	mr = dwcroce_alloc(&dev->mr_pool);
	if (!mr) {
		err = -ENOMEM;
		goto err1;
	}
	dwcroce_add_index(mr);
	dwcroce_add_ref(pd);

	err = dwcroce_mem_init_dma(pd,acc,mr);
	if(err)
		goto err2;

	/*wait to add end!*/
	printk("dwcroce:dwcroce_get_dma_mr succeed end!\n");//added by hs for printing end info
	return &mr->ibmr;
err2:
	dwcroce_drop_ref(pd);
	dwcroce_drop_index(mr);
	dwcroce_drop_ref(mr);
err1:
	return ERR_PTR(err);
}

struct ib_mr *dwcroce_reg_user_mr(struct ib_pd *ibpd, u64 start, u64 length,
				 u64 virt, int acc, struct ib_udata *udata)
{
	printk("dwcroce:dwcroce_reg_user_mr start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_reg_user_mr succeed end!\n");//added by hs for printing end info
	return NULL;
}

struct ib_mr *dwcroce_alloc_mr(struct ib_pd *pd,
			      enum ib_mr_type mr_type,
			      u32 max_num_sg)
{
	printk("dwcroce:dwcroce_alloc_mr start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_dev *dev = get_dwcroce_dev(pd->device);
	struct dwcroce_pd *dwcpd = get_dwcroce_pd(pd);
	struct dwcroce_mr *mr;
	int err;

	if(mr_type != IB_MR_TYPE_MEM_REG)
			return	ERR_PTR(-EINVAL);
	mr = dwcroce_alloc(&dev->mr_pool);
	if (!mr) {
			err = -ENOMEM;
			goto err1;
	}
	dwcroce_add_index(mr);
	dwcroce_add_ref(dwcpd);

	err = dwcroce_mem_init_fast(dev,max_num_sg,mr);
	if(err)
		goto err2;
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_alloc_mr succeed end!\n");//added by hs for printing end info
	return &mr->ibmr;
err2:
	dwcroce_drop_ref(dwcpd);
	dwcroce_drop_index(mr);
	dwcroce_drop_ref(mr);
err1:
	return ERR_PTR(err);
}

int dwcroce_map_mr_sg(struct ib_mr *ibmr, struct scatterlist *sg, int sg_nents,unsigned int *sg_offset)
{
	printk("dwcroce:dwcroce_map_mr_sg start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_map_mr_sg succeed end!\n");//added by hs for printing end info
	return 0;
}

void dwcroce_cq_cleanup(struct dwcroce_pool_entry *arg)
{
	printk("dwcroce: dwcroce_cq_cleanup\n");//added by hs 
}
void dwcroce_qp_cleanup(struct dwcroce_pool_entry *arg)
{
	printk("dwcroce： dwcroce_qp_cleanup\n");//added by hs 
}
void dwcroce_mem_cleanup(struct dwcroce_pool_entry *arg)
{
	printk("dwcroce:  dwcroce_mem_cleanup start\n");//added by hs 
	struct dwcroce_mr *mr = container_of(arg, struct dwcroce_mr, pelem);
	int i;
	if(mr->umem)
		ib_umem_release(mr->umem);
	if (mr->map) {
		for(i=0;i<mr->num_map;i++)
			kfree(mr->map[i]);
		kfree(mr->map);
	}
}
