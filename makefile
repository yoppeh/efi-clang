
cc = clang
cflags = -I efi -target x86_64-pc-win32-coff -fno-stack-protector -fshort-wchar -mno-red-zone
ld = lld-link
lflags = -subsystem:efi_application -nodefaultlib -dll

all : hello-c.efi hello-fasm.efi memmap.efi

hello-fasm.efi : hello-fasm.obj
	$(ld) $(lflags) -entry:efi_main $< -out:$@

hello-c.efi : hello-c.obj
	$(ld) $(lflags) -entry:efi_main $< -out:$@

memmap.efi : memmap.obj
	$(ld) $(lflags) -entry:efi_main $< -out:$@

hello-fasm.obj : hello-fasm.asm
	fasm $<

hello-c.obj : hello-c.c
	$(cc) $(cflags) -c $< -o $@

memmap.obj : memmap.c
	$(cc) $(cflags) -c $< -o $@

.PHONY : clean
clean:
	if ls *.lib 1> /dev/null 2>&1 ; then rm *.lib ; fi
	if ls *.dll 1> /dev/null 2>&1 ; then rm *.dll ; fi
	if ls *.efi 1> /dev/null 2>&1 ; then rm *.efi ; fi
	if ls *.exe 1> /dev/null 2>&1 ; then rm *.exe ; fi
	if ls *.obj 1> /dev/null 2>&1 ; then rm *.obj ; fi
