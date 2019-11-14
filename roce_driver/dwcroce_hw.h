/*
*			This header file is to define the operation to hardware
*													--edited by hs
*/

#ifndef  __DWCROCE_HW_H__
#define  __DWCROCE_HW_H__

int dwcroce_init_hw(struct dwcroce_dev *);
void dwcroce_cleanup_hw(struct dwcroce_dev *);
int dwcroce_get_hwinfo(struct dwcroce_dev *);
/*for create cq*/
int dwcroce_hw_create_cq(struct dwcroce_dev *,struct dwcroce_cq *,int entries,u16 pd_id); //alter this later.
int dwcroce_alloc_cqqpresource(struct dwcroce_dev *dev, unsigned long *resource_array, u32 max_resources,u32 *req_resource_num,u32 *next );

int dwcroce_hw_create_qp(struct dwcroce_dev *,struct dwcroce_qp *,struct dwcroce_cq *,struct dwcroce_pd *,struct ib_qp_init_attr *);
#endif		/*__DWCROCE_HW_H__*/	
