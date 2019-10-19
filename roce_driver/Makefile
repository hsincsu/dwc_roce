obj-m += dwcroce.o

dwcroce-y := dwcroce_main.o dwcroce_verbs.o dwcroce_ah.o




KERNELDIR:=/lib/modules/5.0.5-1.el7.elrepo.x86_64/build
PWD:=$(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

