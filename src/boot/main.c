#include <efi.h>
#include <efilib.h>
#include <efiprot.h>

static EFI_STATUS
fill_rect(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop,
          UINTN x, UINTN y, UINTN width, UINTN height,
          UINT8 red, UINT8 green, UINT8 blue)
{
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel;
    pixel.Red = red;
    pixel.Green = green;
    pixel.Blue = blue;
    pixel.Reserved = 0;

    return uefi_call_wrapper(
        gop->Blt, 10,
        gop, &pixel, EfiBltVideoFill,
        0, 0, x, y, width, height, 0
    );
}

static void
draw_boot_screen(EFI_SYSTEM_TABLE *system_table)
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL;
    EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    EFI_STATUS status = uefi_call_wrapper(
        system_table->BootServices->LocateProtocol,
        3, &gop_guid, NULL, (VOID **)&gop
    );

    if (EFI_ERROR(status) || gop == NULL || gop->Mode == NULL ||
        gop->Mode->Info == NULL) {
        return;
    }

    const UINTN width = gop->Mode->Info->HorizontalResolution;
    const UINTN height = gop->Mode->Info->VerticalResolution;

    (void)fill_rect(gop, 0, 0, width, height, 12, 12, 16);

    const UINTN card_width = width > 720 ? 560 : width * 3 / 4;
    const UINTN card_height = height > 520 ? 250 : height / 2;
    const UINTN card_x = (width - card_width) / 2;
    const UINTN card_y = (height - card_height) / 2;

    (void)fill_rect(gop, card_x, card_y, card_width, card_height,
                    25, 25, 31);

    const UINTN bar_width = card_width > 100 ? card_width - 100 : 40;
    const UINTN bar_x = card_x + (card_width - bar_width) / 2;
    const UINTN bar_y = card_y + card_height - 48;

    (void)fill_rect(gop, bar_x, bar_y, bar_width, 4,
                    62, 62, 72);
    (void)fill_rect(gop, bar_x, bar_y, bar_width / 3, 4,
                    235, 235, 240);
}

EFI_STATUS EFIAPI
efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table)
{
    InitializeLib(image_handle, system_table);

    (void)uefi_call_wrapper(system_table->ConOut->ClearScreen, 1,
                            system_table->ConOut);

    draw_boot_screen(system_table);

    Print(L"iOS 27.3 ARM64 development boot\r\n");
    Print(L"Target: QEMU virt / AArch64\r\n");
    Print(L"Boot stage: UEFI + GOP framebuffer\r\n");
    Print(L"IOS27_BOOT_OK\r\n");

    (void)uefi_call_wrapper(system_table->BootServices->Stall, 1,
                            2000000);

    return EFI_SUCCESS;
}
