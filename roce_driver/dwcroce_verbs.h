/*
 *
 *
 *
 * 	this file is a header file for verbs.
 * 					--------edited by hs  2019/6/22
 *
 *
 */
#ifndef __DWCROCE_VERBS_H__
#define __DWCROCE_VERBS_H__

int dwcroce_post_send(struct ib_qp *, struct ib_send_wr *, struct ib_send_wr **bad_wr);

int dwcroce_post_recv(struct ib_qp *, struct ib_recv_wr *, struct ib_recv_wr **bad_wr);

int dwcroce_poll_cq(struct ib_cq *, int num_entries, struct ib_wc *wc);

int dwcroce_arm_cq(struct ib_cq *,enum ib_cq_notify_flags flags);

int dwcroce_query_device(struct ib_device *, struct ib_device_attr *props,
		struct ib_udata *uhw);
int dwcroce_query_port(struct ib_device *, u8 port, struct ib_port_attr *props);
int dwcroce_modify_port(struct ib_device *, u8 port, int mask,
		       struct ib_port_modify *props);

enum rdma_protocol_type
dwcroce_query_protocol(struct ib_device *device, u8 port_num);

void dwcroce_get_guid(struct dwcroce_dev *, u8 *guid);
int dwcroce_query_gid(struct ib_device *, u8 port,
		     int index, union ib_gid *gid);
struct net_device *dwcroce_get_netdev(struct ib_device *device, u8 port_num);
int dwcroce_add_gid(struct ib_device *device,
		   u8 port_num,
		   unsigned int index,
		   const union ib_gid *gid,
		   const struct ib_gid_attr *attr,
		   void **context);
int  dwcroce_del_gid(struct ib_device *device,
		    u8 port_num,
		    unsigned int index,
		    void **context);
int dwcroce_query_pkey(struct ib_device *, u8 port, u16 index, u16 *pkey);

struct ib_ucontext *dwcroce_alloc_ucontext(struct ib_device *,
					  struct ib_udata *);
int dwcroce_dealloc_ucontext(struct ib_ucontext *);

int dwcroce_mmap(struct ib_ucontext *, struct vm_area_struct *vma);

struct ib_pd *dwcroce_alloc_pd(struct ib_device *,
			      struct ib_ucontext *, struct ib_udata *);
int dwcroce_dealloc_pd(struct ib_pd *pd);

struct ib_cq *dwcroce_create_cq(struct ib_device *ibdev,
			       const struct ib_cq_init_attr *attr,
			       struct ib_ucontext *ib_ctx,
			       struct ib_udata *udata);
int dwcroce_resize_cq(struct ib_cq *, int cqe, struct ib_udata *);
int dwcroce_destroy_cq(struct ib_cq *);

struct ib_qp *dwcroce_create_qp(struct ib_pd *,
			       struct ib_qp_init_attr *attrs,
			       struct ib_udata *);
int _dwcroce_modify_qp(struct ib_qp *, struct ib_qp_attr *attr,
		      int attr_mask);
int dwcroce_modify_qp(struct ib_qp *, struct ib_qp_attr *attr,
		     int attr_mask, struct ib_udata *udata);
int dwcroce_query_qp(struct ib_qp *,
		    struct ib_qp_attr *qp_attr,
		    int qp_attr_mask, struct ib_qp_init_attr *);
int dwcroce_destroy_qp(struct ib_qp *);
//void dwcroce_del_flush_qp(struct ocrdma_qp *qp);

struct ib_srq *dwcroce_create_srq(struct ib_pd *, struct ib_srq_init_attr *,
				 struct ib_udata *);
int dwcroce_modify_srq(struct ib_srq *, struct ib_srq_attr *,
		      enum ib_srq_attr_mask, struct ib_udata *);
int dwcroce_query_srq(struct ib_srq *, struct ib_srq_attr *);
int dwcroce_destroy_srq(struct ib_srq *);
int dwcroce_post_srq_recv(struct ib_srq *, struct ib_recv_wr *,
			 struct ib_recv_wr **bad_recv_wr);

int dwcroce_dereg_mr(struct ib_mr *);
struct ib_mr *dwcroce_get_dma_mr(struct ib_pd *, int acc);
struct ib_mr *dwcroce_reg_user_mr(struct ib_pd *, u64 start, u64 length,
				 u64 virt, int acc, struct ib_udata *);
struct ib_mr *dwcroce_alloc_mr(struct ib_pd *pd,
			      enum ib_mr_type mr_type,
			      u32 max_num_sg);
int dwcroce_map_mr_sg(struct ib_mr *ibmr, struct scatterlist *sg, int sg_nents,
		     unsigned int *sg_offset);

#endif
