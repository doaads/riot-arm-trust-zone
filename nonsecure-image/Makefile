# name of your application
APPLICATION = default_ns
BOARD ?= nucleo-l552ze-q
RIOTBASE ?= $(CURDIR)/../../RIOT

# mark this as a non-secure image
CFLAGS += -DTZ_NS_IMAGE

# compile the insecure demo API client functions
CFLAGS += -D__TZ_INSECURE_DEV

RIOT_THREAD_STACKSIZE_MAIN ?= 4096

CFLAGS += -Wno-cast-align
CFLAGS += -Wno-error=cast-align

CLFAGS += -DDEVELHELP

DEVELHELP ?= 1
QUIET ?= 1

# Modules to include:
USEMODULE += shell
USEMODULE += shell_cmds_default
USEMODULE += hashes
USEMODULE += base64

USEPKG += micro-ecc
CFLAGS += -DuECC_OPTIMIZATION_LEVEL=1

include $(RIOTBASE)/Makefile.include

## Set a custom channel if needed
include $(RIOTMAKE)/default-radio-settings.inc.mk

# Get NSC location
SECURE_BUILD_DIR ?= ../secure-image/bin/$(BOARD)/
SECURE_EXPORTS := $(SECURE_BUILD_DIR)/secure_exports.o

LINKER_SCRIPT = ../linker/nonsecure-memory.ld
LINKFLAGS += $(SECURE_EXPORTS)
export OPENOCD_SKIP_VERIFY=1
