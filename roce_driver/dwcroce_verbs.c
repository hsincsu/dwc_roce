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

#include "dwcroce.h"
#include "dwcroce_verbs.h"

int dwcroce_post_send(struct ib_qp *ibqp,const struct ib_send_wr *wr,const struct ib_send_wr **bad_wr)
{
	printk("dwcroce:dwcroce_post_send start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_qp *qp;
	struct dwcroce_dev *dev;

	qp = get_dwcroce_qp(ibqp);
	dev = qp->dev;


	/*wait to add end!*/
	printk("dwcroce:dwcroce_post_send succeed end!\n");//added by hs for printing end info
	return 0;
}

int dwcroce_post_recv(struct ib_qp *ibqp,const struct ib_recv_wr *wr,const struct ib_recv_wr **bad_wr)
{
	printk("dwcroce:dwcroce_post_recv start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_qp *qp;
	struct dwcroce_dev *dev;

	qp = get_dwcroce_qp(ibqp);
	dev = qp->dev;
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


	/*wait to add end!*/	
	printk("dwcroce:dwcroce_query_device succeed end!\n");//added by hs for printing end info	
	return 0;
}

int dwcroce_query_port(struct ib_device *ibdev, u8 port, struct ib_port_attr *props)
{
	printk("dwcroce:dwcroce_query_port start!\n");//added by hs for printing start info
	enum ib_port_state port_state;
	struct dwcroce_dev *dev = get_dwcroce_dev(ibdev);
	struct net_device * netdev = dev->devinfo->netdev;
	/*wait to add 2019/6/24*/
	if(netif_running(netdev) && netif_oper_up(netdev)){
			port_state = IB_PORT_ACTIVE;
			props->phys_state = 5;
	}
	else {
		port_state = IB_PORT_DOWN;
		props->phys_state = 3;
	}
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
        addr = dev->devinfo->netdev->dev_addr;
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
	ndev = dev->devinfo->netdev;
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
	struct dwcroce_dev *dev = get_dwcroce_dev(dev);
	int status;
	u8 is_utx_pd =false;
	u32 bitmap_idx = 0;
	/*wait to add 2019/6/24*/
	if(ibctx && udata){
		
		printk("dwcroce: get userspace context & pd\n");//added by hs 
	}
	
	pd = kzalloc(sizeof(*pd),GFP_KERNEL);
	if(!pd)
			return ERR_PTR(-ENOMEM);
	pd->dev = dev;
	mutex_lock(&dev->pd_mutex); // 利用位图来唯一分配PDN。
	bitmap_idx = find_first_zero_bit(dev->pd_id,32);
	pd->id = bitmap_idx;
	__set_bit(bitmap_idx,dev->pd_id);
	mutex_unlock(&dev->pd_mutex);
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_alloc_pd succeed end!\n");//added by hs for printing end info
	return &pd->ibpd;
}

int dwcroce_dealloc_pd(struct ib_pd *pd)
{
	printk("dwcroce:dwcroce_dealloc_pd start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_pd *dwcpd = get_dwcroce_pd(pd);
	struct dwcroce_dev *dev = dwcpd->dev;
	mutex_lock(&dev->pd_mutex);
	__clear_bit(dwcpd->id,dev->pd_id);
	mutex_unlock(&dev->pd_mutex);
	/*wait to add end!*/	
	kfree(dwcpd);
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
	struct dwcroce_dev *dev;
	struct dwcroce_cq *cq;
	
	dev = get_dwcroce_dev(ibdev);
	cq = kzalloc(sizeof(*cq),GFP_KERNEL);
	if(!cq)
		return ERR_PTR(-ENOMEM);
	cq->dev = dev;
	/*wait to add end!*/	
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

int dwcroce_destroy_cq(struct ib_cq *ibcq)
{
	printk("dwcroce:dwcroce_destroy_cq start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_cq *cq;
	cq = get_dwcroce_cq(ibcq);
	kfree(cq);
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_destroy_cq succeed end!\n");//added by hs for printing end info
	return 0;
}

struct ib_qp *dwcroce_create_qp(struct ib_pd *ibpd,
			       struct ib_qp_init_attr *attrs,
			       struct ib_udata *udata)
{
	printk("dwcroce:dwcroce_create_qp start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_dev *dev;
	struct dwcroce_qp *qp;

	qp = kzalloc(sizeof(*qp),GFP_KERNEL);
	
	qp->dev = dev;
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_create_qp succeed end!\n");//added by hs for printing end info
	return &qp->ibqp;
}

int _dwcroce_modify_qp(struct ib_qp *ibqp, struct ib_qp_attr *attr,
		      int attr_mask)
{
	printk("dwcroce:dwcroce_modify_qp start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_modify_qp succeed end!\n");//added by hs for printing end info
	return 0;
}

int dwcroce_modify_qp(struct ib_qp *ibqp, struct ib_qp_attr *attr,
		     int attr_mask, struct ib_udata *udata)
{
	printk("dwcroce:dwcroce_modify_qp start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_qp *qp;
	struct dwcroce_dev *dev;

	qp = get_dwcroce_qp(ibqp);
	dev = qp->dev;

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_modify_qp succeed end!\n");//added by hs for printing end info
	return 0;
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
	qp = get_dwcroce_qp(ibqp);
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
	kfree(mr);
	/*wait to add end!*/	
	printk("dwcroce:dwcroce_dereg_mr succeed end!\n");//added by hs for printing end info
	return 0;
}

struct ib_mr *dwcroce_get_dma_mr(struct ib_pd *ibpd, int acc)
{
	printk("dwcroce:dwcroce_get_dma_mr start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_dev *dev;
	struct dwcroce_mr *mr;

	mr = kzalloc(sizeof(*mr),GFP_KERNEL);
	if(!mr)
		return ERR_PTR(-ENOMEM);
	mr->dev =dev;
	/*wait to add end!*/
	printk("dwcroce:dwcroce_get_dma_mr succeed end!\n");//added by hs for printing end info
	return &mr->ibmr;
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

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_alloc_mr succeed end!\n");//added by hs for printing end info
	return NULL;
}

int dwcroce_map_mr_sg(struct ib_mr *ibmr, struct scatterlist *sg, int sg_nents,unsigned int *sg_offset)
{
	printk("dwcroce:dwcroce_map_mr_sg start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_map_mr_sg succeed end!\n");//added by hs for printing end info
	return 0;
}

