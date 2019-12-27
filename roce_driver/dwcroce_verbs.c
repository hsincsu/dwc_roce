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

static void dwcroce_set_wqe_dmac(struct dwcroce_qp *qp, struct dwcroce_wqe *wqe)
{
	struct dwcroce_wqe tmpwqe;
	memset(&tmpwqe,0,sizeof(struct dwcroce_wqe));
	tmpwqe->destqp = qp->mac_addr[1];
	tmpwqe->destqp << 8;
	tmpwqe->destqp = tmpwqe->destqp + qp->mac_addr[0];
	tmpwqe->destqp <<4;
	tmpwqe->destsocket1 = qp->mac_addr[5];
	tmpwqe->destqp << 8;
	tmpwqe->destsocket1 += qp->mac_addr[4];
	tmpwqe->destqp << 8;
	tmpwqe->destsocket1 += qp->mac_addr[3];
	tmpwqe->destqp <<8;
	tmpwqe->destsocket1 +=qp->mac_addr[2];
	tmpwqe->destqp <<4;
	tmpwqe->destsocket1 += qp->mac_addr[1] & 0xf0;
	tmpwqe->destsocket2 += qp->mac-addr[5] & 0x0f;

	wqe->destqp & 0x000f;
	wqe->destqp += tmpwqe->destqp;
	wqe->destsocket1 & 0x0;
	wqe->destsocket1 tmpwqe->destsocket1;
	wqe->destsocket2 & 0xf0;
	wqe->destsocket2 += tmpwqe->destsocket2;
	printk("dwcroce:%s destqp:0x%x,  destsocket1: 0x%x,  destsocket2: 0x%x \n"
		,__func__,wqe->destqp,wqe->destsocket1,wqe->destsocket2);//added by hs


}

static void dwcroce_set_wqe_opcode(struct dwcroce_wqe *wqe,u8 qp_type,u8 opcode)
{
	u8 opcode_l = 0;
	u8 opcode_h = 0;
	opcode_l = opcode;
	opcode_l = opcode_l << 4;
	opcode_h = qp_type;
	if(wqe->destsocket2 >> 4)
		wqe->destsocket2 = wqe->destsocket2 & 0x0f;
	wqe->destsocket2 += opcode_l;
	printk("dwcroce:%s,destsocket2:0x%x \n",__func__,wqe->destsocket2);//added by hs
	if(wqe->opcode << 4)
		wqe->opcode = wqe->destsocket2 & 0xf0;
	wqe->opcode += opcode_h;
	printk("dwcroce:%s,opcode:0x%x \n",__func__,wqe->opcode);//added by hs
}

//set rc,uc 's wqe
static void dwcroce_set_wqe_destqp(struct dwcroce_qp *qp,struct dwcroce_wqe *wqe)
{
	
	u16 tempqpn;
	u16 tempqpn_l = 0;
	u16 tempqpn_h = 0;

	tempqpn =qp->destqp; // get lower 16bits ,but qpn only 10 bits.
	tempqpn_l = tempqpn & 0x003f;
	tempqpn_l = tempqpn_l << 10;

	tempqpn_h = tempqpn& 0x03c0; // get higher 4 bits;
	tempqpn_h = tempqpn_h >> 6;

	/*make sure the wqe's eecntx higher 6 bits is zero*/
	if(wqe->eecntx >> 10)
		wqe->eecntx = wqe->eecntx & 0x03ff; // mask is 0000 0011 1111 1111 to make higher 6 bits zero.
	wqe->eecntx += tempqpn_l;
	
	if(wqe->destqp << 12)
		wqe->destqp = wqe->destqp & 0xfff0;
	wqe->destqp += tempqpn_h;
	printk("dwcroce:%s2,eecntx:0x%x , destqp:0x%x \n",__func__,wqe->eecntx,wqe->destqp);
}

//set ud 's wqe
static void dwcroce_set_wqe_destqp(struct dwcroce_qp *qp, struct dwcroce_wqe *wqe, const struct ib_send_wr *wr) {
	
	u16 tempqpn;
	u16 tempqpn_l = 0;
	u16 tempqpn_h = 0;

	tempqpn =ud_wr(wr)->remote_qpn; // get lower 16bits ,but qpn only 10 bits.
	tempqpn_l = tempqpn & 0x003f;
	tempqpn_l = tempqpn_l << 10;

	tempqpn_h = tempqpn& 0x03c0; // get higher 4 bits;
	tempqpn_h = tempqpn_h >> 6;

	/*make sure the wqe's eecntx higher 6 bits is zero*/
	if(wqe->eecntx >> 10)
		wqe->eecntx = wqe->eecntx & 0x03ff; // mask is 0000 0011 1111 1111 to make higher 6 bits zero.
	wqe->eecntx += tempqpn_l;

	if(wqe->destqp << 12)
		wqe->destqp = wqe->destqp & 0xfff0;
	wqe->destqp += tempqpn_h;
	printk("dwcroce:%s3,eecntx:0x%x, destqp:0x%x \n",__func__,wqe->eecntx,wqe->destqp);//added by hs
}

static int  dwcroce_build_wqe_opcode(struct dwcroce_qp *qp,struct dwcroce_wqe *wqe,struct ib_send_wr *wr)
{
	int status= 0;
	u8 qp_type;
	u8 opcode;
	switch(qp->qp_type) {
		case IB_QPT_UD:
				qp_type = UD;
				break;
		case IB_QPT_UC:
				qp_type = UC;
				break;
		case IB_QPT_RESERVED2:
				qp_type = RD;
				break;
		case IB_QPT_RC:
				qp_type = RC;
				break;
		default:
				printk("dwcroce: qp type default ...\n");//added by hs
				status = 0x1;
				break;
	}

	switch (wr->opcode) {
	case IB_WR_SEND:
				opcode = SEND;
				break;
	case IB_WR_SEND_WITH_IMM:
				opcode = SEND_WITH_IMM;
				break;
	case IB_WR_SEND_WITH_INV:
				opcode = SEND_WITH_INV;
				break;
	case IB_WR_RDMA_WRITE:
				opcode = RDMA_WRITE;
				break;
	case IB_WR_RDMA_WRITE_WITH_IMM:
				opcode = WRITE_WITH_IMM;
				break;
	case IB_WR_RDMA_READ:
				opcode = RDMA_READ;
				break;	
	default:
				printk("dwcroce: wr opcode default...\n");//added by hs
				status = status | 0x2;	
				break;	
	}
	if(status & 0x1||status & 0x2)
	{
		printk("dwcroce: transport or  opcode not supported \n");//added by hs 	
		return -EINVAL;
	}
	if(qp_type == UD && !(opcode & (SEND|SEND_WITH_IMM)))
		return -EINVAL;
	if(qp_type == UC && !(opcode &(SEND|SEND_WITH_IMM|WRITE|WRITE_WITH_IMM )))
		return -EINVAL;
	if(qp_type == RD && (opcode & SEND_WITH_INV))
		return -EINVAL;
	printk("dwcroce: %s,qp_type:0x%x , opcode:0x%x \n "
			,__func__,qp_type,opcode);//added by hs
	dwcroce_set_wqe_opcode(wqe,qp_type,opcode);
	return 0;

}

static int dwcroce_build_sges(struct dwcroce_qp *qp, struct dwcroce_wqe *wqe, int num_sge, struct ib_sge *sg_list,struct ib_send_wr *wr)
{
	int i;
	int status = 0;
	struct dwcroce_wqe *tmpwqe = wqe;
	for (i = 0; i < num_sge; i++) {
		status = dwcroce_build_wqe_opcode(qp,tmpwqe,wr);//added by hs 
		if(status)
			return status;
		if(qp->destqp)
			dwcroce_set_wqe_destqp(qp,tmpwqe);
		
		dwcroce_set_wqe_dmac(qp,tmpwqe);
		tmpwqe->qkey = qp->qkey;
		tmpwqe->rkey = sg_list[i].rkey;
		tmpwqe->lkey = sg_list[i].lkey;
		tmpwqe->localaddr = sg_list[i].addr;
		tmpwqe->dmalen = sg_list[i].length;
		printk("dwcroce: ---------------check send wqe--------------\n");//added by hs
		printk("dwcroce:immdat:0x%x \n",tmpwqe->immdt);//added by hs
		printk("dwcroce:pkey:0x%x \n",tmpwqe->pkey);//added by hs
		printk("dwcroce:rkey:0x%x \n",tmpwqe->rkey);//added by hs
		printk("dwcroce:lkey:0x%x \n",tmpwqe->lkey);//added by hs
		printk("dwcroce:qkey:0x%x \n",tmpwqe->qkey);//added by hs
		printk("dwcroce:dmalen:0x%x \n",tmpwqe->dmalen);//added by hs
		printk("dwcroce:destaddr:0x%x \n",tmpwqe->destaddr);//added by hs
		printk("dwcroce:localaddr:0x%x \n",tmpwqe->localaddr);//added by hs
		printk("dwcroce:eecntx:0x%x \n",tmpwqe->eecntx);//added by hs
		printk("dwcroce:destqp:0x%x \n",tmpwqe->destqp);//added by hs
		printk("dwcroce:destsocket1:0x%x \n",tmpwqe->destsocket1);//added by hs
		printk("dwcroce:destsocket2:0x%x \n",tmpwqe->destsocket2);//added by hs
		printk("dwcroce:opcode:0x%x \n",tmpwqe->opcode);//added by hs
		printk("dwcroce:----------------check send wqe end------------\n");//added by hs
		tmpwqe += 1;
	}
	if (num_sge == 0) {
		memset(wqe,0,sizeof(*wqe));
	}
	return status;
}

static int dwcroce_buildwrite_sges(struct dwcroce_qp *qp, struct dwcroce_wqe *wqe,int num_sge, struct ib_sge *sg_list, struct ib_send_wr *wr)
{
	int i;
	int status = 0;
	struct dwcroce_wqe *tmpwqe = wqe;
	for (i = 0; i < num_sge; i++) {
		status = dwcroce_build_wqe_opcode(qp,tmpwqe,wr);//added by hs 
		if(status)
			return -EINVAL;
		tmpwqe->rkey = rdma_wr(wr)->rkey;
		tmpwqe->lkey = sg_list[i].lkey;
		tmpwqe->localaddr = sg_list[i].addr;
		tmpwqe->dmalen = sg_list[i].length;
		tmpwqe->destaddr = rdma_wr(wr)->remote_addr;
		printk("dwcroce: ---------------check write wqe--------------\n");//added by hs
		printk("dwcroce:immdat:0x%x \n",tmpwqe->immdt);//added by hs
		printk("dwcroce:pkey:0x%x \n",tmpwqe->pkey);//added by hs
		printk("dwcroce:rkey:0x%x \n",tmpwqe->rkey);//added by hs
		printk("dwcroce:lkey:0x%x \n",tmpwqe->lkey);//added by hs
		printk("dwcroce:qkey:0x%x \n",tmpwqe->qkey);//added by hs
		printk("dwcroce:dmalen:0x%x \n",tmpwqe->dmalen);//added by hs
		printk("dwcroce:destaddr:0x%x \n",tmpwqe->destaddr);//added by hs
		printk("dwcroce:localaddr:0x%x \n",tmpwqe->localaddr);//added by hs
		printk("dwcroce:eecntx:0x%x \n",tmpwqe->eecntx);//added by hs
		printk("dwcroce:destqp:0x%x \n",tmpwqe->destqp);//added by hs
		printk("dwcroce:destsocket1:0x%x \n",tmpwqe->destsocket1);//added by hs
		printk("dwcroce:destsocket2:0x%x \n",tmpwqe->destsocket2);//added by hs
		printk("dwcroce:opcode:0x%x \n",tmpwqe->opcode);//added by hs
		printk("dwcroce:----------------check write wqe end------------\n");//added by hs
		tmpwqe += 1;
	}
	if (num_sge == 0) {
		memset(wqe,0,sizeof(*wqe));
	}
	return status;
}

static inline uint32_t dwcroce_sglist_len(struct ib_sge *sg_list, int num_sge)
{
	uint32_t total_len =0, i;
	for(i=0;i < num_sge; i++)
		total_len += sg_list[i].length;
	return total_len;
}

static int dwcroce_build_inline_sges(struct dwcroce_qp *qp, struct dwcroce_wqe *wqe, const struct ib_send_wr *wr, u32 wqe_size)
{
	int i;

	if (wr->send_flags & IB_SEND_INLINE && qp->qp_type != IB_QPT_UD) {//
		wqe->dmalen = dwcroce_sglist_len(wr->sg_list,wr->num_sge);
			printk("%s() supported_len = 0x%x,\n"
				   "unsupported len req =0x%x,\n,the funtion is not supported now\n",__func__,qp->max_inline_data,wqe->dmalen);//added by hs 
			return -EINVAL;
	}
	else {
		dwcroce_build_sges(qp,wqe,wr->num_sge,wr->sg_list,wr);
		if(wr->num_sge){
			wqe_size +=((wr->num_sge-1) * sizeof(struct dwcroce_wqe));
			qp->sq.head = (qp->sq.head + wr->num_sge) % qp->sq.max_cnt; // update the head ptr,and check if the queue if full.
			if(qp->sq.head == qp->sq.tail){
				qp->sq.qp_foe == DWCROCE_Q_FULL;
			}
		}
		else {
			qp->sq.head = (qp->sq.head + 1) % qp->sq.max_cnt; // update the head ptr, and check if the queue if full.
			if(qp->sq.head == qp->sq.tail){
				qp->sq.qp_foe == DWCROCE_Q_FULL;
			}
		}
	}
	printk("dwcroce: post send, sq.head is %d, sq.tail is %d \n",qp->sq.head,qp->sq.tail);//added by hs
	return 0;


}


static int dwcroce_buildwrite_inline_sges(struct dwcroce_qp *qp,struct dwcroce_wqe *wqe,const struct ib_send_wr *wr, u32 wqe_size)
{
	int i;
	if (wr->send_flags & IB_SEND_INLINE && qp->qp_type != IB_QPT_UD) {//
		wqe->dmalen = dwcroce_sglist_len(wr->sg_list,wr->num_sge);
			printk("%s() supported_len = 0x%x,\n"
				   "unsupported len req =0x%x,\n,the funtion is not supported now\n",__func__,qp->max_inline_data,wqe->dmalen);//added by hs 
			return -EINVAL;
	}
	else {
		dwcroce_buildwrite_sges(qp,wqe,wr->num_sge,wr->sg_list,wr);
		if(wr->num_sge){
			wqe_size +=((wr->num_sge-1)*sizeof(struct dwcroce_wqe));
			qp->sq.head = (qp->sq.head + wr->num_sge) % qp->sq.max_cnt; // update the head ptr,and check if the queue if full.
			if(qp->sq.head == qp->sq.tail){
				qp->sq.qp_foe == DWCROCE_Q_FULL;
			}
		}
		else {
			qp->sq.head = (qp->sq.head + 1) % qp->sq.max_cnt; // update the head ptr, and check if the queue if full.
			if(qp->sq.head == qp->sq.tail){
				qp->sq.qp_foe == DWCROCE_Q_FULL;
			}
		}
	}
	printk("dwcroce: post send, sq.head is %d, sq.tail is %d\n",qp->sq.head,qp->sq.tail);//added by hs
	return 0;
}


static int dwcroce_build_send(struct dwcroce_qp *qp, struct dwcroce_wqe *wqe, const struct ib_send_wr *wr)
{
	int status;
	struct dwcroce_sge *sge;
	u32 wqe_size = sizeof(*wqe);

	if (qp->qp_type == IB_QPT_UD || qp->qp_type == IB_QPT_GSI) {
			dwcroce_set_wqe_destqp(qp,wqe,wr);
			if(qp->qp_type == IB_QPT_GSI)
				wqe->qkey = qp->qkey;
			else
				wqe->qkey = ud_wr(wr)->remote_qkey;
	}
	status = dwcroce_build_inline_sges(qp,wqe,wr,wqe_size);
	return status;
}

static int dwcroce_build_write(struct dwcroce_qp *qp, struct dwcroce_wqe *wqe, const struct ib_send_wr *wr) 
{
	int status;
	u32 wqe_size = sizeof(*wqe);

	status = dwcroce_buildwrite_inline_sges(qp,wqe,wr,wqe_size);
	if(status)
		return status;
	
}

static void dwcroce_build_read(struct dwcroce_qp *qp, struct dwcroce_wqe *wqe, const struct ib_send_wr *wr)
{
	u32 wqe_size = sizeof(*wqe);
	dwcroce_buildwrite_inline_sges(qp,wqe,wr,wqe_size);
}

static int dwcroce_build_reg(struct dwcroce_qp *qp, struct dwcroce_wqe *wqe, const struct ib_reg_wr *wr)
{
	u32 wqe_size = sizeof(*wqe);
	printk("don't support reg mr now\n");//added by hs 
	return 0;
}

static void dwcroce_ring_sq_hw(struct dwcroce_qp *qp) {
	struct dwcroce_dev *dev;
	u32 qpn;
	dev = get_dwcroce_dev(qp->ibqp.device);
	/*from head to get dma address*/
	u32 phyaddr;
	phyaddr =qp->sq.head * sizeof(struct dwcroce_wqe); //head * sizeof(wqe)
	printk("dwcroce: post send wp's phyaddr is %x \n",phyaddr);//added by hs	
	/*access hw ,write wp to notify hw*/
	void __iomem* base_addr;
	base_addr = dev->devinfo.base_addr;
	qpn = qp->id;

	writel(PGU_BASE + QPLISTREADQPN,base_addr + MPB_WRITE_ADDR);
	writel(qpn,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + WRITEORREADQPLIST,base_addr + MPB_WRITE_ADDR);
	writel(0x1,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + WRITEQPLISTMASK,base_addr + MPB_WRITE_ADDR);
	writel(0x7,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + QPLISTWRITEQPN,base_addr + MPB_WRITE_ADDR);
	writel(0x0,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + WPFORQPLIST, base_addr + MPB_WRITE_ADDR); // give the offset in the sq.
	writel(phyaddr,base_addr + MPB_RW_DATA); 

	writel(PGU_BASE + WRITEQPLISTMASK,base_addr + MPB_WRITE_ADDR);
	writel(0x1,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + QPLISTWRITEQPN,base_addr + MPB_WRITE_ADDR);
	writel(0x1,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + WRITEORREADQPLIST,base_addr + MPB_WRITE_ADDR);
	writel(0x0,base_addr + MPB_RW_DATA);
	// end


}

static int dwcroce_check_foe(struct dwcroce_qp_hwq_info *q, struct ib_send_wr *wr, u32 free_cnt)
{
	if (wr->num_sge > free_cnt)
		return -ENOMEM;
	else return 0;
}

static int dwcroce_hwq_free_cnt(struct dwcroce_qp_hwq_info *q)
{
	if(q->head > q->tail)
		return ((q->max_wqe_idx - q->head) + q->tail)% q->max_cnt;
	if (q->head == q->tail) {
		if(q->qp_foe == DWCROCE_Q_EMPTY)
			return q->max_cnt;
		else
			return 0;	
	}
	if(q->head < q->tail)
		return q->tail - q->head;
}

static void *dwcroce_hwq_head(struct dwcroce_qp_hwq_info *q) {
	return q->va + (q->head * q->entry_size);
}

int dwcroce_post_send(struct ib_qp *ibqp,const struct ib_send_wr *wr,const struct ib_send_wr **bad_wr)
{
	printk("dwcroce:dwcroce_post_send start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/
	struct dwcroce_qp *qp;
	struct dwcroce_dev *dev;
	struct dwcroce_wqe *hdwqe;
	unsigned long flags;
	int status = 0;
	u32 free_cnt;
	qp = get_dwcroce_qp(ibqp);
	dev = get_dwcroce_dev(ibqp->device);
	spin_lock_irqsave(&qp->sq.lock,flags);
	if (qp->qp_state != DWCROCE_QPS_RTS) {
		spin_unlock_irqrestore(&qp->sq.lock,flags);
		*bad_wr = wr;
		return -EINVAL;
	}

	printk("dwcroce: post_send, process sge.. \n");//added by hs
	while(wr){ //if UD, should support SEND OR SEND WITH IMM,or it can't do anything.
		if(qp->qp_type == IB_QPT_UD &&
		  (wr->opcode != IB_WR_SEND &&
		   wr->opcode != IB_WR_SEND_WITH_IMM)){
			*bad_wr = wr;
			status = -EINVAL;
			break;
		}
		free_cnt = dwcroce_hwq_free_cnt(&qp->sq);

		if(free_cnt == 0 || wr->num_sge > qp->sq.max_sges){
			*bad_wr = wr;
			status = -ENOMEM;
			break;
		}
		status = dwcroce_check_foe(&qp->sq,wr,free_cnt);// check if the wr can be processed with enough memory.
		if(status) break;

		hdwqe = dwcroce_hwq_head(&qp->sq); // To get the head ptr.

		switch(wr->opcode){
		case IB_WR_SEND_WITH_IMM:
			hdwqe->immdt = ntohl(wr->ex.imm_data);
		case IB_WR_SEND:
			status = dwcroce_build_send(qp,hdwqe,wr);
			break;
		case IB_WR_SEND_WITH_INV:
			hdwqe->lkey = wr->ex.invalidate_rkey;
			status = dwcroce_build_send(qp,hdwqe,wr);
			break;
		case IB_WR_RDMA_WRITE_WITH_IMM:
			hdwqe->immdt = ntohl(wr->ex.imm_data);
		case IB_WR_RDMA_WRITE:
			status = dwcroce_build_write(qp,hdwqe,wr);
			break;
		case IB_WR_RDMA_READ:
			dwcroce_build_read(qp,hdwqe,wr);
			break;
		case IB_WR_LOCAL_INV:
			hdwqe->lkey = wr->ex.invalidate_rkey;
		case IB_WR_REG_MR:
			status = dwcroce_build_reg(qp,hdwqe,reg_wr(wr));
			break;
		default:
			status = -EINVAL;
			break;
		}
		if (status) {
			*bad_wr = wr;
			break;
		}
		if(wr->send_flags & IB_SEND_SIGNALED || qp->signaled)
				qp->wqe_wr_id_tbl[qp->sq.head].signaled = 1;
		else
				qp->wqe_wr_id_tbl[qp->sq.head].signaled = 0;
		qp->wqe_wr_id_tbl[qp->sq.head].wrid = wr->wr_id;

		/*make sure wqe is written befor adapter can access it*/
		wmb();

		dwcroce_ring_sq_hw(qp); // notify hw to send wqe.


		wr = wr->next;
	}

	/*wait to add end!*/
	spin_unlock_irqrestore(&qp->sq.lock,flags);
	printk("dwcroce:dwcroce_post_send succeed end!\n");//added by hs for printing end info
	return status;
}

static void dwcroce_ring_rq_hw(struct dwcroce_qp *qp)
{
	 struct dwcroce_dev *dev;
	 dev = get_dwcroce_dev(qp->ibqp.device);
	 /*from head to get dma address*/
	u32 phyaddr;
	phyaddr =qp->rq.head * sizeof(struct dwcroce_rqe); //head * sizeof(wqe)
	printk("rq wp's phyaddr is %x\n",phyaddr);//added by hs
	/*access hw ,write wp to notify hw*/
	void __iomem* base_addr;
	u32 qpn;
	base_addr = dev->devinfo.base_addr;
	qpn = qp->id;
	phyaddr = phyaddr << 10; // because wp 's postition is 10 bytes from revq_inf.
	printk("rq wp's phyadr is %x \n",phyaddr);//added by hs
	qpn = qpn + phyaddr;
	printk("rq wp+qpn is %x \n",qpn);//added by hs

	//update rq's wp ,so hw can judge that there is still some wqes not processed.
	writel(PGU_BASE + RCVQ_INF,base_addr + MPB_WRITE_ADDR);
	writel(qpn,base_addr + MPB_RW_DATA);

	writel(PGU_BASE + RCVQ_WRRD,base_addr + MPB_WRITE_ADDR);
	writel(0x2,base_addr + MPB_RW_DATA);
	//end


}

static void dwcroce_build_rqsges(struct dwcroce_rqe *rqe, struct ib_recv_wr *wr)
{
	int i;
	int num_sge = wr->num_sge;
	struct dwcroce_rqe *tmprqe = rqe;
	struct ib_sge *sg_list;
	sg_list = wr->sg_list;
	for (i = 0; i < num_sge; i++) {
		tmprqe->descbaseaddr = sg_list[i].addr;
		tmprqe->dmalen = sg_list[i].length;
		tmprqe->opcode = 0x80000000;
		tmprqe += 1;
		printk("dwcroce: in rq,num_sge = %d, tmprqe 's addr is %x\n",num_sge,tmprqe);//added by hs
	}
	if(num_sge == 0)
		memset(tmprqe,0,sizeof(*tmprqe));
}

static void dwcroce_build_rqe(struct dwcroce_qp *qp,struct dwcroce_rqe* rqe, const struct ib_recv_wr* wr) 
{
	u32 wqe_size = 0;

	dwcroce_build_rqsges(rqe,wr);
	printk("dwcroce:-----------------------check rq wqe--------------------\n");//added by hs
	printk("dwcroce:descbaseaddr: %x \n",rqe->descbaseaddr);//added by hs
	printk("dwcroce:dmalen:		  %x \n",rqe->dmalen);//added by hs
	printk("dwcroce:opcode:		  %x \n",rqe->opcode);//added by hs
	if(wr->num_sge){
			wqe_size +=((wr->num_sge-1) * sizeof(struct dwcroce_wqe));
			qp->rq.head = (qp->rq.head + wr->num_sge) % qp->rq.max_cnt; // update the head ptr,and check if the queue if full.
			if(qp->rq.head == qp->rq.tail){
				qp->rq.qp_foe == DWCROCE_Q_FULL;
			}
			
	}
		else {
			qp->rq.head = (qp->rq.head + 1) % qp->rq.max_cnt; // update the head ptr, and check if the queue if full.
			if(qp->rq.head == qp->rq.tail){
				qp->rq.qp_foe == DWCROCE_Q_FULL;
			}
	}
	printk("dwcroce: in rq,qp->rq.head is %d, qp->rq.tail is %d \n",qp->rq.head,qp->rq.tail);//added by hs
	
}

int dwcroce_post_recv(struct ib_qp *ibqp,const struct ib_recv_wr *wr,const struct ib_recv_wr **bad_wr)
{
		printk("dwcroce:dwcroce_post_recv start!\n");//added by hs for printing start info
		int status = 0;
		unsigned long flags;
		struct dwcroce_qp *qp = get_dwcroce_qp(ibqp);
		struct dwcroce_rqe *rqe;
		struct dwcroce_dev *dev = get_dwcroce_dev(ibqp->device);
		u32 free_cnt;
		/*wait to add 2019/6/24*/
		printk("dwcroce: in rq qpn is %d \n",qp->id);//added by hs
		spin_lock_irqsave(&qp->rq.lock,flags);
		if (qp->qp_state == DWCROCE_QPS_RST || qp->qp_state == DWCROCE_QPS_ERR) {
			spin_unlock_irqrestore(&qp->rq.lock,flags);
			*bad_wr = wr;
			return -EINVAL;
		}
		while (wr) {
			free_cnt = dwcroce_hwq_free_cnt(&qp->rq);
			if (free_cnt == 0 || wr->num_sge > qp->rq.max_sges) {
				*bad_wr = wr;
				status = -ENOMEM;
				break;
			}
			status = dwcroce_check_foe(&qp->rq,wr,free_cnt);// check if the wr can be processed with enough memory.
			if(status) break;

			rqe = dwcroce_hwq_head(&qp->rq);
			printk("dwcroce: in rq, free_cnt=%d, rqe is %x \n",free_cnt,rqe);//added by hs
			dwcroce_build_rqe(qp,rqe,wr); // update rq->head & set rqe 's value
	
			qp->rqe_wr_id_tbl[qp->rq.head] = wr->wr_id; // to store the wr ,so CQ can verify which one is for this wr.
			/*make sure rqe is written before hw access it*/
			wmb();
			/*notify hw to process the rq*/
			dwcroce_ring_rq_hw(qp);

			wr = wr->next;
		}
	
		/*wait to add end!*/	
		spin_unlock_irqrestore(&qp->rq.lock,flags);
		printk("dwcroce:dwcroce_post_recv succeed end!\n");//added by hs for printing end info
		return status;
}

/*access hw for cqe*/
static int dwcroce_poll_hwcq(struct dwcroce_cq *cq, int num_entries, struct ib_wc *ibwc)
{
		u16 qpn = 0;
        int i = 0;
        bool expand = false;
        int polled_hw_cqes = 0;
        struct dwcroce_qp *qp = NULL;
        struct dwcroce_dev *dev = get_dwcroce_dev(cq->ibcq.device);
        struct dwcroce_cqe *cqe;
        u16 cur_getp; bool polled = false; bool stop = false; 

		while (num_entries) {
			/*get tx cqe*/
			/*read cq's wp,rp*/
			/*read qp's sq.tail, update qp's sq.tail*/
			/*get CQE from spcific CQ,retrieve the CQE to dwcroce_cqe.*/
			/*fill the ib_wc ,next cqe.*/
			break;
		}
		
		

		return num_entries;
}

/*poll cqe from cq.*/
int dwcroce_poll_cq(struct ib_cq *ibcq, int num_entries, struct ib_wc *wc)
{
		printk("dwcroce:dwcroce_poll_cq start!\n");//added by hs for printing start info
		/*wait to add 2019/6/24*/
		int cqes_to_poll = num_entries;
		struct dwcroce_cq *cq = get_dwcroce_cq(ibcq);
		struct dwcroce_dev *dev = get_dwcroce_dev(ibcq->device);
		int num_os_cqe = 0, err_cqes = 0;
		struct dwcroce_qp *qp;
		unsigned long flags;

		/*poll cq from hw*/
		spin_lock_irqsave(&cq->lock,flags);
		num_os_cqe = dwcroce_poll_hwcq(cq, cqes_to_poll,wc);//To get cq from hw,Please note that there is 3 types cq queues for one cq.
		spin_unlock_irqrestore(&cq->lock,flags);
		cqes_to_poll -= num_os_cqe; //if cqes_to_poll ==0,means all cqs needed have been received.
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
	/*a little mistake is that props should be attrs, may fix later*/
	//	if(uhw->inlen || uhw->outlen)
	//		return -EINVAL;
		memset(props,0,sizeof *props);
		dwcroce_get_guid(dev,(u8 *)&props->sys_image_guid);
		props->vendor_id = dev->devinfo.pcidev->vendor;
		props->vendor_part_id = dev->devinfo.pcidev->device;
		props->page_size_cap = 0xffff000;
		props->device_cap_flags = IB_DEVICE_CURR_QP_STATE_MOD |
											IB_DEVICE_RC_RNR_NAK_GEN |
											IB_DEVICE_SHUTDOWN_PORT |
											IB_DEVICE_SYS_IMAGE_GUID |
											//IB_DEVICE_LOCAL_DMA_LKEY |
											IB_DEVICE_MEM_MGT_EXTENSIONS;	
		props->max_pd = 1024;
		props->max_mr = 256*1024;
		props->max_cq = 16384;
       		props->max_qp = 1024;
		props->max_cqe = 256;
		props->max_qp_wr = 1024;
		props->max_send_sge = 256;
		props->max_recv_sge = 256;

		props->atomic_cap = 0;
		props->max_fmr = 0;
		props->max_map_per_fmr = 0;
		props->max_pkeys = 1;


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
		props->gid_tbl_len = 16;
		props->pkey_tbl_len = 1;
		props->bad_pkey_cntr = 0;
		props->qkey_viol_cntr = 0;
		props->active_speed = IB_SPEED_DDR;
		props->active_width = IB_WIDTH_4X;
		props->max_msg_sz = 1 << 16;
		props->max_vl_num = 0;
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
		   // u8 *addr;
		   u8 mac[ETH_ALEN];
		// addr = dev->devinfo.netdev->dev_addr;
			memcpy(mac, dev->devinfo.netdev->dev_addr, ETH_ALEN);
		printk("dwcroce:mac address is %s \n",dev->devinfo.netdev->dev_addr);//added by hs
			guid[0] = mac[0] ^ 2;
			guid[1] = mac[1];
			guid[2] = mac[2];
			guid[3] = 0xff;
			guid[4] = 0xfe;
			guid[5] = mac[3];
			guid[6] = mac[4];
			guid[7] = mac[5];
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
		if (index > 1)
					return -EINVAL;

			*pkey = 0xffff;
			return 0;

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
	   
		printk("--------------check qp1's ib_qp_init_attr start---------------\n");//added by hs
//		printk("cap.max_send_wr is %d \n",attrs->cap.max_send_wr);
//		printk("cap.max_recv_wr is %d \n",attrs->cap.max_recv_wr);
//		printk("cap.max_send_sge is %d \n",attrs->cap.max_send_sge);
//		printk("cap.max_recv_sge is %d \n",attrs->cap.max_recv_sge);
//		printk("cap.max_inline_data is %d \n",attrs->cap.max_inline_data);
//		printk("cap.max_rdma_ctxs is %d \n",attrs->cap.max_rdma_ctxs);
//		printk("qptype is %d \n",attrs->qp_type);
//		printk("port_num is %d \n",attrs->port_num);
//		printk("source_qpn is %d \n",attrs->source_qpn);
//		printk("--------------check qp1's ib_qp_init_attr  end---------------\n");//added by hs

		if ((attrs->qp_type != IB_QPT_GSI) &&
				(attrs->qp_type != IB_QPT_RC) &&
				(attrs->qp_type != IB_QPT_UC) &&
				(attrs->qp_type != IB_QPT_UD)) {
				printk("%s unsupported qp type = 0x%x requested \n",__func__,attrs->qp_type);
				return -EINVAL;
		}
		printk("dwcroce: gsi ,max_send_wr \n");//added by hs
	    if ((attrs->qp_type != IB_QPT_GSI) &&
				(attrs->cap.max_send_wr > dev->attr.max_qp_wr)) {
					printk("dwcroce: %s unsupported send_wr =0x%x requested\n",__func__,attrs->cap.max_send_wr);//added by hs
					printk("dwcroce: %s unsupported send_wr = 0x%x\n",__func__,dev->attr.max_qp_wr);//added by hs 
					return -EINVAL;
		}
//	    if (!attrs->srq && (attrs->cap.max_recv_wr > dev->attr.max_qp_wr)) {
//               pr_err("%s unsupported recv_wr=0x%x requested\n",
//                       __func__,attrs->cap.max_recv_wr);
//                pr_err("%s(%d) supported recv_wr=0x%x\n",
//                       __func__,dev->attr.max_qp_wr);
//               return -EINVAL;
//        }
		printk("dwcroce: cap.max_inline-data \n");//added by hs
        if (attrs->cap.max_inline_data > 0) {
                pr_err("%s unsupported inline data size=0x%x requested\n",
                       __func__,attrs->cap.max_inline_data);
                pr_err("%s supported inline data size=0\n",
                       __func__);
                return -EINVAL;
        }
		if (attrs->cap.max_send_sge > dev->attr.max_send_sge) {
			pr_err("%s unsupported send_sge=0x%x requested \n",__func__,attrs->cap.max_send_sge);
			pr_err("%s supported send_sge = 0x%x \n",__func__,dev->attr.max_send_sge);
			return -EINVAL;
		}
		if (attrs->cap.max_recv_sge > dev->attr.max_recv_sge) {
			pr_err("%s unsupported send_sge=0x%x requested \n",__func__,attrs->cap.max_recv_sge);
			pr_err("%s supported send_sge = 0x%x \n",__func__,dev->attr.max_recv_sge);
			return -EINVAL;
		}
		if (attrs->qp_type == IB_QPT_GSI && udata) {
			pr_err("Userpace can't create special QPs of type = 0x %x \n",__func__,attrs->qp_type);
		}

	printk("dwccroce:check qp param end \n");//added by hs

		

		 return 0;

}

/*dwcroce_set_qp_init_params. To get init params to private qp dwcroce_qp*/
static void dwcroce_set_qp_init_params(struct dwcroce_qp *qp, struct dwcroce_pd *pd, struct ib_qp_init_attr *attrs)
{
		printk("dwcroce: set_qp_init params \n");//added by hs
		qp->pd = pd;
		spin_lock_init(&qp->sq.lock);
		spin_lock_init(&qp->rq.lock);
		mutex_init(&qp->mutex);

		qp->qp_type = attrs->qp_type;
		qp->max_inline_data = attrs->cap.max_inline_data;
		qp->sq.max_sges = attrs->cap.max_send_sge;
		qp->rq.max_sges = attrs->cap.max_recv_sge;
		qp->qp_state = DWCROCE_QPS_RST;
		qp->signaled = (attrs->sq_sig_type == IB_SIGNAL_ALL_WR) ? true:false;
		printk("dwcroce: set_qp_init params end .. \n");//added by hs
}

static int dwcroce_alloc_wr_id_tbl(struct dwcroce_qp* qp)
{
		qp->wqe_wr_id_tbl =
				kcalloc(qp->sq.max_cnt, sizeof(*(qp->wqe_wr_id_tbl)),
						GFP_KERNEL);
			if (qp->wqe_wr_id_tbl == NULL)
					return -ENOMEM;
			qp->rqe_wr_id_tbl =
				kcalloc(qp->rq.max_cnt, sizeof(u64), GFP_KERNEL);
			if (qp->rqe_wr_id_tbl == NULL)
					return -ENOMEM;

			return 0;

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
		if(dev)
			printk("dev exist");//added by hs
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

		/*alloate id for qp,should consider servral situation*/
		if(attrs->qp_type == IB_QPT_SMI)/*In Roce, SM is not supportted*/
			return -EINVAL;
		else if(attrs->qp_type == IB_QPT_GSI)
			{	
			if(!dev->Is_qp1_allocated) 
					{qp_num = 1; dev->Is_qp1_allocated = false;}
			else
				return -EINVAL;
			}		
		else 
			status = dwcroce_alloc_cqqpresource(dev,dev->allocated_qps,dev->attr.max_qp,&qp_num,&dev->next_qp);
		qp->id = qp_num;
		qp->ibqp.qp_num = qp_num;
		printk("dwcroce: create_qp for qp_num is %d\n",qp_num);//added by hs ;
		
		/*kenrel create qp*/
		mutex_lock(&dev->dev_lock); 
		status = dwcroce_hw_create_qp(dev,qp,cq,pd,attrs);
			if (status) {
				kfree(qp);
				return ERR_PTR(status);
		}
					

		dev->qp_table[qp->id] = qp; // store the qp in dev struc.

		/*alloc wr_id table*/
		if (udata == NULL) {
			status = dwcroce_alloc_wr_id_tbl(qp);
			if(status)
				goto map_err;
		}
		mutex_unlock(&dev->dev_lock);
		printk("dwcroce: dwcroce_create_qp succeed end!\n");//added by hs for printing end info
		return &qp->ibqp;
	map_err:
		printk("dwcroce: dwcroce_create_qp map err\n");//added by hs
		dwcroce_destroy_qp(&qp->ibqp);
		return status;
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
		printk("dwcroce:dwcroce_modify_qp qp_state_change\n");//added by hs	
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
			u32 status = 1;
			writel(PGU_BASE + SRCQP,base_addr + MPB_WRITE_ADDR); // INIT PSN
			writel(lqp,base_addr + MPB_RW_DATA);

			writel(PGU_BASE + DESTQP,base_addr + MPB_WRITE_ADDR);
			writel(destqp,base_addr + MPB_RW_DATA);

			writel(PGU_BASE + RC_QPMAPPING,base_addr + MPB_WRITE_ADDR);
			writel(0x1,base_addr + MPB_RW_DATA);
			/*map destqp and srcqp end*/

			while (status != 0)
			{
				writel(PGU_BASE + RC_QPMAPPING,base_addr + MPB_WRITE_ADDR);
				status = readl(base_addr + MPB_RW_DATA);
			}
			printk("dwcroce:rc mapping success lqp:%d rqp:%d\n",lqp,destqp);//added by hs
			u32 wqepagesize = 0;
			u32 cfgenable =0;
			writel(PGU_BASE + GENRSP,base_addr + MPB_WRITE_ADDR);
			wqepagesize = readl(base_addr + MPB_RW_DATA);

			writel(PGU_BASE + CFGRNR,base_addr + MPB_WRITE_ADDR);
			cfgenable =real(base_addr + MPB_RW_DATA);
			if(wqepagesize != 0x00100000)
			{/*start nic*/
				writel(PGU_BASE + GENRSP,base_addr + MPB_WRITE_ADDR);
				writel(0x00100000,base_addr + MPB_RW_DATA);
			}
			if(cfgenable != 0x04010041)
			{	writel(PGU_BASE + CFGRNR,base_addr + MPB_WRITE_ADDR);
				writel(0x04010041,base_addr + MPB_RW_DATA);
				printk("dwcroce:start nic \n");//added by hs
				/*END*/
			}
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
		mutex_lock(&qp->mutex);
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
		mutex_unlock(&qp->mutex);
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
