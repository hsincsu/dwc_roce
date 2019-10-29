/*some definition --hs*/

#ifndef DWCROCE_LOC_H
#define DWCROCE_LOC_H

#define DWCROCE_MAX_MR_INDEX  0x00040000
#define DWCROCE_MIN_MR_INDEX  0x00000001

#define DWCROCE_MIN_QP_INDEX  16
#define DWCROCE_MAX_QP_INDEX  0x00020000

void dwcroce_cq_cleanup(struct dwcroce_pool_entry* arg);
void dwcroce_qp_cleanup(struct dwcroce_pool_entry* arg);
void dwcroce_mem_cleanup(struct dwcroce_pool_entry* arg);

int dwcroce_mem_init_fast(struct dwcroce_pd *pd, int max_pages, struct dwcroce_mr *mr);
int dwcroce_mem_init_dma(struct rxe_pd *pd,int access, struct rxe_mem *mem);

#endif