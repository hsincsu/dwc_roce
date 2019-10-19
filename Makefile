# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the Synopsys network device drivers.
#

obj-$(CONFIG_DWC_XLGMAC) += dwc-xlgmac.o
dwc-xlgmac-objs := dwc-xlgmac-net.o dwc-xlgmac-desc.o \
		   dwc-xlgmac-hw.o dwc-xlgmac-common.o \
		   dwc-xlgmac-ethtool.o dwc_roce.o

dwc-xlgmac-$(CONFIG_DWC_XLGMAC_PCI) += dwc-xlgmac-pci.o

KERNELDIR:=/lib/modules/5.0.5-1.el7.elrepo.x86_64/build
PWD:=$(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

