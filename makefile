ARCH = x86_64
TARGET = $(ARCH)-unknown-windows

CC = clang
CFLAGS = -I efi --target $(TARGET) -fno-stack-protector -fshort-wchar -mno-red-zone
# LD = lld-link
LDFLAGS = \
        --target $(TARGET) \
        -nostdlib \
        -Wl,-entry:efi_main \
        -Wl,-subsystem:efi_application \
        -fuse-ld=lld-link

all : hello-c.efi memmap.efi hello-fasm.efi

.SUFFIXES: .c .obj .efi .asm

.c.obj:
	$(CC) $(CFLAGS) -c $< -o $@

.asm.obj:
	fasm $<

.obj.efi:
	$(CC) $(LDFLAGS) $< -out:$@

.PHONY : clean
clean:
	-rm -f *.lib *.dll *.efi *.exe *.obj
