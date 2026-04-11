#!/bin/sh

arm-none-eabi-gdb secure-image/bin/nucleo-l552ze-q/riotboot_secure.elf -x gdb_cmds.gdb

