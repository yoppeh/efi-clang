# efi-clang

Build UEFI applications with the Clang compiler and LLD linker. Of course, you'll need to have those installed. I tested this with Clang v. 6.0.0. I use *Arch Linux*, so I installed `pacman -S clang lld`. 

Next, gather the topology of your EFI setup. When I partitioned my drive, I made a couple of Linux partitions and then one EFI system partition. It was formatted with fat32 and I installed *systemd-boot*. I made an arch directory for the *Arch Linux* kernel and an app directory that contains *shellx64_v2.efi* (The EFI shell you can get from [Tianocore](https://github.com/tianocore/tianocore.github.io/wiki/Efi-shell)). I setup systemd-boot's menu so I could either load Linux or the EFI shell at bootup.

With that, you would drop into the efi-clang directory, run `make` and then `sudo cp hello.efi /boot/app` (/boot is where the EFI system partition is mounted).

Then reboot, choose the shell option from the boot menu and then do: 

`fs0:`

`cd app`

`hello.efi`

If you built a similar application using *gnu-efi*, you'll notice this executable is substantially smaller.

The `petoefi` program used by the linker script can be found in my [repository](https://github.com/yoppeh/petoefi). A binary built for 64-bit Linux is included in this repository. This program is necessary because the clang linker doesn't seem to have an option to output EFI binaries (it's basically a specific value in a single byte in the PE header)... at least, not as of this writing. If you're nervous about using this binary and don't want to build it yourself, the command `printf "\x0a\x00" | dd of=file-to-change obs=1 seek=156 conv=notrunc` will do the same thing. where file-to-change is the output of link.sh (with the petoefi command removed that would be 'hello.dll'). Note that this command is not always guaranteed to work (thus, the reason I wrote the program to do it), but in this case it likely will.