# name of your application
APPLICATION = default_ns

# If no BOARD is found in the environment, use this default:
BOARD ?= nucleo-l552ze-q

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT

# mark this as a non-secure image (skip some peripherial inits)
CFLAGS += -DTZ_NS_IMAGE

CFLAGS += -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard
CFLAGS += -mcmse
CFLAGS += -Wno-cast-align
CFLAGS += -Wno-error=cast-align

# Secure image configures the board and CPU
CFLAGS += -DDISABLE_BOARD_INIT=1
CFLAGS += -DDISABLE_CPU_INIT=1

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
#DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 0

# Modules to include:
USEMODULE += shell
USEMODULE += shell_cmds_default
#USEMODULE += ps

# include and auto-initialize all available sensors
#USEMODULE += saul_default
#FEATURES_OPTIONAL += netif
#FEATURES_OPTIONAL += periph_rtc

include $(RIOTBASE)/Makefile.include

## Set a custom channel if needed
include $(RIOTMAKE)/default-radio-settings.inc.mk

# Get NSC location
SECURE_BUILD_DIR ?= ../secure-image/bin/$(BOARD)/
SECURE_EXPORTS := $(SECURE_BUILD_DIR)/secure_exports.o

LINKER_SCRIPT = ../linker/nonsecure-memory.ld
LINKFLAGS += $(SECURE_EXPORTS)
