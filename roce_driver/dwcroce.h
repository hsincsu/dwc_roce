/*
 *
 *  This is a main header file of the driver.Important struct start here.
 *
 *
 *
 *  								------------edited by hs in 2019/6/18
 *
 *
 */
#ifndef __DWCROCE_H__
#define __DWCROCE_H__

#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

#include <rdma/ib_verbs.h>
#include <rdma/ib_user_verbs.h>
#include <rdma/ib_addr.h>

//#include "../dwc_roce.h"//  hs 2019/6/19
#include "../dwc-xlgmac.h"
#include "../dwc-xlgmac-reg.h"
#include "dwcroce_pool.h"
#include "dwcroce_loc.h"
#define DWCROCEDRV_VER "1.0.0.0"

struct dwcroce_dev{
	struct ib_device ibdev;
	struct ib_device_attr attr;
	struct dwc_dev_info devinfo;
//	unsigned long *pd_id; // for allocate an unique id to each pd.
	struct mutex pd_mutex;
	//not finished ,added later.

	struct dwcroce_pool mr_pool;
	struct dwcroce_pool pd_pool;
};

struct dwcroce_pd {
	struct dwcroce_pool_entry pelem;
	struct ib_pd ibpd;
	u32 id;

};

struct dwcroce_cq {
	struct ib_cq ibcq;
        u32 phase;
        u32 getp;

        u32 max_hw_cqe;
        u32 cqe_cnt;
        u32 len;
        spinlock_t lock; //for serialize accessing to the CQ
        struct list_head sq_head, rq_head;
 

};


struct dwcroce_mr {
	struct dwcroce_pool_entry pelem;
	union{
		struct ib_mr ibmr;
		struct ib_mw ibmw;
	};
	struct dwcroce_pd *pd;
	struct ib_umem	  *umem;

	u32				 lkey;
	u32				 rkey;

	enum dwcroce_mem_state	state;
	enum dwcroce_mem_type	type;
	u64				 va;
	u64				 iova;
	size_t			 length;
	u32				 offset;
	int				 access;

	int				 page_shift;
	int				 page_mask;
	int				 map_shift;
	int				 map_mask;

	u32				 num_buf;
	u32				 nbuf;

	u32				 max_buf;
	u32				 num_map;

	struct dwcroce_map  **map;
};

struct dwcroce_qp {
	struct ib_qp ibqp;
};
static inline struct dwcroce_dev *get_dwcroce_dev(struct ib_device *ibdev)
{
	return container_of(ibdev, struct dwcroce_dev, ibdev);
}

static inline struct dwcroce_pd *get_dwcroce_pd(struct ib_pd *ibpd)
{
	return container_of(ibpd, struct dwcroce_pd, ibpd);
}

static inline struct dwcroce_cq *get_dwcroce_cq(struct ib_cq *ibcq)
{
	return container_of(ibcq, struct dwcroce_cq, ibcq);
}

static inline struct dwcroce_mr *get_dwcroce_mr(struct ib_mr *ibmr)
{
	return container_of(ibmr, struct dwcroce_mr, ibmr);
}

static inline struct dwcroce_qp *get_dwcroce_qp(struct ib_qp *ibqp)
{
	return container_of(ibqp,struct dwcroce_qp, ibqp);
}


#endif
