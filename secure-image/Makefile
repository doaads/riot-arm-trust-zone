# Default RIOT bootloader
APPLICATION = riotboot_secure

BOARD = nucleo-l552ze-q

# Include riotboot flash partition functionality
USEMODULE += stdin
USEPKG += cmsis

# crypto packages
USEPKG    += micro-ecc
USEMODULE += base64
USEMODULE += hashes

#RIOT_THREAD_STACKSIZE_MAIN ?= 4096

CFLAGS += -DuECC_OPTIMIZATION_LEVEL=1

CFLAGS += -mcmse
#CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-m33
CFLAGS += -Wno-cast-align
CFLAGS += -Wno-error=cast-align
CFLAGS += -fno-lto
CFLAGS += -mabi=aapcs

LINKER_SCRIPT = ../linker/secure-memory.ld
LINKFLAGS += -Wl,--cmse-implib,--out-implib=$(BINDIR)/secure_exports.o
LINKFLAGS += -Wl,--just-symbols=$(SECURE_BUILD_DIR)/export.txt

# include security definitions
include tz_security_attribution.mk

include riotboot_common.mk
