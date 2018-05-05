ar=("$@")
lld-link -subsystem:EFI_APPLICATION -nodefaultlib -dll -entry:efi_main ${ar[@]:1} -out:${ar[0]}.dll
petoefi ${ar[0]}.dll ${ar[0]} app
rm ${ar[0]}.lib
rm ${ar[0]}.dll

