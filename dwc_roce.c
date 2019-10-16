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

	pdata->rocedev = dwcroce->add(&dev_info);
	
}

/* xlgmac_register_dev register pdata struct into dwcpdata_list
 * @xlgmac_pdata *pdata which have some private data about the nic.
 *                                          --edited by hs 
 */
int xlgmac_register_dev(struct xlgmac_pdata *pdata)
{
        list_add_tail(&pdata->list,&dwcpdata_list); // register pdata to dwcpdata_list
}



/* xlgmac_register_interface register interface to dwc-xlgmac driver
 * @xlgmac_interface a interface structure
 *                                        --edited by hs 
 */
int dwc_roce_register_driver(struct dwcroce_driver* drv)
{
        struct xlgmac_pdata *pdata;

		dwcroce = drv;
        list_for_each_entry(pdata,&dwcpdata_list,list)
                xlgmac_add_device(pdata);

        return 0;
}


