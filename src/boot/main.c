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
typedef EFI_STATUS (*EFI_STALL)(EFI_UINTN Microseconds);

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
    void *ReservedBeforeStall[28];
    EFI_STALL Stall;
    void *ReservedAfterStall[8];
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

typedef struct {
    int32_t RelativeMovementX;
    int32_t RelativeMovementY;
    int32_t RelativeMovementZ;
    uint8_t LeftButton;
    uint8_t RightButton;
} EFI_SIMPLE_POINTER_STATE;

typedef struct {
    uint64_t ResolutionX;
    uint64_t ResolutionY;
    uint64_t ResolutionZ;
    uint8_t LeftButton;
    uint8_t RightButton;
} EFI_SIMPLE_POINTER_MODE;

typedef EFI_STATUS (*EFI_POINTER_RESET)(void *This, uint8_t ExtendedVerification);
typedef EFI_STATUS (*EFI_POINTER_GET_STATE)(void *This, EFI_SIMPLE_POINTER_STATE *State);

typedef struct {
    EFI_POINTER_RESET Reset;
    EFI_POINTER_GET_STATE GetState;
    void *WaitForInput;
    EFI_SIMPLE_POINTER_MODE *Mode;
} EFI_SIMPLE_POINTER_PROTOCOL;

static const EFI_GUID GOP_GUID = {
    0x9042a9de, 0x23dc, 0x4a38,
    {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}
};

static const EFI_GUID SIMPLE_POINTER_GUID = {
    0x31878c87, 0x0b75, 0x11d5,
    {0x9a, 0x4f, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}
};

static const uint8_t FONT[41][5] = {
    {0x7e,0x11,0x11,0x11,0x7e}, {0x7f,0x49,0x49,0x49,0x36},
    {0x3e,0x41,0x41,0x41,0x22}, {0x7f,0x41,0x41,0x22,0x1c},
    {0x7f,0x49,0x49,0x49,0x41}, {0x7f,0x09,0x09,0x09,0x01},
    {0x3e,0x41,0x49,0x49,0x7a}, {0x7f,0x08,0x08,0x08,0x7f},
    {0x00,0x41,0x7f,0x41,0x00}, {0x20,0x40,0x41,0x3f,0x01},
    {0x7f,0x08,0x14,0x22,0x41}, {0x7f,0x40,0x40,0x40,0x40},
    {0x7f,0x02,0x0c,0x02,0x7f}, {0x7f,0x04,0x08,0x10,0x7f},
    {0x3e,0x41,0x41,0x41,0x3e}, {0x7f,0x09,0x09,0x09,0x06},
    {0x3e,0x41,0x51,0x21,0x5e}, {0x7f,0x09,0x19,0x29,0x46},
    {0x46,0x49,0x49,0x49,0x31}, {0x01,0x01,0x7f,0x01,0x01},
    {0x3f,0x40,0x40,0x40,0x3f}, {0x1f,0x20,0x40,0x20,0x1f},
    {0x3f,0x40,0x38,0x40,0x3f}, {0x63,0x14,0x08,0x14,0x63},
    {0x07,0x08,0x70,0x08,0x07}, {0x61,0x51,0x49,0x45,0x43},
    {0x3e,0x51,0x49,0x45,0x3e}, {0x00,0x42,0x7f,0x40,0x00},
    {0x42,0x61,0x51,0x49,0x46}, {0x21,0x41,0x45,0x4b,0x31},
    {0x18,0x14,0x12,0x7f,0x10}, {0x27,0x45,0x45,0x45,0x39},
    {0x3c,0x4a,0x49,0x49,0x30}, {0x01,0x71,0x09,0x05,0x03},
    {0x36,0x49,0x49,0x49,0x36}, {0x06,0x49,0x49,0x29,0x1e},
    {0x00,0x36,0x36,0x00,0x00}, {0x00,0x60,0x60,0x00,0x00},
    {0x08,0x08,0x08,0x08,0x08}, {0x20,0x10,0x08,0x04,0x02},
    {0x00,0x00,0x00,0x00,0x00}
};

static EFI_GRAPHICS_OUTPUT_PROTOCOL *g_gop;
static EFI_UINTN g_width;
static EFI_UINTN g_height;
static EFI_UINTN g_scale = 1;
static EFI_UINTN g_cursor_x;
static EFI_UINTN g_cursor_y;
static uint32_t g_page;
static uint32_t g_app;
static uint8_t g_mouse_down;

static EFI_STATUS fill_rect(EFI_UINTN x, EFI_UINTN y, EFI_UINTN w, EFI_UINTN h,
                            uint8_t r, uint8_t g, uint8_t b)
{
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel = {b, g, r, 0};
    if (!w || !h || x >= g_width || y >= g_height) return 0;
    if (x + w > g_width) w = g_width - x;
    if (y + h > g_height) h = g_height - y;
    return g_gop->Blt(g_gop, &pixel, 0, 0, 0, x, y, w, h, 0);
}

static void round_rect(EFI_UINTN x, EFI_UINTN y, EFI_UINTN w, EFI_UINTN h,
                       EFI_UINTN radius, uint8_t r, uint8_t g, uint8_t b)
{
    if (radius * 2 > h) radius = h / 2;
    if (radius * 2 > w) radius = w / 2;
    for (EFI_UINTN row = 0; row < h; ++row) {
        EFI_UINTN dy = 0;
        if (row < radius) dy = radius - row - 1;
        else if (row >= h - radius) dy = row - (h - radius);
        EFI_UINTN inset = 0;
        while (inset < radius && inset * inset + dy * dy < radius * radius)
            ++inset;
        if (w > inset * 2)
            (void)fill_rect(x + inset, y + row, w - inset * 2, 1, r, g, b);
    }
}

static int inside_ellipse(int32_t x, int32_t y, int32_t rx, int32_t ry)
{
    int64_t xx = (int64_t)x * x * ry * ry;
    int64_t yy = (int64_t)y * y * rx * rx;
    int64_t rr = (int64_t)rx * rx * ry * ry;
    return xx + yy <= rr;
}

static void ellipse(EFI_UINTN cx, EFI_UINTN cy, int32_t rx, int32_t ry,
                    uint8_t r, uint8_t g, uint8_t b)
{
    for (int32_t dy = -ry; dy <= ry; ++dy) {
        int32_t left = rx;
        int32_t right = -rx;
        for (int32_t dx = -rx; dx <= rx; ++dx) {
            if (inside_ellipse(dx, dy, rx, ry)) {
                if (dx < left) left = dx;
                if (dx > right) right = dx;
            }
        }
        if (right >= left)
            (void)fill_rect(cx + left, cy + dy, right - left + 1, 1, r, g, b);
    }
}

static void apple_mark(EFI_UINTN cx, EFI_UINTN cy, EFI_UINTN size)
{
    int32_t s = (int32_t)size;
    int32_t rx1 = s * 30 / 100, ry1 = s * 27 / 100;
    int32_t rx2 = s * 22 / 100, ry2 = s * 24 / 100;
    int32_t rx3 = s * 16 / 100, ry3 = s * 10 / 100;

    for (int32_t y = -s * 30 / 100; y <= s * 34 / 100; ++y) {
        int32_t run = 0;
        int active = 0;
        for (int32_t x = -s * 34 / 100; x <= s * 34 / 100; ++x) {
            int body =
                inside_ellipse(x, y - s * 8 / 100, rx1, ry1) ||
                inside_ellipse(x + s * 15 / 100, y - s * 3 / 100, rx2, ry2) ||
                inside_ellipse(x - s * 15 / 100, y - s * 3 / 100, rx2, ry2) ||
                inside_ellipse(x, y - s * 22 / 100, rx3, ry3);
            int bite = inside_ellipse(x - s * 31 / 100, y - s * 8 / 100,
                                      s * 12 / 100, s * 13 / 100);
            int on = body && !bite;
            if (on && !active) { run = x; active = 1; }
            if ((!on || x == s * 34 / 100) && active) {
                int end = on ? x : x - 1;
                (void)fill_rect(cx + run, cy + y, end - run + 1, 1, 242, 242, 246);
                active = 0;
            }
        }
    }
    ellipse(cx + s * 12 / 100, cy - s * 37 / 100,
            s * 15 / 100, s * 6 / 100, 242, 242, 246);
}

static const uint8_t *glyph(char c)
{
    if (c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
    if (c >= 'A' && c <= 'Z') return FONT[(uint32_t)(c - 'A')];
    if (c >= '0' && c <= '9') return FONT[26 + (uint32_t)(c - '0')];
    if (c == ':') return FONT[36];
    if (c == '.') return FONT[37];
    if (c == '-') return FONT[38];
    if (c == '/') return FONT[39];
    return FONT[40];
}

static void text(EFI_UINTN x, EFI_UINTN y, const char *str, EFI_UINTN scale,
                 uint8_t r, uint8_t g, uint8_t b)
{
    while (*str) {
        const uint8_t *bits = glyph(*str++);
        for (EFI_UINTN col = 0; col < 5; ++col) {
            for (EFI_UINTN row = 0; row < 7; ++row) {
                if (bits[col] & (1u << row))
                    (void)fill_rect(x + col * scale, y + row * scale,
                                    scale, scale, r, g, b);
            }
        }
        x += 6 * scale;
    }
}

static void status_bar(void)
{
    text(24 * g_scale, 18 * g_scale, "9:41", 2 * g_scale, 245, 245, 248);
    /* Minimal signal and battery glyphs. */
    for (EFI_UINTN i = 0; i < 4; ++i)
        (void)fill_rect(g_width - 112 * g_scale + i * 5 * g_scale,
                        32 * g_scale - i * 4 * g_scale,
                        3 * g_scale, (8 + i * 4) * g_scale, 235, 235, 240);
    round_rect(g_width - 68 * g_scale, 19 * g_scale, 34 * g_scale,
               17 * g_scale, 4 * g_scale, 235, 235, 240);
    (void)fill_rect(g_width - 64 * g_scale, 23 * g_scale,
                    24 * g_scale, 9 * g_scale, 30, 30, 34);
    (void)fill_rect(g_width - 32 * g_scale, 24 * g_scale,
                    3 * g_scale, 7 * g_scale, 235, 235, 240);
}

static void draw_welcome(void)
{
    (void)fill_rect(0, 0, g_width, g_height, 7, 7, 10);
    status_bar();

    EFI_UINTN logo = 116 * g_scale;
    apple_mark(g_width / 2, g_height * 31 / 100, logo);

    text(g_width / 2 - 78 * g_scale, g_height * 53 / 100,
         "WELCOME", 3 * g_scale, 248, 248, 250);
    text(g_width / 2 - 105 * g_scale, g_height * 53 / 100 + 34 * g_scale,
         "YOUR DEVICE IS READY", g_scale, 160, 160, 170);

    EFI_UINTN bw = g_width * 64 / 100;
    EFI_UINTN bh = 52 * g_scale;
    EFI_UINTN bx = (g_width - bw) / 2;
    EFI_UINTN by = g_height - 112 * g_scale;
    round_rect(bx, by, bw, bh, 18 * g_scale, 242, 242, 246);
    text(g_width / 2 - 27 * g_scale, by + 22 * g_scale,
         "CONTINUE", g_scale, 20, 20, 24);
    text(g_width / 2 - 52 * g_scale, g_height - 36 * g_scale,
         "IOS 27.3 DEVELOPMENT", g_scale, 105, 105, 116);
}

static const char *APP_NAMES[8] = {
    "SETTINGS", "PHOTOS", "CAMERA", "MUSIC",
    "NOTES", "CLOCK", "SAFARI", "FILES"
};
static const uint8_t APP_COLORS[8][3] = {
    {86, 91, 105}, {184, 104, 148}, {90, 94, 102}, {190, 65, 92},
    {196, 171, 104}, {74, 125, 174}, {70, 151, 188}, {170, 173, 180}
};

static void draw_home(void)
{
    (void)fill_rect(0, 0, g_width, g_height, 22, 30, 48);
    status_bar();
    text(26 * g_scale, 72 * g_scale, "HOME", 2 * g_scale, 248, 248, 250);

    EFI_UINTN icon = 58 * g_scale;
    EFI_UINTN start_x = g_width * 9 / 100;
    EFI_UINTN gap_x = (g_width - 2 * start_x - 4 * icon) / 3;
    EFI_UINTN start_y = g_height * 22 / 100;
    EFI_UINTN gap_y = 142 * g_scale;

    for (uint32_t i = 0; i < 8; ++i) {
        EFI_UINTN col = i % 4;
        EFI_UINTN row = i / 4;
        EFI_UINTN x = start_x + col * (icon + gap_x);
        EFI_UINTN y = start_y + row * gap_y;
        round_rect(x, y, icon, icon, 14 * g_scale,
                   APP_COLORS[i][0], APP_COLORS[i][1], APP_COLORS[i][2]);
        char initial[2] = {APP_NAMES[i][0], 0};
        text(x + 23 * g_scale, y + 24 * g_scale, initial,
             2 * g_scale, 250, 250, 252);
        EFI_UINTN label_x = x + (icon - 8 * g_scale * 6) / 2;
        text(label_x, y + icon + 10 * g_scale, APP_NAMES[i],
             g_scale, 245, 245, 248);
    }

    round_rect(g_width * 8 / 100, g_height - 105 * g_scale,
               g_width * 84 / 100, 76 * g_scale, 24 * g_scale,
               75, 83, 98);
    text(g_width / 2 - 47 * g_scale, g_height - 75 * g_scale,
         "IOS 27.3", 2 * g_scale, 248, 248, 250);
}

static void draw_app(void)
{
    (void)fill_rect(0, 0, g_width, g_height, 14, 15, 20);
    status_bar();
    round_rect(20 * g_scale, 62 * g_scale, 92 * g_scale, 40 * g_scale,
               12 * g_scale, 54, 57, 66);
    text(43 * g_scale, 78 * g_scale, "BACK", g_scale, 245, 245, 248);

    const char *name = APP_NAMES[g_app & 7];
    text(28 * g_scale, 150 * g_scale, name, 3 * g_scale, 248, 248, 250);
    text(28 * g_scale, 205 * g_scale, "UNDER DEVELOPMENT", g_scale,
         170, 170, 180);
    text(28 * g_scale, 232 * g_scale, "SYSTEM UI PROTOTYPE", g_scale,
         130, 130, 145);
    round_rect(28 * g_scale, 300 * g_scale, g_width - 56 * g_scale,
               66 * g_scale, 16 * g_scale, 35, 37, 44);
    text(48 * g_scale, 326 * g_scale, "THIS APP IS A PLACEHOLDER",
         g_scale, 230, 230, 235);
}

static void draw_ui(void)
{
    if (g_page == 0) draw_welcome();
    else if (g_page == 1) draw_home();
    else draw_app();
}

static void draw_cursor(void)
{
    EFI_UINTN x = g_cursor_x;
    EFI_UINTN y = g_cursor_y;
    for (EFI_UINTN row = 0; row < 20 * g_scale; ++row) {
        EFI_UINTN width = row / 2 + 2 * g_scale;
        (void)fill_rect(x + row / 2, y + row, width, 2 * g_scale,
                        0, 0, 0);
    }
    for (EFI_UINTN row = 0; row < 17 * g_scale; ++row) {
        EFI_UINTN width = row / 2 + g_scale;
        (void)fill_rect(x + row / 2, y + row, width, g_scale,
                        250, 250, 252);
    }
    (void)fill_rect(x + 9 * g_scale, y + 14 * g_scale,
                    6 * g_scale, 12 * g_scale, 0, 0, 0);
    (void)fill_rect(x + 10 * g_scale, y + 15 * g_scale,
                    3 * g_scale, 9 * g_scale, 250, 250, 252);
}

static void click_at(EFI_UINTN x, EFI_UINTN y)
{
    if (g_page == 0) {
        EFI_UINTN bw = g_width * 64 / 100;
        EFI_UINTN bx = (g_width - bw) / 2;
        EFI_UINTN by = g_height - 112 * g_scale;
        if (x >= bx && x <= bx + bw && y >= by && y <= by + 52 * g_scale)
            g_page = 1;
        return;
    }

    if (g_page == 2) {
        if (x < 125 * g_scale && y < 115 * g_scale) g_page = 1;
        return;
    }

    EFI_UINTN icon = 58 * g_scale;
    EFI_UINTN start_x = g_width * 9 / 100;
    EFI_UINTN gap_x = (g_width - 2 * start_x - 4 * icon) / 3;
    EFI_UINTN start_y = g_height * 22 / 100;
    EFI_UINTN gap_y = 142 * g_scale;
    for (uint32_t i = 0; i < 8; ++i) {
        EFI_UINTN col = i % 4;
        EFI_UINTN row = i / 4;
        EFI_UINTN ix = start_x + col * (icon + gap_x);
        EFI_UINTN iy = start_y + row * gap_y;
        if (x >= ix && x <= ix + icon &&
            y >= iy && y <= iy + icon + 28 * g_scale) {
            g_app = i;
            g_page = 2;
            return;
        }
    }
}

static void emit(EFI_SYSTEM_TABLE *st, CHAR16 *s)
{
    if (st && st->ConOut && st->ConOut->OutputString)
        (void)st->ConOut->OutputString(st->ConOut, s);
}

static CHAR16 msg_gop[] = {'G','O','P','_','R','E','A','D','Y',13,10,0};
static CHAR16 msg_no_gop[] = {'G','O','P','_','N','O','T','_','F','O','U','N','D',13,10,0};
static CHAR16 msg_pointer[] = {'P','O','I','N','T','E','R','_','R','E','A','D','Y',13,10,0};
static CHAR16 msg_no_pointer[] = {'P','O','I','N','T','E','R','_','N','O','T','_','F','O','U','N','D',13,10,0};
static CHAR16 msg_ui[] = {'I','N','T','E','R','A','C','T','I','V','E','_','U','I','_','R','E','A','D','Y',13,10,0};
static CHAR16 msg_boot[] = {'I','O','S','2','7','_','B','O','O','T','_','O','K',13,10,0};
static CHAR16 msg_title[] = {'i','O','S',' ','2','7','.','3',' ','A','R','M','6','4',' ','U','I',' ','p','r','o','t','o','t','y','p','e',13,10,0};

EFI_STATUS efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table)
{
    (void)image_handle;
    if (!system_table || !system_table->BootServices) return 1;

    if (system_table->ConOut && system_table->ConOut->ClearScreen)
        (void)system_table->ConOut->ClearScreen(system_table->ConOut);
    emit(system_table, msg_title);

    EFI_STATUS status = system_table->BootServices->LocateProtocol(
        (EFI_GUID *)&GOP_GUID, 0, (void **)&g_gop
    );
    if (status || !g_gop || !g_gop->Mode || !g_gop->Mode->Info || !g_gop->Blt) {
        emit(system_table, msg_no_gop);
        return 1;
    }

    g_width = g_gop->Mode->Info->HorizontalResolution;
    g_height = g_gop->Mode->Info->VerticalResolution;
    g_scale = g_width / 800;
    if (g_scale == 0) g_scale = 1;
    if (g_height / 600 < g_scale) g_scale = g_height / 600;
    if (g_scale == 0) g_scale = 1;

    g_cursor_x = g_width / 2;
    g_cursor_y = g_height / 2;
    emit(system_table, msg_gop);

    EFI_SIMPLE_POINTER_PROTOCOL *pointer = 0;
    status = system_table->BootServices->LocateProtocol(
        (EFI_GUID *)&SIMPLE_POINTER_GUID, 0, (void **)&pointer
    );
    if (status || !pointer || !pointer->GetState) {
        pointer = 0;
        emit(system_table, msg_no_pointer);
    } else {
        emit(system_table, msg_pointer);
    }

    draw_ui();
    draw_cursor();
    emit(system_table, msg_ui);
    emit(system_table, msg_boot);

    for (;;) {
        int dirty = 0;
        if (pointer) {
            EFI_SIMPLE_POINTER_STATE state;
            if (pointer->GetState(pointer, &state) == 0) {
                int64_t nx = (int64_t)g_cursor_x + (int64_t)state.RelativeMovementX * 2;
                int64_t ny = (int64_t)g_cursor_y + (int64_t)state.RelativeMovementY * 2;
                if (nx < 0) nx = 0;
                if (ny < 0) ny = 0;
                if (nx >= (int64_t)g_width) nx = (int64_t)g_width - 1;
                if (ny >= (int64_t)g_height) ny = (int64_t)g_height - 1;
                if ((EFI_UINTN)nx != g_cursor_x || (EFI_UINTN)ny != g_cursor_y) {
                    g_cursor_x = (EFI_UINTN)nx;
                    g_cursor_y = (EFI_UINTN)ny;
                    dirty = 1;
                }
                if (state.LeftButton && !g_mouse_down) {
                    click_at(g_cursor_x, g_cursor_y);
                    dirty = 1;
                }
                g_mouse_down = state.LeftButton;
            }
        }

        if (dirty) {
            draw_ui();
            draw_cursor();
        }

        if (system_table->BootServices->Stall)
            (void)system_table->BootServices->Stall(10000);
        else
            __asm__ volatile("wfe" ::: "memory");
    }
}
