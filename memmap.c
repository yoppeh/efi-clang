/*******************************************************************************
 * memmap.c
 *
 * A program to dump the UEFI memory map.
 */

#include <efi.h>


#define CONSOLE_STR_BUFFER_MAX  128
#define NULL                    (void *)0


UINTN               memory_map_key;
EFI_HANDLE          image_handle;
EFI_SYSTEM_TABLE    *system_table;

CHAR16 *console_str_yes = L"yes";
CHAR16 *console_str_no = L"no";
CHAR16 *console_str_valid = L"valid";
CHAR16 *console_str_invalid = L"invalid";
CHAR16 *console_str_crlf = L"\r\n";

static CHAR16 *_tab_str = L"    ";

static CHAR16 *_header_str = L"physical address     virtual address      pages                type";


static CHAR16 _hex_table[] = {
    L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', 
    L'8', L'9', L'a', L'b', L'c', L'd', L'e', L'f'
};

static CHAR16 *_mem_attribute[] = {
    L"reserved",
    L"loader code",
    L"loader data",
    L"boot services code",
    L"boot services data",
    L"runtime services code",
    L"runtime services data",
    L"conventional memory",
    L"unusable memory",
    L"acpi reclaim memory",
    L"acpi memory nvs",
    L"memory mapped io",
    L"memory mapped io port space",
    L"pal code",
    L"persistent memory"
};

static CHAR16 *_mem_attribute_unrecognized = L"unrecognized";

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
static void dump_uefi_map(void);
static int init_memory(void);


EFI_STATUS efi_main(EFI_HANDLE ih, EFI_SYSTEM_TABLE *st) 
{
    image_handle = ih;
    system_table = st;

    if (init_memory()) {
        return(EFI_ERR);
    }
    dump_uefi_map();

    return(EFI_SUCCESS);
}


static void console_print(CHAR16 * s) 
{
    system_table->ConOut->OutputString(system_table->ConOut, s);
}


static void console_println(CHAR16 * s)
{
    system_table->ConOut->OutputString(system_table->ConOut, s);
    system_table->ConOut->OutputString(system_table->ConOut, 
            console_str_crlf);
}


static void console_print_dec(uint64_t dec) 
{
    int16_t i = CONSOLE_STR_BUFFER_MAX - 1;
    CHAR16  buffer[CONSOLE_STR_BUFFER_MAX];

    buffer[i--] = 0x0000;

    do {
        buffer[i--] = L'0' + dec % 10;
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


static void dump_uefi_map(void) 
{
    void                    *mm = _mem_map;
    EFI_MEMORY_DESCRIPTOR   *mem_map;
    uint64_t                i;
    uint64_t                total_mapped = 0;

    _mem_map_num_entries = _mem_map_size / _mem_map_desc_size;
    console_println(_header_str);
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
    console_print(L"Total memory mapped ... ");
    console_print_dec(total_mapped);
    console_print(console_str_crlf);
}


static int init_memory(void) 
{
    UINTN   map_size;

    if (system_table->BootServices->GetMemoryMap(&_mem_map_size, NULL, 
            &memory_map_key, &_mem_map_desc_size, 
            &_mem_map_desc_ver) != EFI_BUFFER_TOO_SMALL) {
        console_println(L"error getting system memory map");
        return 1;
    }

    map_size = (_mem_map_size + 4095) / 4096;
    _mem_map_alloced_pages = map_size;
    _mem_map_size = map_size * 4096;
    if (system_table->BootServices->AllocatePages(AllocateAnyPages, 
            EfiLoaderData, map_size, 
            (EFI_PHYSICAL_ADDRESS *)&_mem_map) != EFI_SUCCESS) {
        console_print(L"error allocating ");
        console_print_dec(_mem_map_size);
        console_println(L" pages for GetMemoryMap");
        return 1;
    }

    /* Finally, get the memory map for real. It will be stored in the memory
     * area we allocated above and pointed to by _mem_map. */
    if (system_table->BootServices->GetMemoryMap(&_mem_map_size,
            _mem_map, &memory_map_key, &_mem_map_desc_size,
            &_mem_map_desc_ver) != EFI_SUCCESS) {
        console_println(L"failed to get system memory map");
        return 1;
    }

    return 0;
}
