/*
 *
 *
 *
 * 	This is the roce function file in mail driver.I need define some really important struct here#=!
 *
 *
 *
 *
 *
 *							---------edited by hs in 2019/6/19
 *
 *					
 */
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/netdevice.h>
#include <linux/module.h>

#include "dwc_roce.h"//get the header file
#include "dwcroce.h"
#define HSDEBUG 1
static struct dwcroce_driver *dwc_drv;
int dwc_roce_register_driver(struct dwcroce_driver *dwc_drv)
{
	//detail function no finished   hs 2019/6/19
	printk("dwcroce:dwc_roce_register_driver start!\n");//added by hs for print errno information
	//waiting to add
#if HSDEBUG //add some function to debug
	printk("dwcroce:get in debug\n");//added by hs for debuging
	struct dwc_dev_info ddinfo;
	dwc_drv->dev = dwc_drv->add(&ddinfo);
#endif
	printk("dwcroce:dwc_roce_unregister_driver succeed end\n");//added by hs for printing info
	return 0;
}					
EXPORT_SYMBOL(dwc_roce_register_driver);	

void dwc_roce_unregister_driver(struct dwcroce_driver *dwc_drv)
{
	//detail function no finished 	hs 2019/6/19
	printk("dwcroce:dwc_roce_unregister_driver start!\n");//added by hs for printing errno info
	//wating to add...
#if HSDEBUG //add some function to debug
	printk("dwcroce:get in debug\n");//added by hs to debug
	if(dwc_drv->dev)
	dwc_drv->remove(dwc_drv->dev);
	else
	printk("all_dev is null");//added to print info
#endif
	printk("dwcroce:dwc_roce_unregister_driver succeed end\n");//added by hs for printing info
}	
EXPORT_SYMBOL(dwc_roce_unregister_driver);
