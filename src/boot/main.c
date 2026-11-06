#include <stdint.h>
#include <stddef.h>

typedef uint64_t EFI_STATUS;
typedef uint64_t EFI_UINTN;
typedef uint16_t CHAR16;
typedef void *EFI_HANDLE;

typedef struct {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t Data4[8];
} EFI_GUID;

typedef struct {
    uint64_t Signature;
    uint32_t Revision;
    uint32_t HeaderSize;
    uint32_t CRC32;
    uint32_t Reserved;
} EFI_TABLE_HEADER;

typedef EFI_STATUS (*EFI_OUTPUT_STRING)(void *This, CHAR16 *String);
typedef EFI_STATUS (*EFI_CLEAR_SCREEN)(void *This);

typedef struct {
    void *Reset;
    EFI_OUTPUT_STRING OutputString;
    void *TestString;
    void *QueryMode;
    void *SetMode;
    void *SetAttribute;
    EFI_CLEAR_SCREEN ClearScreen;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef EFI_STATUS (*EFI_LOCATE_PROTOCOL)(
    EFI_GUID *Protocol, void *Registration, void **Interface
);

typedef struct {
    EFI_TABLE_HEADER Header;
    void *ReservedFunctions[37];
    EFI_LOCATE_PROTOCOL LocateProtocol;
} EFI_BOOT_SERVICES_PREFIX;

typedef struct {
    EFI_TABLE_HEADER Header;
    CHAR16 *FirmwareVendor;
    uint32_t FirmwareRevision;
    EFI_HANDLE ConsoleInHandle;
    void *ConIn;
    EFI_HANDLE ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    EFI_HANDLE StandardErrorHandle;
    void *StdErr;
    void *RuntimeServices;
    EFI_BOOT_SERVICES_PREFIX *BootServices;
    EFI_UINTN NumberOfTableEntries;
    void *ConfigurationTable;
} EFI_SYSTEM_TABLE;

typedef struct {
    uint8_t Blue;
    uint8_t Green;
    uint8_t Red;
    uint8_t Reserved;
} EFI_GRAPHICS_OUTPUT_BLT_PIXEL;

typedef EFI_STATUS (*EFI_GRAPHICS_BLT)(
    void *This,
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer,
    uint32_t BltOperation,
    EFI_UINTN SourceX,
    EFI_UINTN SourceY,
    EFI_UINTN DestinationX,
    EFI_UINTN DestinationY,
    EFI_UINTN Width,
    EFI_UINTN Height,
    EFI_UINTN Delta
);

typedef struct {
    uint32_t Version;
    uint32_t HorizontalResolution;
    uint32_t VerticalResolution;
    uint32_t PixelFormat;
    uint32_t PixelInformation[4];
    uint32_t PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct {
    uint32_t MaxMode;
    uint32_t Mode;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
    EFI_UINTN SizeOfInfo;
    uint64_t FrameBufferBase;
    EFI_UINTN FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct {
    void *QueryMode;
    void *SetMode;
    EFI_GRAPHICS_BLT Blt;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

static const EFI_GUID GOP_GUID = {
    0x9042a9de, 0x23dc, 0x4a38,
    {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}
};

static EFI_STATUS
fill_rect(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop,
          EFI_UINTN x, EFI_UINTN y, EFI_UINTN width, EFI_UINTN height,
          uint8_t red, uint8_t green, uint8_t blue)
{
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel = {blue, green, red, 0};
    return gop->Blt(gop, &pixel, 0, 0, 0, x, y, width, height, 0);
}

static void
draw_boot_screen(EFI_SYSTEM_TABLE *system_table)
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = 0;
    EFI_STATUS status = system_table->BootServices->LocateProtocol(
        (EFI_GUID *)&GOP_GUID, 0, (void **)&gop
    );

    if (status != 0 || gop == 0 || gop->Mode == 0 || gop->Mode->Info == 0)
        return;

    EFI_UINTN width = gop->Mode->Info->HorizontalResolution;
    EFI_UINTN height = gop->Mode->Info->VerticalResolution;

    (void)fill_rect(gop, 0, 0, width, height, 12, 12, 16);

    EFI_UINTN card_width = width > 720 ? 560 : width * 3 / 4;
    EFI_UINTN card_height = height > 520 ? 250 : height / 2;
    EFI_UINTN card_x = (width - card_width) / 2;
    EFI_UINTN card_y = (height - card_height) / 2;

    (void)fill_rect(gop, card_x, card_y, card_width, card_height,
                    25, 25, 31);

    EFI_UINTN bar_width = card_width > 100 ? card_width - 100 : 40;
    EFI_UINTN bar_x = card_x + (card_width - bar_width) / 2;
    EFI_UINTN bar_y = card_y + card_height - 48;

    (void)fill_rect(gop, bar_x, bar_y, bar_width, 4, 62, 62, 72);
    (void)fill_rect(gop, bar_x, bar_y, bar_width / 3, 4,
                    235, 235, 240);
}

static CHAR16 line1[] = {
    'i','O','S',' ','2','7','.','3',' ','A','R','M','6','4',' ','d','e','v','e','l','o','p','m','e','n','t',' ','b','o','o','t',13,10,0
};
static CHAR16 line2[] = {
    'T','a','r','g','e','t',':',' ','Q','E','M','U',' ','v','i','r','t',' ','/',' ','A','A','r','c','h','6','4',13,10,0
};
static CHAR16 line3[] = {
    'B','o','o','t',' ','s','t','a','g','e',':',' ','U','E','F','I',' ','+',' ','G','O','P',' ','f','r','a','m','e','b','u','f','f','e','r',13,10,0
};
static CHAR16 marker[] = {
    'I','O','S','2','7','_','B','O','O','T','_','O','K',13,10,0
};

EFI_STATUS efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table)
{
    (void)image_handle;

    if (system_table == 0 || system_table->ConOut == 0 ||
        system_table->BootServices == 0)
        return 1;

    if (system_table->ConOut->ClearScreen != 0)
        (void)system_table->ConOut->ClearScreen(system_table->ConOut);

    draw_boot_screen(system_table);

    (void)system_table->ConOut->OutputString(system_table->ConOut, line1);
    (void)system_table->ConOut->OutputString(system_table->ConOut, line2);
    (void)system_table->ConOut->OutputString(system_table->ConOut, line3);
    (void)system_table->ConOut->OutputString(system_table->ConOut, marker);

    return 0;
}
