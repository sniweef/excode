# Makefile – makefile of our first driver
#  
#  # if KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq (${KERNELRELEASE},)
	obj-m := memory.o
	#     # Otherwise we were called directly from the command line.
	# Invoke the kernel build system.
else
	KERNEL_SOURCE := /usr/src/linux-3.7.10-1-ARCH/
	#PWD := $(pwd)
	PWD := /home/sniweef/programming/c/gtk/os-lab3
default:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} modules

clean:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} clean
endif
