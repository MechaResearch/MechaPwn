EE_BIN = MechaPwn.elf
IRX_DIR = irx/compiled
EE_PACKED_BIN = MechaPwn_pck.elf

# C File
EE_OBJS = main.o mecha.o exploit.o pad.o ui.o mass.o
EE_OBJS += iomanX.o fileXio.o freesio2.o freepad.o mcman.o mcsrv.o USBD.o USBHDFSD.o IndieFlower.o
EE_OBJS += MECHAPROXY_irx.o MASSWATCHER_irx.o pwr50k.o pwr70k.o pwr90k.o frame_001.o frame_002.o frame_003.o frame_004.o frame_005.o frame_006.o frame_007.o frame_008.o frame_009.o frame_010.o frame_011.o frame_012.o frame_013.o frame_014.o frame_015.o frame_016.o frame_017.o frame_018.o frame_019.o frame_020.o frame_021.o frame_022.o frame_023.o frame_024.o frame_025.o frame_026.o frame_027.o frame_028.o frame_029.o frame_030.o frame_031.o frame_032.o frame_033.o frame_034.o frame_035.o frame_036.o frame_037.o frame_038.o frame_039.o frame_040.o frame_041.o frame_042.o frame_043.o frame_044.o frame_045.o frame_046.o frame_047.o frame_048.o frame_049.o frame_050.o frame_051.o frame_052.o frame_053.o frame_054.o frame_055.o frame_056.o frame_057.o frame_058.o frame_059.o frame_060.o frame_061.o frame_062.o

EE_INCS = -I$(PS2SDK)/ports/include -I$(PS2SDK)/sbv/include -I$(PS2SDK)/common/include -I./irx/source/mechaproxy/include/ -I./irx/source/masswatcher/include/ -I$(PS2SDK)/ports/include/freetype2 -I$(GSKIT)/include
EE_LDFLAGS = -L$(PS2SDK)/sbv/lib -L$(PS2SDK)/ports/lib -L$(GSKIT)/lib
EE_LIBS = -lpatches -lpadx -lmc -lfreetype -lpng16 -lzlib -lgskit -lgskit_toolkit -ldmakit -lfileXio
EE_CFLAGS = -std=c99

BIN2S = $(PS2SDK)/bin/bin2s

all: $(EE_BIN)
	ps2-packer $(EE_BIN) $(EE_PACKED_BIN)
	rm -rf *.o *.s resources/*.bin resources/*.pyc *.pyc

clean:
	make clean -C irx/source/mechaproxy
	make clean -C irx/source/masswatcher
	rm -f *.elf *.o *.s resources/*.bin resources/*.pyc *.pyc
	
#IRX Modules
iomanX.s:
	$(BIN2S) $(PS2SDK)/iop/irx/iomanX.irx iomanX.s iomanX
fileXio.s:
	$(BIN2S) $(PS2SDK)/iop/irx/fileXio.irx fileXio.s fileXio
freesio2.s:
	$(BIN2S) $(PS2SDK)/iop/irx/freesio2.irx freesio2.s freesio2
freepad.s:
	$(BIN2S) $(PS2SDK)/iop/irx/freepad.irx freepad.s freepad
mcman.s:
	$(BIN2S) $(PS2SDK)/iop/irx/mcman.irx mcman.s mcman
mcsrv.s:
	$(BIN2S) $(PS2SDK)/iop/irx/mcserv.irx mcsrv.s mcserv
USBD.s: $(PS2SDK)/iop/irx/usbd.irx
	$(BIN2S) $(PS2SDK)/iop/irx/usbd.irx USBD.s USBD
USBHDFSD.s: $(PS2SDK)/iop/irx/usbhdfsd.irx
	$(BIN2S) $(PS2SDK)/iop/irx/usbhdfsd.irx USBHDFSD.s USBHDFSD
MECHAPROXY_irx.s: 
	$(MAKE) -C irx/source/mechaproxy
	$(BIN2S) irx/source/mechaproxy/irx/mechaproxy.irx MECHAPROXY_irx.s MECHAPROXY_irx
MASSWATCHER_irx.s:
	$(MAKE) -C irx/source/masswatcher
	$(BIN2S) irx/source/masswatcher/irx/masswatcher.irx MASSWATCHER_irx.s MASSWATCHER_irx
IndieFlower.s: resources/IndieFlower-Regular.ttf
	$(BIN2S) resources/IndieFlower-Regular.ttf IndieFlower.s IndieFlower
resources/50k.bin: resources/50k.png
	python resources/conv.py resources/50k.png resources/50k.bin
pwr50k.s: resources/50k.bin
	$(BIN2S) resources/50k.bin pwr50k.s pwr50k
resources/70k.bin: resources/70k.png
	python resources/conv.py resources/70k.png resources/70k.bin
pwr70k.s: resources/70k.bin
	$(BIN2S) resources/70k.bin pwr70k.s pwr70k
resources/90k.bin: resources/90k.png
	python resources/conv.py resources/90k.png resources/90k.bin
pwr90k.s: resources/90k.bin
	$(BIN2S) resources/90k.bin pwr90k.s pwr90k

resources/frame_001.bin: resources/frame_001.png
	python resources/conv.py resources/frame_001.png resources/frame_001.bin
frame_001.s: resources/frame_001.bin
	$(BIN2S) resources/frame_001.bin frame_001.s frame_001
resources/frame_002.bin: resources/frame_002.png
	python resources/conv.py resources/frame_002.png resources/frame_002.bin
frame_002.s: resources/frame_002.bin
	$(BIN2S) resources/frame_002.bin frame_002.s frame_002
resources/frame_003.bin: resources/frame_003.png
	python resources/conv.py resources/frame_003.png resources/frame_003.bin
frame_003.s: resources/frame_003.bin
	$(BIN2S) resources/frame_003.bin frame_003.s frame_003
resources/frame_004.bin: resources/frame_004.png
	python resources/conv.py resources/frame_004.png resources/frame_004.bin
frame_004.s: resources/frame_004.bin
	$(BIN2S) resources/frame_004.bin frame_004.s frame_004
resources/frame_005.bin: resources/frame_005.png
	python resources/conv.py resources/frame_005.png resources/frame_005.bin
frame_005.s: resources/frame_005.bin
	$(BIN2S) resources/frame_005.bin frame_005.s frame_005
resources/frame_006.bin: resources/frame_006.png
	python resources/conv.py resources/frame_006.png resources/frame_006.bin
frame_006.s: resources/frame_006.bin
	$(BIN2S) resources/frame_006.bin frame_006.s frame_006
resources/frame_007.bin: resources/frame_007.png
	python resources/conv.py resources/frame_007.png resources/frame_007.bin
frame_007.s: resources/frame_007.bin
	$(BIN2S) resources/frame_007.bin frame_007.s frame_007
resources/frame_008.bin: resources/frame_008.png
	python resources/conv.py resources/frame_008.png resources/frame_008.bin
frame_008.s: resources/frame_008.bin
	$(BIN2S) resources/frame_008.bin frame_008.s frame_008
resources/frame_009.bin: resources/frame_009.png
	python resources/conv.py resources/frame_009.png resources/frame_009.bin
frame_009.s: resources/frame_009.bin
	$(BIN2S) resources/frame_009.bin frame_009.s frame_009
resources/frame_010.bin: resources/frame_010.png
	python resources/conv.py resources/frame_010.png resources/frame_010.bin
frame_010.s: resources/frame_010.bin
	$(BIN2S) resources/frame_010.bin frame_010.s frame_010
resources/frame_011.bin: resources/frame_011.png
	python resources/conv.py resources/frame_011.png resources/frame_011.bin
frame_011.s: resources/frame_011.bin
	$(BIN2S) resources/frame_011.bin frame_011.s frame_011
resources/frame_012.bin: resources/frame_012.png
	python resources/conv.py resources/frame_012.png resources/frame_012.bin
frame_012.s: resources/frame_012.bin
	$(BIN2S) resources/frame_012.bin frame_012.s frame_012
resources/frame_013.bin: resources/frame_013.png
	python resources/conv.py resources/frame_013.png resources/frame_013.bin
frame_013.s: resources/frame_013.bin
	$(BIN2S) resources/frame_013.bin frame_013.s frame_013
resources/frame_014.bin: resources/frame_014.png
	python resources/conv.py resources/frame_014.png resources/frame_014.bin
frame_014.s: resources/frame_014.bin
	$(BIN2S) resources/frame_014.bin frame_014.s frame_014
resources/frame_015.bin: resources/frame_015.png
	python resources/conv.py resources/frame_015.png resources/frame_015.bin
frame_015.s: resources/frame_015.bin
	$(BIN2S) resources/frame_015.bin frame_015.s frame_015
resources/frame_016.bin: resources/frame_016.png
	python resources/conv.py resources/frame_016.png resources/frame_016.bin
frame_016.s: resources/frame_016.bin
	$(BIN2S) resources/frame_016.bin frame_016.s frame_016
resources/frame_017.bin: resources/frame_017.png
	python resources/conv.py resources/frame_017.png resources/frame_017.bin
frame_017.s: resources/frame_017.bin
	$(BIN2S) resources/frame_017.bin frame_017.s frame_017
resources/frame_018.bin: resources/frame_018.png
	python resources/conv.py resources/frame_018.png resources/frame_018.bin
frame_018.s: resources/frame_018.bin
	$(BIN2S) resources/frame_018.bin frame_018.s frame_018
resources/frame_019.bin: resources/frame_019.png
	python resources/conv.py resources/frame_019.png resources/frame_019.bin
frame_019.s: resources/frame_019.bin
	$(BIN2S) resources/frame_019.bin frame_019.s frame_019
resources/frame_020.bin: resources/frame_020.png
	python resources/conv.py resources/frame_020.png resources/frame_020.bin
frame_020.s: resources/frame_020.bin
	$(BIN2S) resources/frame_020.bin frame_020.s frame_020
resources/frame_021.bin: resources/frame_021.png
	python resources/conv.py resources/frame_021.png resources/frame_021.bin
frame_021.s: resources/frame_021.bin
	$(BIN2S) resources/frame_021.bin frame_021.s frame_021
resources/frame_022.bin: resources/frame_022.png
	python resources/conv.py resources/frame_022.png resources/frame_022.bin
frame_022.s: resources/frame_022.bin
	$(BIN2S) resources/frame_022.bin frame_022.s frame_022
resources/frame_023.bin: resources/frame_023.png
	python resources/conv.py resources/frame_023.png resources/frame_023.bin
frame_023.s: resources/frame_023.bin
	$(BIN2S) resources/frame_023.bin frame_023.s frame_023
resources/frame_024.bin: resources/frame_024.png
	python resources/conv.py resources/frame_024.png resources/frame_024.bin
frame_024.s: resources/frame_024.bin
	$(BIN2S) resources/frame_024.bin frame_024.s frame_024
resources/frame_025.bin: resources/frame_025.png
	python resources/conv.py resources/frame_025.png resources/frame_025.bin
frame_025.s: resources/frame_025.bin
	$(BIN2S) resources/frame_025.bin frame_025.s frame_025
resources/frame_026.bin: resources/frame_026.png
	python resources/conv.py resources/frame_026.png resources/frame_026.bin
frame_026.s: resources/frame_026.bin
	$(BIN2S) resources/frame_026.bin frame_026.s frame_026
resources/frame_027.bin: resources/frame_027.png
	python resources/conv.py resources/frame_027.png resources/frame_027.bin
frame_027.s: resources/frame_027.bin
	$(BIN2S) resources/frame_027.bin frame_027.s frame_027
resources/frame_028.bin: resources/frame_028.png
	python resources/conv.py resources/frame_028.png resources/frame_028.bin
frame_028.s: resources/frame_028.bin
	$(BIN2S) resources/frame_028.bin frame_028.s frame_028
resources/frame_029.bin: resources/frame_029.png
	python resources/conv.py resources/frame_029.png resources/frame_029.bin
frame_029.s: resources/frame_029.bin
	$(BIN2S) resources/frame_029.bin frame_029.s frame_029
resources/frame_030.bin: resources/frame_030.png
	python resources/conv.py resources/frame_030.png resources/frame_030.bin
frame_030.s: resources/frame_030.bin
	$(BIN2S) resources/frame_030.bin frame_030.s frame_030
resources/frame_031.bin: resources/frame_031.png
	python resources/conv.py resources/frame_031.png resources/frame_031.bin
frame_031.s: resources/frame_031.bin
	$(BIN2S) resources/frame_031.bin frame_031.s frame_031
resources/frame_032.bin: resources/frame_032.png
	python resources/conv.py resources/frame_032.png resources/frame_032.bin
frame_032.s: resources/frame_032.bin
	$(BIN2S) resources/frame_032.bin frame_032.s frame_032
resources/frame_033.bin: resources/frame_033.png
	python resources/conv.py resources/frame_033.png resources/frame_033.bin
frame_033.s: resources/frame_033.bin
	$(BIN2S) resources/frame_033.bin frame_033.s frame_033
resources/frame_034.bin: resources/frame_034.png
	python resources/conv.py resources/frame_034.png resources/frame_034.bin
frame_034.s: resources/frame_034.bin
	$(BIN2S) resources/frame_034.bin frame_034.s frame_034
resources/frame_035.bin: resources/frame_035.png
	python resources/conv.py resources/frame_035.png resources/frame_035.bin
frame_035.s: resources/frame_035.bin
	$(BIN2S) resources/frame_035.bin frame_035.s frame_035
resources/frame_036.bin: resources/frame_036.png
	python resources/conv.py resources/frame_036.png resources/frame_036.bin
frame_036.s: resources/frame_036.bin
	$(BIN2S) resources/frame_036.bin frame_036.s frame_036
resources/frame_037.bin: resources/frame_037.png
	python resources/conv.py resources/frame_037.png resources/frame_037.bin
frame_037.s: resources/frame_037.bin
	$(BIN2S) resources/frame_037.bin frame_037.s frame_037
resources/frame_038.bin: resources/frame_038.png
	python resources/conv.py resources/frame_038.png resources/frame_038.bin
frame_038.s: resources/frame_038.bin
	$(BIN2S) resources/frame_038.bin frame_038.s frame_038
resources/frame_039.bin: resources/frame_039.png
	python resources/conv.py resources/frame_039.png resources/frame_039.bin
frame_039.s: resources/frame_039.bin
	$(BIN2S) resources/frame_039.bin frame_039.s frame_039
resources/frame_040.bin: resources/frame_040.png
	python resources/conv.py resources/frame_040.png resources/frame_040.bin
frame_040.s: resources/frame_040.bin
	$(BIN2S) resources/frame_040.bin frame_040.s frame_040
resources/frame_041.bin: resources/frame_041.png
	python resources/conv.py resources/frame_041.png resources/frame_041.bin
frame_041.s: resources/frame_041.bin
	$(BIN2S) resources/frame_041.bin frame_041.s frame_041
resources/frame_042.bin: resources/frame_042.png
	python resources/conv.py resources/frame_042.png resources/frame_042.bin
frame_042.s: resources/frame_042.bin
	$(BIN2S) resources/frame_042.bin frame_042.s frame_042
resources/frame_043.bin: resources/frame_043.png
	python resources/conv.py resources/frame_043.png resources/frame_043.bin
frame_043.s: resources/frame_043.bin
	$(BIN2S) resources/frame_043.bin frame_043.s frame_043
resources/frame_044.bin: resources/frame_044.png
	python resources/conv.py resources/frame_044.png resources/frame_044.bin
frame_044.s: resources/frame_044.bin
	$(BIN2S) resources/frame_044.bin frame_044.s frame_044
resources/frame_045.bin: resources/frame_045.png
	python resources/conv.py resources/frame_045.png resources/frame_045.bin
frame_045.s: resources/frame_045.bin
	$(BIN2S) resources/frame_045.bin frame_045.s frame_045
resources/frame_046.bin: resources/frame_046.png
	python resources/conv.py resources/frame_046.png resources/frame_046.bin
frame_046.s: resources/frame_046.bin
	$(BIN2S) resources/frame_046.bin frame_046.s frame_046
resources/frame_047.bin: resources/frame_047.png
	python resources/conv.py resources/frame_047.png resources/frame_047.bin
frame_047.s: resources/frame_047.bin
	$(BIN2S) resources/frame_047.bin frame_047.s frame_047
resources/frame_048.bin: resources/frame_048.png
	python resources/conv.py resources/frame_048.png resources/frame_048.bin
frame_048.s: resources/frame_048.bin
	$(BIN2S) resources/frame_048.bin frame_048.s frame_048
resources/frame_049.bin: resources/frame_049.png
	python resources/conv.py resources/frame_049.png resources/frame_049.bin
frame_049.s: resources/frame_049.bin
	$(BIN2S) resources/frame_049.bin frame_049.s frame_049
resources/frame_050.bin: resources/frame_050.png
	python resources/conv.py resources/frame_050.png resources/frame_050.bin
frame_050.s: resources/frame_050.bin
	$(BIN2S) resources/frame_050.bin frame_050.s frame_050
resources/frame_051.bin: resources/frame_051.png
	python resources/conv.py resources/frame_051.png resources/frame_051.bin
frame_051.s: resources/frame_051.bin
	$(BIN2S) resources/frame_051.bin frame_051.s frame_051
resources/frame_052.bin: resources/frame_052.png
	python resources/conv.py resources/frame_052.png resources/frame_052.bin
frame_052.s: resources/frame_052.bin
	$(BIN2S) resources/frame_052.bin frame_052.s frame_052
resources/frame_053.bin: resources/frame_053.png
	python resources/conv.py resources/frame_053.png resources/frame_053.bin
frame_053.s: resources/frame_053.bin
	$(BIN2S) resources/frame_053.bin frame_053.s frame_053
resources/frame_054.bin: resources/frame_054.png
	python resources/conv.py resources/frame_054.png resources/frame_054.bin
frame_054.s: resources/frame_054.bin
	$(BIN2S) resources/frame_054.bin frame_054.s frame_054
resources/frame_055.bin: resources/frame_055.png
	python resources/conv.py resources/frame_055.png resources/frame_055.bin
frame_055.s: resources/frame_055.bin
	$(BIN2S) resources/frame_055.bin frame_055.s frame_055
resources/frame_056.bin: resources/frame_056.png
	python resources/conv.py resources/frame_056.png resources/frame_056.bin
frame_056.s: resources/frame_056.bin
	$(BIN2S) resources/frame_056.bin frame_056.s frame_056
resources/frame_057.bin: resources/frame_057.png
	python resources/conv.py resources/frame_057.png resources/frame_057.bin
frame_057.s: resources/frame_057.bin
	$(BIN2S) resources/frame_057.bin frame_057.s frame_057
resources/frame_058.bin: resources/frame_058.png
	python resources/conv.py resources/frame_058.png resources/frame_058.bin
frame_058.s: resources/frame_058.bin
	$(BIN2S) resources/frame_058.bin frame_058.s frame_058
resources/frame_059.bin: resources/frame_059.png
	python resources/conv.py resources/frame_059.png resources/frame_059.bin
frame_059.s: resources/frame_059.bin
	$(BIN2S) resources/frame_059.bin frame_059.s frame_059
resources/frame_060.bin: resources/frame_060.png
	python resources/conv.py resources/frame_060.png resources/frame_060.bin
frame_060.s: resources/frame_060.bin
	$(BIN2S) resources/frame_060.bin frame_060.s frame_060
resources/frame_061.bin: resources/frame_061.png
	python resources/conv.py resources/frame_061.png resources/frame_061.bin
frame_061.s: resources/frame_061.bin
	$(BIN2S) resources/frame_061.bin frame_061.s frame_061
resources/frame_062.bin: resources/frame_062.png
	python resources/conv.py resources/frame_062.png resources/frame_062.bin
frame_062.s: resources/frame_062.bin
	$(BIN2S) resources/frame_062.bin frame_062.s frame_062

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
