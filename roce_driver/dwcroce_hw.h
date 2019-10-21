/*
*			This header file is to define the operation to hardware
*													--edited by hs
*/

#ifndef  __DWCROCE_HW_H__
#define  __DWCROCE_HW_H__

int dwcroce_init_hw(struct dwcroce_dev *);
void dwcroce_cleanup_hw(struct dwcroce_dev *);
int dwcroce_get_hwinfo(struct dwcroce_dev *);

#endif		/*__DWCROCE_HW_H__*/	