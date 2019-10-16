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

#include <rdma/ib_verbs.h>
#include <rdma/ib_user_verbs.h>
#include <rdma/ib_addr.h>

//#include "../dwc_roce.h"//  hs 2019/6/19
#include "../dwc-xlgmac.h"
#include "../dwc-xlgmac-reg.h"
#define DWCROCEDRV_VER "1.0.0.0"

struct dwcroce_dev{
	struct ib_device ibdev;
	//not finished ,added later.
};






#endif
