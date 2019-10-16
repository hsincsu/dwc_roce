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
 *                                                                 ---------edited by hs in 2019/6/19                                  
 */

/* a list for pdata and dwcintf so that dwc_roce can access the pdata in dwc-xlgmac driver
 *                              --edied by hs                   
 */
static LIST_HEAD(dwcpdata_list);
static LIST_HEAD(dwcintf_list);
/* end */

/*xlgmac_register_dev register pdata struct into dwcpdata_list
 *@xlgmac_pdata *pdata which have some private data about the nic.
 *                                          --edited by hs 
 */
int xlgmac_register_dev(struct xlgmac_pdata *pdata)
{
        list_add_tail(&pdata->list,&dwcpdata_list); // register pdata to dwcpdata_list
}



/*xlgmac_register_interface register interface to dwc-xlgmac driver
 *@xlgmac_interface a interface structure
 *                                        --edited by hs 
 */
int xlgmac_register_interface(struct xlgmac_interface *intf)
{
        struct xlgmac_pdata *pdata;

        if(!intf->add && !intf->remove)
                return -ENIVAL;
        list_add_tail(&intr->list,&dwcintf_list);
        list_for_each_entry(pdata,&dwcpdata_list,list)
                xlgmac_add_device(intf,pdata);

        return 0;
}

/*xlgmac_add_device this funtion add roce driver to dwc-xlgmac
 *
 *                                      --edited by hs
 */
int xlgmac_add_device(xlgmac_interface *intf,xlgmac_pdata *pdata);
{

        return 0;
}

