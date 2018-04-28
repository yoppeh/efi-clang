
all : hello-c.efi hello-fasm.efi memmap.efi

hello-fasm.efi : hello-fasm.obj
	./link.sh $@ hello-fasm.obj

hello-c.efi : hello-c.obj
	./link.sh $@ hello-c.obj

memmap.efi : memmap.obj
	./link.sh $@ memmap.obj

hello-fasm.obj : hello-fasm.asm
	fasm $<

hello-c.obj : hello-c.c
	clang -I efi -target x86_64-pc-win32-coff -fno-stack-protector -fshort-wchar -mno-red-zone -DHAVE_USE_MS_ABI -c $< -o $@

memmap.obj : memmap.c
	clang -I efi -target x86_64-pc-win32-coff -fno-stack-protector -fshort-wchar -mno-red-zone -DHAVE_USE_MS_ABI -c $< -o $@

.PHONY : clean
clean:
	if ls *.lib 1> /dev/null 2>&1 ; then rm *.lib ; fi
	if ls *.dll 1> /dev/null 2>&1 ; then rm *.dll ; fi
	if ls *.efi 1> /dev/null 2>&1 ; then rm *.efi ; fi
	if ls *.exe 1> /dev/null 2>&1 ; then rm *.exe ; fi
	if ls *.obj 1> /dev/null 2>&1 ; then rm *.obj ; fi
