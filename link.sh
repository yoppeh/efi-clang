lld-link -subsystem:console -nodefaultlib -dll -entry:efi_main $@ -out:nukeld.dll
petoefi nukeld.dll nukeld.efi app
rm nukeld.dll
rm nukeld.lib
