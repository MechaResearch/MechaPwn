EE_BIN = MechaPwn.elf
IRX_DIR = irx/compiled
EE_PACKED_BIN = MechaPwn_pck.elf

# C File
EE_OBJS = main.o mecha.o exploit.o pad.o ui.o mass.o consoleident.o
EE_OBJS += iomanX_irx.o fileXio_irx.o freesio2_irx.o freepad_irx.o mcman_irx.o mcsrv_irx.o USBD_irx.o USBHDFSD_irx.o IndieFlower_irx.o
EE_OBJS += MECHAPROXY_irx.o MASSWATCHER_irx.o pwr50k.o pwr70k.o pwr90k.o pwrpsx1.o pwrpsx2.o pwrtvcombo.o $(addprefix frame_, 001.o 002.o 003.o 004.o 005.o 006.o 007.o 008.o 009.o 010.o 011.o 012.o 013.o 014.o 015.o 016.o 017.o 018.o 019.o 020.o 021.o 022.o 023.o 024.o 025.o 026.o 027.o 028.o 029.o 030.o 031.o 032.o 033.o 034.o 035.o 036.o 037.o 038.o 039.o 040.o 041.o 042.o 043.o 044.o 045.o 046.o 047.o 048.o 049.o 050.o 051.o 052.o 053.o 054.o 055.o 056.o 057.o 058.o 059.o 060.o 061.o 062.o)

EE_INCS = -I$(PS2SDK)/ports/include -I$(PS2SDK)/sbv/include -I$(PS2SDK)/common/include -I./irx/source/mechaproxy/include/ -I./irx/source/masswatcher/include/ -I$(PS2SDK)/ports/include/freetype2 -I$(GSKIT)/include
EE_LDFLAGS = -L$(PS2SDK)/sbv/lib -L$(PS2SDK)/ports/lib -L$(GSKIT)/lib
EE_LIBS = -lpatches -lpadx -lmc -lfreetype -lpng -lz -lgskit -lgskit_toolkit -ldmakit -lfileXio
EE_CFLAGS = -std=c99

BIN2C = $(PS2SDK)/bin/bin2c

all: $(EE_BIN)
	ps2-packer $(EE_BIN) $(EE_PACKED_BIN)

clean:
	make clean -C irx/source/mechaproxy
	make clean -C irx/source/masswatcher
	rm -f *.elf *.o *_irx.c *.s resources/*.bin resources/*.pyc *.pyc

#IRX Modules
iomanX_irx.c:
	$(BIN2C) $(PS2SDK)/iop/irx/iomanX.irx iomanX_irx.c iomanX
fileXio_irx.c:
	$(BIN2C) $(PS2SDK)/iop/irx/fileXio.irx fileXio_irx.c fileXio
freesio2_irx.c:
	$(BIN2C) $(PS2SDK)/iop/irx/freesio2.irx freesio2_irx.c freesio2
freepad_irx.c:
	$(BIN2C) $(PS2SDK)/iop/irx/freepad.irx freepad_irx.c freepad
mcman_irx.c:
	$(BIN2C) $(PS2SDK)/iop/irx/mcman.irx mcman_irx.c mcman
mcsrv_irx.c:
	$(BIN2C) $(PS2SDK)/iop/irx/mcserv.irx mcsrv_irx.c mcserv
USBD_irx.c: $(PS2SDK)/iop/irx/usbd.irx
	$(BIN2C) $(PS2SDK)/iop/irx/usbd.irx USBD_irx.c USBD
USBHDFSD_irx.c: $(PS2SDK)/iop/irx/usbhdfsd.irx
	$(BIN2C) $(PS2SDK)/iop/irx/usbhdfsd.irx USBHDFSD_irx.c USBHDFSD
MECHAPROXY_irx.c:
	$(MAKE) -C irx/source/mechaproxy
	$(BIN2C) irx/source/mechaproxy/irx/mechaproxy.irx MECHAPROXY_irx.c MECHAPROXY_irx
MASSWATCHER_irx.c:
	$(MAKE) -C irx/source/masswatcher
	$(BIN2C) irx/source/masswatcher/irx/masswatcher.irx MASSWATCHER_irx.c MASSWATCHER_irx
IndieFlower_irx.c: resources/IndieFlower-Regular.ttf
	$(BIN2C) resources/IndieFlower-Regular.ttf IndieFlower_irx.c IndieFlower

%.c: resources/%
	$(BIN2C) $^ $@ $(^F)

resources/%: resources/%.png
	python3 resources/conv.py $^ $@

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
