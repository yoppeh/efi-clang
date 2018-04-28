; ------------------------------------------------------------------------------
; main.asm
;
; UEFI calling parameters: rcx, rdx, r8, r9, stack...
;

format ms64 coff


include 'types.inc'
include 'efi.inc'


section '.text' code executable readable

public efi_main


; ------------------------------------------------------------------------------
; Entry Point

efi_main:

    sub rsp, 4 * 8

    ; Save UEFI parameters.
    mov [efi_image_handle], rcx
    mov [efi_system_table], rdx

    ; Print hello.
    lea rdx, [str_hello]
    call uefi_print_str

    mov rax, EFI_SUCCESS
    add rsp, 4 * 8
    retn


uefi_print_str:

    sub rsp, 4 * 8
    mov rcx, [efi_system_table]
    mov rcx, [rcx + EFI_SYSTEM_TABLE.ConOut]
    call [rcx + EFI_SIMPLE_TEXT_OUTPUT_INTERFACE.OutputString]
    add rsp, 4 * 8
    retn

 
section '.data' data readable writeable

    efi_image_handle    dq  (0)
    efi_system_table    dq  (0)

    str_hello           du  'Hello, you warm slab of meat!', 13, 10, 0
