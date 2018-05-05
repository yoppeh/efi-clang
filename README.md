# efi-clang

Build UEFI applications with the Clang compiler and LLD linker. Of course, you'll need to have those installed. I tested this with Clang v. 6.0.0. I use *Arch Linux*, so I installed `pacman -S clang lld`. This uses my [efi library](https://github.com/yoppeh/efi), included as a submodule. After cloning this repo, you'd do: `git submodule update --init` to pull it in.

Next, gather the topology of your EFI setup. When I partitioned my drive, I made a couple of Linux partitions and then one EFI system partition. It was formatted with fat32 and I installed *systemd-boot* (a poor choice). I made an arch directory for the *Arch Linux* kernel and an app directory that contains *shellx64_v2.efi* (The EFI shell you can get from [Tianocore](https://github.com/tianocore/tianocore.github.io/wiki/Efi-shell)). I setup systemd-boot's menu so I could either load Linux or the EFI shell at bootup.

With that, you would drop into the efi-clang directory, run `make` and then `sudo cp *.efi /boot/app` (/boot is where the EFI system partition is mounted).

Then reboot, choose the shell option from the boot menu and then do: 

`fs0:`

`cd app`

`hello-c.efi`

`hello-fasm.efi`

`memmap.efi`

If you built a similar application using *gnu-efi*, you'll notice this executable is substantially smaller.
