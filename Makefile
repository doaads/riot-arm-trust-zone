# Default RIOT bootloader
APPLICATION = riotboot_secure

# Include riotboot flash partition functionality
USEMODULE += riotboot_slot
USEPKG += cmsis
#USEMODULE += cmsis_core

# We don't want to re-configure any hardware
CFLAGS += -DDISABLE_BOARD_INIT=1
CFLAGS += -DDISABLE_CPU_INIT=1

# CFLAGS += -DCONFIG_TRUSTZONE
# CFLAGS += -DCONFIG_TRUSTZONE_SECURE

CFLAGS += -mcmse
CFLAGS += -mcpu=cortex-m33
CFLAGS += -Wno-cast-align
CFLAGS += -Wno-error=cast-align

LINKER_SCRIPT = stm32-secure.ld


include riotboot_common.mk
