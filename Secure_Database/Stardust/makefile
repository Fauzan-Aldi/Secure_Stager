MAKEFLAGS += "-s -j 16"

Project := stardust

CC_X64	:= x86_64-w64-mingw32-g++


CFLAGS  := -Os -fno-asynchronous-unwind-tables -nostdlib
CFLAGS  += -fno-ident -fpack-struct=8 -falign-functions=1
CFLAGS  += -s -ffunction-sections -falign-jumps=1 -w
CFLAGS  += -falign-labels=1 -fPIC -Wl,-Tscripts/Linker.ld
CFLAGS  += -Wl,-s,--no-seh,--enable-stdcall-fixup
CFLAGS  += -Iinclude -masm=intel -fpermissive -mrdrnd

STAR-SRC := $(wildcard src/*.c)
STAR-OBJ := $(STAR-SRC:%.c=%.o)

EXE-X64	:= bin/$(Project).x64.exe
BIN-X64	:= bin/$(Project).x64.bin


all: x64

x64: clean asm-x64 $(STAR-OBJ)
	@ echo "[+] compile x64 executable"
	@ $(CC_X64) bin/obj/*.x64.o -o $(EXE-X64) $(CFLAGS)
	@ python3 scripts/build.py -f $(EXE-X64) -o $(BIN-X64)
	@ rm $(EXE-X64)

#
$(STAR-OBJ):
	@ $(CC_X64) -o bin/obj/$(Project)_$(basename $(notdir $@)).x64.o -c $(basename $@).c $(CFLAGS)

asm-x64:
	@ echo "[*] compile assembly files"
	@ nasm -f win64 asm/x64/Stardust.asm -o bin/obj/asm_Stardust.x64.o

clean:
	@ rm -rf .idea
	@ rm -rf bin/obj/*.o
	@ rm -rf bin/*.bin
	@ rm -rf bin/*.exe
	@ rm -rf cmake-build-debug
