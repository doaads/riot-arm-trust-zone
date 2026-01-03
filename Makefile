# Default RIOT bootloader
APPLICATION = riotboot_secure

BOARD = nucleo-l552ze-q

# Include riotboot flash partition functionality
USEMODULE += riotboot_slot
USEPKG += cmsis
#USEMODULE += cmsis_core

# We don't want to re-configure any hardware
#CFLAGS += -DDISABLE_BOARD_INIT=1
#CFLAGS += -DDISABLE_CPU_INIT=1

RIOT_THREAD_STACKSIZE_MAIN ?= 4096

CFLAGS += -mcmse
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-m33
CFLAGS += -Wno-cast-align
CFLAGS += -Wno-error=cast-align
CFLAGS += -fno-lto

LINKER_SCRIPT = linker/secure-memory.ld
LINKFLAGS += -Wl,--cmse-implib,--out-implib=$(BINDIR)/secure_exports.o
LINKFLAGS += -Wl,--just-symbols=$(SECURE_BUILD_DIR)/export.txt

include riotboot_common.mk
