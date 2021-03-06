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

#define  DWCROCE_MIN_Q_PAGE_SIZE	4096
#define  DWCROCE_AV_VALID			BIT(7)
#define  DWCROCE_AV_VLAN_VALID		BIT(1)
struct dwcroce_pbe {
	u32 pa_hi;
	u32 pa_lo;
};

struct dwcroce_eth_basic {
		u8 dmac[6];
        u8 smac[6];
        __be16 eth_type;
};
struct dwcroce_eth_vlan {
		u8 dmac[6];
        u8 smac[6];
        __be16 eth_type;
        __be16 vlan_tag;
        __be16 roce_eth_type;
};


struct dwcroce_grh {
		__be32  tclass_flow;
        __be32  pdid_hoplimit;
        u8      sgid[16];
        u8      dgid[16];      
        u16     rsvd;
};

struct dwcroce_av {
	 struct dwcroce_eth_vlan eth_hdr;
	 struct dwcroce_grh grh;
	 u32 valid;
};

struct dwcroce_ah {
	struct ib_ah	ibah;
	struct dwcroce_av *av;
	u16 sgid_index;
	u32 id;
	u8 hdr_type;
};

enum dwcroce_qp_foe {
        DWCROCE_Q_FULL = 0,
        DWCROCE_Q_EMPTY = 1,
        DWCROCE_Q_FREE = 2,
};

struct dwcroce_pd {
	struct dwcroce_pool_entry pelem;
	struct ib_pd ibpd;
	u32 id;

};
struct dwcroce_cqe { // cqe need 16 byte memory.
	u32 cqe_p1;//4 byte
	u32 cqe_p2;//4 byte
	u32 cqe_p3;//4 byte
	u32 cqe_p4;//4 byte
};

struct dwcroce_cq {
	struct ib_cq ibcq;
	/*three types of cq,tx,rx,xmit*/
	struct dwcroce_cqe *txva;
	struct dwcroce_cqe *rxva;
	struct dwcroce_cqe *xmitva;
	/*wp ,rp for three types of cq*/
	struct dwcroce_cqe *txwp;
	struct dwcroce_cqe *txrp;
	struct dwcroce_cqe *rxwp;
	struct dwcroce_cqe *rxrp;
	struct dwcroce_cqe *xmitwp;
	struct dwcroce_cqe *xmitrp;

	dma_addr_t txpa;
	dma_addr_t rxpa;
	dma_addr_t xmitpa;
		
		u32 phase;
        u32 getp;
		u32 max_hw_cqe;

		u32 id; // allocate a unique id for cq.
        u32 cqe_cnt;//cqe count
        u32 len; // cq's len
        spinlock_t lock; //for serialize accessing to the CQ
        struct list_head sq_head, rq_head, xmit_head;

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

struct dwcroce_qp_hwq_info {
	u8 *va;
	u32 max_sges;
	u32 head,tail;
	u32 entry_size;
	u32 max_cnt;
	u32 max_wqe_idx;
	//u16 dbid;
	u32 len;
	dma_addr_t pa;
	enum dwcroce_qp_foe qp_foe;
	spinlock_t lock;
};

struct dwcroce_sge {
	u32 addr_hi;
	u32 addr_lo;
	u32 lrkey;
	u32 len;
};
struct dwcroce_rqe {
	u64  descbaseaddr;
	u32  dmalen;
	u32  opcode;
};

#pragma pack(1)//we don't need the default align,we need to make sure the wqe is 48 bytes.
struct dwcroce_wqe {//defaultly,we use 48 byte WQE.a queue may have 256 wqes. 48 bytes long,but length is 64 bytes.
	u32 immdt;
	u16 pkey;
	u32 rkey;
	u32 lkey;
	u32 qkey;
	u32 dmalen;
	u64 destaddr;
	u64 localaddr;
	u16 eecntx; // just the first 10 bits is for eecntx,the later 6bits is for destqp;
	u16 destqp; //just the first 4 bits is for destqp.the later 12 bits is for destsocket1.
	u32 destsocket1;
	u8 destsocket2;//just the first 4 bits is for destsocket2,the later 4 bits is for opcode.
	u8  opcode; // just the first 4 bits is for opcode .the later 4 bits is useless.
	u64 reserved1;
	u64 reserved2; // 
};
#pragma pack()

enum dwcroce_qp_state {
	DWCROCE_QPS_RST				=0,
	DWCROCE_QPS_INIT			=1,
	DWCROCE_QPS_RTR				=2,
	DWCROCE_QPS_RTS				=3,
	DWCROCE_QPS_SQE				=4,
	DWCROCE_QPS_SQ_DRAINING			=5,
	DWCROCE_QPS_ERR				=6,
	DWCROCE_QPS_SQD				=7,

};

struct dwcroce_qp {
	struct ib_qp ibqp;
	
	u32 id; // qp unique id.
	u32 len; // qp len.send queue is same to recv queue.
	u32 max_inline_data;
	struct {
		uint64_t wrid;
		uint16_t dpp_wqe_idx;
		uint16_t dpp_wqe;
		uint8_t  signaled;
		uint8_t  rsvd[3];
	} *wqe_wr_id_tbl;
	u64 *rqe_wr_id_tbl;

	struct dwcroce_cq *cq; 
	struct dwcroce_pd *pd;

	struct dwcroce_qp_hwq_info rq;
	struct dwcroce_qp_hwq_info sq;

	enum ib_qp_type qp_type;
	enum dwcroce_qp_state qp_state;
	
	u32 qkey;
	bool signaled;
	u32 destqp;
	u32 pkey_index;
	int sgid_idx;
	struct mutex mutex;
	u8 mac_addr[6]; // dest mac addr


};

struct dwcroce_pbl {
	void *va;
	dma_addr_t pa;
	u32 size;
};

struct dwcroce_dev{
	struct ib_device ibdev;
	struct ib_device_attr attr;
	struct dwc_dev_info devinfo;
//	unsigned long *pd_id; // for allocate an unique id to each pd.
	struct mutex pd_mutex;
	struct mutex dev_lock; 
	//not finished ,added later.

	struct dwcroce_pool mr_pool;
	struct dwcroce_pool pd_pool;
	u8 *mem_resources; // for bitmap memory
	unsigned long *allocated_cqs; // allocate id for cqs
	unsigned long *allocated_qps;//allocated id for qps
	struct dwcroce_qp **qp_table;
	struct dwcroce_cq **cq_table;

	struct {
		struct dwcroce_av *va;
		dma_addr_t pa;
		u32 size;
		u32 num_ah;

		spinlock_t lock;/*for synchronization*/
		u32 ahid;
		struct dwcroce_pbl pbl;
	} av_tbl;

	u32 next_cq;
	u32 next_qp;
	u32	used_cqs;
	u32 used_qps;

	spinlock_t resource_lock; //for cq,qp resource access
	spinlock_t qptable_lock;

	bool Is_qp1_allocated;
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

static inline struct dwcroce_ah *get_dwcroce_ah(struct ib_ah* ibah)
{
	return container_of(ibah,struct dwcroce_ah, ibah);
}

int dwcroce_mem_init_fast(struct dwcroce_pd *dwcpd, int max_pages, struct dwcroce_mr *dwcmr);

int dwcroce_mem_init_dma(struct dwcroce_pd *dwcpd,int access, struct dwcroce_mr *dwcmr);


#endif
