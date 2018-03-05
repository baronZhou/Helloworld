#//*/@Droi.zhangyonghua for tee 2018/1/16 start */
ifeq ($(MTK_PLATFORM),MT6739)
PLAT_MT6739 := 1
PLAT_MT6763 := 0
endif
ifeq ($(MTK_PLATFORM),MT6763)
PLAT_MT6739 := 1
PLAT_MT6763 := 0
endif
#//*/@Droi.zhangyonghua for tee 2018/1/16 end */

$(info "teeid.mk:MICROTRUST_TEE_VERSION=$(MICROTRUST_TEE_VERSION)")

TEEID_DIR		:=	services/spd/teeid/251/

SPD_INCLUDES		:=	-I${TEEID_DIR}

SPD_SOURCES		:=	teei_fastcall.c	\
	teei_main.c		\
	teei_helpers.S		\
	teei_common.c	\
	teei_pm.c

vpath %.c ${TEEID_DIR}
vpath %.S ${TEEID_DIR}