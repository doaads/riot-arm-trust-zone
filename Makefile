# Default RIOT bootloader
APPLICATION = riotboot_secure

BOARD = nucleo-l552ze-q

# Include riotboot flash partition functionality
USEMODULE += riotboot_slot
USEMODULE += stdin
USEMODULE += ps
USEPKG += cmsis

RIOT_THREAD_STACKSIZE_MAIN ?= 4096

CFLAGS += -mcmse
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-m33
CFLAGS += -Wno-cast-align
CFLAGS += -Wno-error=cast-align
CFLAGS += -fno-lto
CFLAGS += -mabi=aapcs

LINKER_SCRIPT = linker/secure-memory.ld
LINKFLAGS += -Wl,--cmse-implib,--out-implib=$(BINDIR)/secure_exports.o
LINKFLAGS += -Wl,--just-symbols=$(SECURE_BUILD_DIR)/export.txt

include riotboot_common.mk
