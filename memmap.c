/*******************************************************************************
 * memmap.c
 *
 * A program to dump the UEFI memory map.
 */

#include <efi.h>


#define CONSOLE_STR_BUFFER_MAX  128
#define NULL    (void *)0


UINTN               memory_map_key;
EFI_HANDLE          main_image_handle;
EFI_SYSTEM_TABLE    *main_system_table;

CHAR16 *console_str_yes = (CHAR16 *)L"yes";
CHAR16 *console_str_no = (CHAR16 *)L"no";
CHAR16 *console_str_valid = (CHAR16 *)L"valid";
CHAR16 *console_str_invalid = (CHAR16 *)L"invalid";
CHAR16 *console_str_crlf = (CHAR16 *)L"\r\n";

static CHAR16 *_tab_str = (CHAR16 *)L"    ";


static CHAR16 _hex_table[] = {
    L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', 
    L'8', L'9', L'a', L'b', L'c', L'd', L'e', L'f'
};

static CHAR16 *_mem_attribute[] = {
    (CHAR16 *)L"reserved",
    (CHAR16 *)L"loader code",
    (CHAR16 *)L"loader data",
    (CHAR16 *)L"boot services code",
    (CHAR16 *)L"boot services data",
    (CHAR16 *)L"runtime services code",
    (CHAR16 *)L"runtime services data",
    (CHAR16 *)L"conventional memory",
    (CHAR16 *)L"unusable memory",
    (CHAR16 *)L"acpi reclaim memory",
    (CHAR16 *)L"acpi memory nvs",
    (CHAR16 *)L"memory mapped io",
    (CHAR16 *)L"memory mapped io port space",
    (CHAR16 *)L"pal code",
    (CHAR16 *)L"persistent memory"
};

static CHAR16 *_mem_attribute_unrecognized = (CHAR16 *)L"unrecognized";

static EFI_MEMORY_DESCRIPTOR    *_mem_map = NULL;
static UINTN                    _mem_map_size = 0;
static UINTN                    _mem_map_desc_size = 0;
static uint64_t                 _mem_map_num_entries = 0;
static uint64_t                 _mem_map_alloced_pages = 0;
static UINT32                   _mem_map_desc_ver = 0;


static void console_print(CHAR16 * s);
static void console_println(CHAR16 * s);
static void console_print_dec(uint64_t dec);
static void console_print_hex(uint64_t hex, uint8_t width);
static void memory_dump_uefi_map(void);
static int memory_init(void);


EFI_STATUS efi_main(EFI_HANDLE ih, EFI_SYSTEM_TABLE *st) 
{
    main_image_handle = ih;
    main_system_table = st;

    if (memory_init()) {
        return(EFI_ERR);
    }
    memory_dump_uefi_map();

    return(EFI_SUCCESS);
}


static void console_print(CHAR16 * s) 
{
    main_system_table->ConOut->OutputString(main_system_table->ConOut, s);
}


static void console_println(CHAR16 * s)
{
    main_system_table->ConOut->OutputString(main_system_table->ConOut, s);
    main_system_table->ConOut->OutputString(main_system_table->ConOut, 
            console_str_crlf);
}


static void console_print_dec(uint64_t dec) 
{
    int16_t i = CONSOLE_STR_BUFFER_MAX - 1;
    CHAR16  buffer[CONSOLE_STR_BUFFER_MAX];

    buffer[i--] = 0x0000;

    do {
        buffer[i--] = (CHAR16)L'0' + dec % 10;
        dec /= 10;
    } while (dec > 0 && i >= 0);
    i++;

    console_print(buffer + i);
}


static void console_print_hex(uint64_t hex, uint8_t width) 
{
    int16_t i = width + 1;
    CHAR16  buffer[CONSOLE_STR_BUFFER_MAX];

    if (i >= CONSOLE_STR_BUFFER_MAX) {
        return;
    }

    buffer[i--] = 0x0000;
    buffer[i--] = L'h';
 
    while (width && i >= 0) {
        buffer[i--] = _hex_table[hex & 0x0f];
        hex >>= 4;
        width--;
    }

    console_print(buffer);
}


static void memory_dump_uefi_map(void) 
{
    void                    *mm = _mem_map;
    EFI_MEMORY_DESCRIPTOR   *mem_map;
    uint64_t                i;
    uint64_t                total_mapped = 0;

    _mem_map_num_entries = _mem_map_size / _mem_map_desc_size;
    for (i = 0; i < _mem_map_num_entries; i++) {
        mem_map = (EFI_MEMORY_DESCRIPTOR *)mm;
        console_print_hex(mem_map->PhysicalStart, 16);
        console_print(_tab_str);
        console_print_hex(mem_map->VirtualStart, 16);
        console_print(_tab_str);
        console_print_hex(mem_map->NumberOfPages, 16);
        console_print(_tab_str);
        if (mem_map->Type >= EfiMaxMemoryType) {
            console_println(_mem_attribute_unrecognized);
        } else {
            console_println(_mem_attribute[mem_map->Type]);
        }
        total_mapped += mem_map->NumberOfPages * 4096;
        mm += _mem_map_desc_size;
    }
    console_print((CHAR16 *)L"Total memory mapped ... ");
    console_print_dec(total_mapped);
    console_print(console_str_crlf);
}


static int memory_init(void) 
{
    UINTN   map_size;

    if (main_system_table->BootServices->GetMemoryMap(&_mem_map_size, NULL, 
            &memory_map_key, &_mem_map_desc_size, 
            &_mem_map_desc_ver) != EFI_BUFFER_TOO_SMALL) {
        console_println((CHAR16 *)L"error getting system memory map");
        return 1;
    }

    map_size = (_mem_map_size + 4095) / 4096;
    _mem_map_alloced_pages = map_size;
    _mem_map_size = map_size * 4096;
    if (main_system_table->BootServices->AllocatePages(AllocateAnyPages, 
            EfiLoaderData, map_size, 
            (EFI_PHYSICAL_ADDRESS *)&_mem_map) != EFI_SUCCESS) {
        console_print((CHAR16 *)L"error allocating ");
        console_print_dec(_mem_map_size);
        console_println((CHAR16 *)L" pages for GetMemoryMap");
        return 1;
    }

    /* Finally, get the memory map for real. It will be stored in the memory
     * area we allocated above and pointed to by _mem_map. */
    if (main_system_table->BootServices->GetMemoryMap(&_mem_map_size,
            _mem_map, &memory_map_key, &_mem_map_desc_size,
            &_mem_map_desc_ver) != EFI_SUCCESS) {
        console_println((CHAR16 *)L"failed to get system memory map");
        return 1;
    }

    return 0;
}
