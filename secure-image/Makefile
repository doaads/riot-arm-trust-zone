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

CFLAGS += -DuECC_OPTIMIZATION_LEVEL=1

CFLAGS += -Wno-cast-align
CFLAGS += -Wno-error=cast-align
CFLAGS += -mcmse

# development flag only
CFLAGS += -D__TZ_INSECURE_DEV

LINKER_SCRIPT = ../linker/secure-memory.ld
LINKFLAGS += -Wl,--cmse-implib,--out-implib=$(BINDIR)/secure_exports.o
LINKFLAGS += -Wl,--just-symbols=$(SECURE_BUILD_DIR)/export.txt

NON_SECURE_FLASH_ADDR ?= 0x08040000
NON_SECURE_FLASH_END  ?= 0x0807FFFF
NON_SECURE_SRAM_ADDR  ?= 0x20030000
NON_SECURE_SRAM_END   ?= 0x2003FFFF
NSC_ADDR              ?= 0x08030000

# include security definitions
include tz_security_attribution.mk

include riotboot_common.mk
