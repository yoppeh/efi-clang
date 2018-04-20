lld-link -subsystem:console -nodefaultlib -dll -entry:efi_main $@ -out:hello.dll
petoefi hello.dll hello.efi app
rm hello.lib
