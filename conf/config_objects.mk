ifeq (${SOC}, SOC_ZYNQ)
    include conf/soc_zynq
endif

ifeq (${SOC}, SOC_TI_TM4C1294NCPDT)
    include conf/soc_ti_tm4c1294ncpdt
endif

ifeq (${SOC}, SOC_MK20DX256)
    include  conf/soc_mk20dx256
endif

