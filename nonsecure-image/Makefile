# name of your application
APPLICATION = default_ns
BOARD ?= nucleo-l552ze-q
RIOTBASE ?= $(CURDIR)/../../RIOT

# mark this as a non-secure image
CFLAGS += -DTZ_NS_IMAGE

RIOT_THREAD_STACKSIZE_MAIN ?= 4096

CFLAGS += -mcpu=cortex-m33 -mthumb
CFLAGS += -mcmse
CFLAGS += -Wno-cast-align
CFLAGS += -Wno-error=cast-align

CLFAGS += -DDEVELHELP

#CFLAGS += -DDISABLE_BOARD_INIT=1
#CFLAGS += -DDISABLE_CPU_INIT=1

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
#DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:
USEMODULE += shell
USEMODULE += shell_cmds_default
USEMODULE += hashes
USEMODULE += base64

USEPKG += micro-ecc

DISABLE_MODULE += cortexm_fpu
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
