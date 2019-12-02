/*
 *
 * 	this file for address handle
 * 				eidted by hs 2019/6/24
 *
 *
 */

#include <net/neighbour.h>
#include <net/netevent.h>

#include <rdma/ib_addr.h>
#include <rdma/ib_mad.h>
#include <rdma/ib_cache.h>

#include "dwcroce.h"
#include "dwcroce_verbs.h"
#include "dwcroce_ah.h"

struct ib_ah *dwcroce_create_ah(struct ib_pd *pd, struct rdma_ah_attr *ah_attr,u32 flags,
		struct ib_udata *udata)
{
	printk("dwcroce:dwcroce_create_ah start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_create_ah succeed end!\n");//added by hs for printing end info
	return NULL;
}

int dwcroce_destroy_ah(struct ib_ah *ah,u32 flags)
{
	printk("dwcroce:dwcroce_destroy_ah start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_destroy_ah succeed end!\n");//added by hs for printing end info
	return 0;
}

int dwcroce_query_ah(struct ib_ah *ah, struct rdma_ah_attr *ah_attr)
{
	printk("dwcroce:dwcroce_query_ah start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_query_ah succeed end!\n");//added by hs for printing end info
	return 0;
}

int dwcroce_modify_ah(struct ib_ah *ah,struct rdma_ah_attr *ah_attr)
{
	printk("dwcroce:dwcroce_modify_ah start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_modify_ah succeed end!\n");//added by hs for printing end info
	return 0;
}

int  dwcroce_process_mad(struct ib_device *ibdev,
		int process_mad_flags,
		u8 port_num,
		const struct ib_wc *in_wc,
		const struct ib_grh *in_grh,
		const struct ib_mad_hdr *in, size_t in_mad_size,
		struct ib_mad_hdr *out, size_t *out_mad_size,
		u16 *out_mad_pkey_index)
{
	printk("dwcroce:dwcroce_process_mad start!\n");//added by hs for printing start info
	/*wait to add 2019/6/24*/

	/*wait to add end!*/	
	printk("dwcroce:dwcroce_process_mad succeed end!\n");//added by hs for printing end info
	return 0;
}


 				
