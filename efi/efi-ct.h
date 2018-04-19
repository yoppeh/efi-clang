/*
 * efi-ct.h
 *
 * UEFI Configuration Table.
 */

#ifndef __EFI_CT_H
#define __EFI_CT_H

#include "efi.h"


typedef struct EFI_CONFIGURATION_TABLE {
    EFI_GUID    VendorGuid;
    VOID        *VendorTable;
} EFI_CONFIGURATION_TABLE;


#endif /* __EFI_CT_H */
