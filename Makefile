#
# Makefile
#
# Targets:
# make ......... build all.
# make all ..... build all.
# make clean ... delete object files, libraries and executables.
#
# Variables:
# EFI_PLATFORM ... 0 = IA32, 1 = X64, 2 = Itanium, 3 = ARM, 
#                  4 = AARCH64, 5 = RISCV32, 6 = RISCV64, 
#                  7 = RISCV128, 8 = LOONGARCH32, 9 = LOONGARCH64

CC = clang
CFLAGS = -I efi -target x86_64-pc-win32-coff -fno-stack-protector -fshort-wchar -mno-red-zone
LD = lld-link
LDFLAGS = -subsystem:efi_application -nodefaultlib -dll
ifeq ($(EFI_PLATFORM),)
EFI_PLATFORM = 1
endif

.PHONY : all clean

all : hello-c.efi memmap.efi hello-fasm.efi

hello-fasm.efi : hello-fasm.obj
	$(LD) $(LDFLAGS) -entry:efi_main $< -out:$@

hello-c.efi : hello-c.obj
	$(LD) $(LDFLAGS) -entry:efi_main $< -out:$@

memmap.efi : memmap.obj
	$(LD) $(LDFLAGS) -entry:efi_main $< -out:$@

hello-fasm.obj : hello-fasm.asm
	fasm $<

hello-c.obj : hello-c.c
	$(CC) $(CFLAGS) -c $< -o $@

memmap.obj : memmap.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	- rm -rf *.lib
	- rm -rf *.dll
	- rm -rf *.efi
	- rm -rf *.exe
	- rm -rf *.obj
