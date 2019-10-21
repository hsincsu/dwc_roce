/*
 *  
 *   
 *    
 *           This is the roce function file in mail driver.I need define some really important struct here#=!
 *      
 *       
 *        
 *         
 *          
 *                                                                 ---------edited by hs in 2019/9/19                                  
 */

#include <linux/mutex.h>
#include <linux/list.h>
//#include <linux/netdevice.h>
#include <linux/module.h>

#include "dwc-xlgmac.h"

/* a list for pdata and dwcintf so that dwc_roce can access the pdata in dwc-xlgmac driver
 *                              --edied by hs                   
 */
struct dwcroce_driver *dwcroce;
static LIST_HEAD(dwcpdata_list);
//static LIST_HEAD(dwcintf_list);
/* end */

/* xlgmac_add_device this funtion add roce driver to dwc-xlgmac
 *
 *                                      --edited by hs
 */
static void xlgmac_add_device(struct xlgmac_pdata *pdata)
{
	struct dwc_dev_info dev_info;
	dev_info.netdev = pdata->netdev;
	dev_info.dev = pdata->dev;
	dev_info.dev_irq = pdata->dev_irq;
	dev_info.mac_addr = pdata->mac_addr;
	dev_info.phy_speed = pdata->phy_speed;
	dev_info.mac_base = pdata->mac_regs;
	dev_info.base_addr = pdata->base_addr;
	dev_info.channel_head = pdata->channel_head;
	pdata->rocedev = dwcroce->add(&dev_info);
	
}

/*xlgmac_del_device(struct xlgmac_pdata *pdata)
 *
 */
static void xlgmac_del_device(struct xlgmac_pdata *pdata)
{
	dwcroce->remove(pdata->rocedev);

}

/* xlgmac_register_dev register pdata struct into dwcpdata_list
 * @xlgmac_pdata *pdata which have some private data about the nic.
 *                                          --edited by hs 
 */
int xlgmac_register_dev(struct xlgmac_pdata *pdata)
{
        list_add_tail(&pdata->list,&dwcpdata_list); // register pdata to dwcpdata_list
	return 0;
}

/* xlgmac_unregister_dev unregister pdata from dwcpdata_list
 *
 */
int xlgmac_unregister_dev(struct xlgmac_pdata *pdata)
{
	list_del(&pdata->list);
	return 0;
}

/* xlgmac_register_interface register interface to dwc-xlgmac driver
 * @xlgmac_interface a interface structure
 *                                        --edited by hs 
 */
int dwc_roce_register_driver(struct dwcroce_driver *drv)
{
        struct xlgmac_pdata *pdata;

		dwcroce = drv;
        list_for_each_entry(pdata,&dwcpdata_list,list)
                xlgmac_add_device(pdata);

        return 0;
}
EXPORT_SYMBOL(dwc_roce_register_driver);

/*dwc_roce_unregister_driver(struct dwcroce_driver *drv)
 *
 */
void dwc_roce_unregister_driver(struct dwcroce_driver *drv)
{
	struct xlgmac_pdata *pdata;
	
	dwcroce = drv;
	list_for_each_entry(pdata,&dwcpdata_list,list)
		xlgmac_del_device(pdata);
}
EXPORT_SYMBOL(dwc_roce_unregister_driver);
