# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
IOP_BIN  = mechaproxy.irx
IOP_OBJS = mechaproxy.o imports.o exports.o

IOP_SRC_DIR = src/
IOP_OPTFLAGS = -O2
IOP_INCS += -I$(IOP_SRC_DIR)

include $(PS2SDK)/Defs.make
include ../Rules.bin.make
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.iopglobal
