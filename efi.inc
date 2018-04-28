; ------------------------------------------------------------------------------
; efi.inc
;
; UEFI declarations.


; ------------------------------------------------------------------------------
; CONSTANTS

; error codes
EFI_ERR               = 0x8000000000000000
EFI_SUCCESS           = 0x0000000000000000
EFI_LOAD_ERROR        = EFI_ERR or 1
EFI_INVALID_PARAMETER = EFI_ERR or 2
EFI_UNSUPPORTED       = EFI_ERR or 3
EFI_BAD_BUFFER_SIZE   = EFI_ERR or 4
EFI_BUFFER_TOO_SMALL  = EFI_ERR or 5
EFI_NOT_READY         = EFI_ERR or 6
EFI_DEVICE_ERROR      = EFI_ERR or 7
EFI_WRITE_PROTECTED   = EFI_ERR or 8
EFI_OUT_OF_RESOURCES  = EFI_ERR or 9
EFI_VOLUME_CORRUPTED  = EFI_ERR or 10
EFI_VOLUME_FULL       = EFI_ERR or 11
EFI_NO_MEDIA          = EFI_ERR or 12
EFI_MEDIA_CHANGED     = EFI_ERR or 13
EFI_NOT_FOUND         = EFI_ERR or 14
EFI_ACCESS_DENIED     = EFI_ERR or 15
EFI_NO_RESPONSE       = EFI_ERR or 16
EFI_NO_MAPPING        = EFI_ERR or 17
EFI_TIMEOUT           = EFI_ERR or 18
EFI_NOT_STARTED       = EFI_ERR or 19
EFI_ALREADY_STARTED   = EFI_ERR or 20
EFI_ABORTED           = EFI_ERR or 21
EFI_ICMP_ERROR        = EFI_ERR or 22 
EFI_TFTP_ERROR        = EFI_ERR or 23
EFI_PROTOCOL_ERROR    = EFI_ERR or 24

; allocation types
EFI_ALLOCATE_TYPE_ALLOCATE_ANY_PAGES   = 0x00000000
EFI_ALLOCATE_TYPE_ALLOCATE_MAX_ADDRESS = 0x00000001
EFI_ALLOCATE_ALLOCATE_ADDRESS          = 0x00000002
EFI_ALLOCATE_MAX_ALLOCATE_TYPE         = 0x00000003

; memory types
EFI_MEMORY_TYPE_RESERVED                    = 0x00000000
EFI_MEMORY_TYPE_LOADER_CODE                 = 0x00000001
EFI_MEMORY_TYPE_LOADER_DATA                 = 0x00000002
EFI_MEMORY_TYPE_BOOT_SERVICES_CODE          = 0x00000003
EFI_MEMORY_TYPE_BOOT_SERVICES_DATA          = 0x00000004
EFI_MEMORY_TYPE_RUNTIME_SERVICES_CODE       = 0x00000005
EFI_MEMORY_TYPE_RUNTIME_SERVICES_DATA       = 0x00000006
EFI_MEMORY_TYPE_CONVENTIONAL                = 0x00000007
EFI_MEMORY_TYPE_UNUSABLE                    = 0x00000008
EFI_MEMORY_TYPE_ACPI_RECLAIM                = 0x00000009
EFI_MEMORY_TYPE_ACPI_NVS                    = 0x0000000a
EFI_MEMORY_TYPE_MEMORY_MAPPED_IO            = 0x0000000b
EFI_MEMORY_TYPE_MEMORY_MAPPED_IO_PORT_SPACE = 0x0000000c
EFI_MEMORY_TYPE_PAL_CODE                    = 0x0000000d
EFI_MEMORY_TYPE_PERSISTENT                  = 0x0000000e
EFI_MEMORY_TYPE_MAX                         = 0x0000000f

; graphics pixel formats
PixelRedGreenBlueReserved8BitPerColor = 0x00000000
PixelBlueGreenRedReserved8BitPerColor = 0x00000001
PixelBitMask                          = 0x00000002
PixelBltOnly                          = 0x00000003
PixelFormatMax                        = 0x00000004


; ------------------------------------------------------------------------------
; STRUCTURES

struc EFI_GUID {
    .Lo int64
    .Hi int64
}
struct EFI_GUID

struc EFI_TABLE_HEADER {
 .Signature    int64
 .Revision     int32
 .HeaderSize   int32
 .CRC32        int32
 .Reserved     int32
}
struct EFI_TABLE_HEADER

struc EFI_SYSTEM_TABLE {
    .Hdr                    EFI_TABLE_HEADER
    .FirmwareVendor         dptr
    .FirmwareRevision       int32
    .ConsoleInHandle        dptr
    .ConIn                  dptr
    .ConsoleOutHandle       dptr
    .ConOut                 dptr
    .StandardErrorHandle    dptr
    .StdErr                 dptr
    .RuntimeServices        dptr
    .BootServices           dptr
    .NumberOfTableEntries   intn
    .ConfigurationTable     dptr
}
struct EFI_SYSTEM_TABLE

struc EFI_BOOT_SERVICES {
    .Hdr                                    EFI_TABLE_HEADER
    .RaiseTPL                               dptr
    .RestoreTPL                             dptr
    .AllocatePages                          dptr
    .FreePages                              dptr
    .GetMemoryMap                           dptr    
    .AllocatePool                           dptr
    .FreePool                               dptr
    .CreateEvent                            dptr
    .SetTimer                               dptr
    .WaitForEvent                           dptr
    .SignalEvent                            dptr
    .CloseEvent                             dptr
    .CheckEvent                             dptr
    .InstallProtocolInterface               dptr
    .ReinstallProtocolInterface             dptr
    .UninstallProtocolInterface             dptr
    .HandleProtocol                         dptr
    .Reserved                               dptr
    .RegisterProtocolNotify                 dptr
    .LocateHandle                           dptr
    .LocateDevicePath                       dptr
    .InstallConfigurationTable              dptr
    .LoadImage                              dptr
    .StartImage                             dptr
    .Exit                                   dptr
    .UnloadImage                            dptr
    .ExitBootServices                       dptr
    .GetNextMonotonicCount                  dptr
    .Stall                                  dptr
    .SetWatchdogTimer                       dptr
    .ConnectController                      dptr
    .DisconnectController                   dptr
    .OpenProtocol                           dptr
    .CloseProtocol                          dptr
    .OpenProtocolInformation                dptr
    .ProtocolsPerHandle                     dptr
    .LocateHandleBuffer                     dptr
    .LocateProtocol                         dptr
    .InstallMultipleProtocolInterfaces      dptr
    .UninstallMultipleProtocolInterfaces    dptr
    .CalculateCrc32                         dptr
    .CopyMem                                dptr
    .SetMem                                 dptr
    .CreateEventEx                          dptr
}
struct EFI_BOOT_SERVICES

struc EFI_MEMORY_DESCRIPTOR {
    .Type           int32
    .PhysicalStart  int64
    .VirtualStart   int64
    .NumberOfPages  int64
    .Attribute      int64
}
struct EFI_MEMORY_DESCRIPTOR

struc EFI_SIMPLE_TEXT_OUTPUT_INTERFACE {
    .Reset              dptr
    .OutputString       dptr
    .TestString         dptr
    .QueryMode          dptr
    .SetMode            dptr
    .SetAttribute       dptr
    .ClearScreen        dptr
    .SetCursorPosition  dptr
    .EnableCursor       dptr
    .Mode               dptr
}
struct EFI_SIMPLE_TEXT_OUTPUT_INTERFACE

struc EFI_SIMPLE_TEXT_INPUT_INTERFACE {
    .Reset          dptr
    .ReadKeyStroke  dptr
    .WaitForKey     dptr
}
struct EFI_SIMPLE_TEXT_INPUT_INTERFACE

struc EFI_GRAPHICS_OUTPUT_PROTOCOL {
    .QueryMode      dptr
    .SetMode        dptr
    .Blt            dptr
    .Mode           dptr
}
struct EFI_GRAPHICS_OUTPUT_PROTOCOL

struc EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE {
    .MaxMode            int32
    .Mode               int32
    .Info               dptr
    .SizeOfInfo         intn
    .FrameBufferBase    dptr
    .FrameBufferSize    intn
}
struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE

struc EFI_PIXEL_BITMASK {
    .RedMask        int32
    .GreenMask      int32
    .BlueMask       int32
    .ReservedMask   int32
}
struct EFI_PIXEL_BITMASK

struc EFI_GRAPHICS_OUTPUT_MODE_INFORMATION {
    .Version                    int32
    .HorizontalResolution       int32
    .VerticalResolution         int32
    .PixelFormat                EFI_PIXEL_BITMASK
    .PixelInformation           int32
    .PixelsPerScanLine          int32
}
struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION


struc EFI_CONFIGURATION_TABLE {
    .VendorGuid                 EFI_GUID
    .VendorTable                dptr
}
struct EFI_CONFIGURATION_TABLE
