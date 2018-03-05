
$(info "teeid.mk:MICROTRUST_TEE_VERSION=$(MICROTRUST_TEE_VERSION)")

MICRO_PLATFORM_VERSION_MAJOR := $(shell grep -oP "^[ \t]*PLATFORM_VERSION.*:= \K[\d.]+" $(ANDROID_BUILD_TOP)/build/core/version_defaults.mk | cut -d . -f 1)
CFLAGS += -DMICRO_PLATFORM_VERSION_MAJOR=$(MICRO_PLATFORM_VERSION_MAJOR)

TEEID_DIR		:=	services/spd/teeid/270/

SPD_INCLUDES		:=	-I${TEEID_DIR}

SPD_SOURCES		:=	teei_fastcall.c	\
	teei_main.c		\
	teei_helpers.S		\
	teei_common.c	\
	teei_pm.c

vpath %.c ${TEEID_DIR}
vpath %.S ${TEEID_DIR}
