#include <efi.h>
#include <efilib.h>

extern "C" EFI_STATUS EFIAPI efi_main(EFI_HANDLE img, EFI_SYSTEM_TABLE *st) {
    ::InitializeLib(img, st);
    return EFI_SUCCESS;
}
