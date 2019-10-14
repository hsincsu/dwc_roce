/*
 *
 * 	this is a header file for address hadle
 * 					----eidted by hs 2019/6/24
 *
 *
 */
#ifndef __DWCROCE_AH_H__
#define __DWCROCE_AH_H__

struct ib_ah *dwcroce_create_ah(struct ib_pd *pd, struct rdma_ah_attr *ah_attr,
		struct ib_udata *udata);
int dwcroce_destroy_ah(struct ib_ah *ah);
int dwcroce_query_ah(struct ib_ah *ah, struct rdma_ah_attr *ah_attr);
int dwcroce_modify_ah(struct ib_ah *ah,struct rdma_ah_attr *ah_attr);

int  dwcroce_process_mad(struct ib_device *,
		int process_mad_flags,
		u8 port_num,
		const struct ib_wc *in_wc,
		const struct ib_grh *in_grh,
		const struct ib_mad_hdr *in, size_t in_mad_size,
		struct ib_mad_hdr *out, size_t *out_mad_size,
		u16 *out_mad_pkey_index);

#endif
