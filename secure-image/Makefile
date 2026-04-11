# Default RIOT bootloader
APPLICATION = riotboot_secure

BOARD = nucleo-l552ze-q

# Include riotboot flash partition functionality
USEMODULE += stdin
USEPKG += cmsis

# crypto packages
USEPKG    += micro-ecc
USEPKG    += wolfssl
USEMODULE += base64
USEMODULE += wolfssl
USEMODULE += wolfcrypt
USEMODULE += wolfcrypt_ecc
USEMODULE += wolfcrypt_coding
USEMODULE += wolfcrypt_rng
USEMODULE += wolfcrypt_sha
USEMODULE += wolfcrypt_asn

# wolfcrypt flags
CFLAGS += -DWOLFSSL_RIOT_OS
CFLAGS += -DWOLFSSL_USER_SETTINGS
CFLAGS += -DHAVE_ECC
CFLAGS += -DHAVE_ECC_SIGN
CFLAGS += -DHAVE_ECC_VERIFY
CFLAGS += -DWOLFSSL_BASE64_ENCODE

#RIOT_THREAD_STACKSIZE_MAIN ?= 4096

CFLAGS += -mcmse
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-m33
CFLAGS += -Wno-cast-align
CFLAGS += -Wno-error=cast-align
CFLAGS += -fno-lto
CFLAGS += -mabi=aapcs

LINKER_SCRIPT = ../linker/secure-memory.ld
LINKFLAGS += -Wl,--cmse-implib,--out-implib=$(BINDIR)/secure_exports.o
LINKFLAGS += -Wl,--just-symbols=$(SECURE_BUILD_DIR)/export.txt

include riotboot_common.mk
