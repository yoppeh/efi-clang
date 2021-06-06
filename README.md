# efi-clang

Build UEFI applications with the Clang compiler and LLD linker. Of course, you'll need to have those installed. I tested this with Clang v. 6.0.0. I use Arch Linux, so I installed `pacman -S clang lld`. This uses my [efi headers](https://github.com/yoppeh/efi), included as a submodule. After cloning this repo, you'd do: `git submodule update --init` to pull it in.

Next, gather the topology of your EFI setup. When I partitioned my drive, I made a couple of Linux partitions and then one EFI system partition. It was formatted with fat32 and I installed systemd-boot (a poor choice). I made an arch directory for the Arch Linux kernel and an app directory that contains shellx64_v2.efi (The EFI shell you can get from [Tianocore](https://github.com/tianocore/tianocore.github.io/wiki/Efi-shell)). I setup systemd-boot's menu so I could either load Linux or the EFI shell at bootup.

With that, you would drop into the efi-clang directory, run `make` and then `sudo cp *.efi /boot/app` (/boot is where the EFI system partition is mounted).

Then reboot, choose the shell option from the boot menu and then do: 

`fs0:`

`cd app`

`hello-c.efi`

`hello-fasm.efi`

`memmap.efi`

If you built a similar application using gnu-efi, you'll notice this executable is substantially smaller.

## Rationale

Modern PC-compatible systems have replaced the old system BIOS with the Unified Extensible Firmware Interface. This is almost an operating system in itself and makes some of the vagaries of bringing up a system a little more clean than in the past. Several well-defined software interfaces are provided for mapping hardware components, figuring out memory layout, loading kernels, showing startup screens and some other boring junk operating systems have to do to survive. I recently had occasion to do some work in UEFI.

UEFI has actually been around a while, but the platforms I've seen still seem a little immature.  A couple of the better ones are the implementations on my Asus Zenbook Pro and the two Intel NUCs I use for development. Having downloaded a copy of the [UEFI specification](http://www.uefi.org/specifications), I set out to build a Hello World UEFI application.

That's when things got a little messy. I found out there are basically three options when it comes to building UEFI applications: [Tianocore](https://www.tianocore.org/), which is Intel's UEFI build environment, [gnu-efi](https://sourceforge.net/projects/gnu-efi/), and FASM which can [natively produce EFI application binaries](https://board.flatassembler.net/topic.php?t=19540) (crazily enough).

I tried Tianocore first—straight from the horse's mouth as it were. The application built fine and it worked instantly. The problem is the labyrinthine build environment. They actually recommend sticking your code somewhere in the Tianocore build tree and modifying the build files to include your application code. Sorry, Intel, but I'm not going to change the layout of my source tree to what you think it should be. Actually, with the instructions I'll provide shortly, you should be able to use Tianocore's headers and libraries and dump their build system without too much trouble. If you care about correctness (which is generally probably good) and/or aren't as mental about your build system as I am, then I would recommend going with that option. Also, Tianocore's [github repo](https://github.com/tianocore/edk2) has thorough instructions for setting it up on various platforms.

Disillusioned with Tianocore's messy build environment, I decided to hold my nose and try the gnu-efi way of doing things. There was a package available for it in the Arch repository, so getting it setup was easy enough. It also took minimal effort to build a working application. I was excited it was much more straight-forward than the Tianocore mess, but I was still holding my nose. It wasn't long before I realized gnu-efi's build instructions had dumped an unreasonably large binary in my build directory. I couldn't imagine what all those bytes were doing in a little Hello World EFI application.  Later, I discovered the problem: the gnu-efi application provides binary startup objects and a library for relocation and dynamic resolving of relocatable symbols. Gross.

I decided I'd go with my own UEFI build environment using [Clang](https://clang.llvm.org/) and [LLD](https://lld.llvm.org/) as the compiler and linker and using my own code instead of Tianocore or gnu-efi project libraries and headers. As it turns out, I had to do considerable research before I could put all the bits together to make it work.

The first thing you need is a set of header files. I built my own, using the spec. I had a creeping feeling Tianocore and GNU would use their own versions of what a header should be, and I preferred to use something custom. My application would have no startup code or link-libraries. Everything used would be defined in the project itself and linked in if it's used and rejected if not.

Next, I installed the build tools themselves: clang and lld. It was an easy install on Arch Linux and I had everything up and going without incident. Then it got hard. Clang had to produce Microsoft object files to be compatible with the way UEFI does things. I had to search down the options and fiddle with them until I got them working properly. LLVM itself has to be called as though it were Microsoft's LINK. Once I got everything together, I captured it here, in git where others could use it. Take special care to examine the clang and lld commands in the makefile.

Notice the code I make available doesn't use a runtime library or startup files. The loader will read it into memory and execute it directly. I prefer to access the various functions and structures in their raw form, without a library hiding the details from me. This gives me a more intimate knowledge of the library that I find useful.  As I said earlier, this setup can be modified to use the official Tianocore headers. Indeed, the headers provided here are incomplete.

Finally, the goal with your c compiler is to build an MS object format output file for each of your compiled modules. Those object files will be linked with Clang's linker in Microsoft LINK mode to generate a PE64 executable.

I've used this to successfully build on FASM without using FASM's built-in PE format generator. I still haven't quite gotten NASM to work. The binary is recognized and loads fine and, if it does nothing but terminate, it works great! I think there are some position independent code related issues. I investigated it briefly, but didn't find a solution and had more pressing things to work on. I think NASM is very close to working and I may look into it in the future. For now, I am content to use FASM when necessary. If anyone reading this figures out the secret, I'd definitely be interested in knowing.

I hope this post will find others looking into getting into UEFI and offer them some help in how it fits together. Maybe it will even give enough information to help building UEFI apps on other systems or with other compilers. 

Good Luck!
