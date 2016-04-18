ifeq (${SOC}, SOC_ZYNQ)
    include conf/arm-a9.mk
endif

ifeq (${SOC}, SOC_TI_TM4C1294NCPDT)
    include conf/arm-m4f.mk
endif

ifeq (${SOC}, SOC_MK20DX256)
    include conf/arm-m4.mk
endif

