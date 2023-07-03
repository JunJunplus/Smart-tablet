#include "font.h"
#include "color.h"

struct LcdDevice *init_lcd(const char *device)
{
    // 申请空间
    struct LcdDevice *lcd = malloc(sizeof(struct LcdDevice));
    if (lcd == NULL)
    {
        return NULL;
    }

    // 1打开设备
    lcd->fd = open(device, O_RDWR);
    if (lcd->fd < 0)
    {
        perror("open lcd fail");
        free(lcd);
        return NULL;
    }

    // 映射
    lcd->mp = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd->fd, 0);

    return lcd;
}

font *init_font(int fontSize)
{
    font *f;

    f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");

    fontSetSize(f, fontSize);

    return f;
}

void acc_font(int *lcd, font *f, char *buf, int px, int py)
{
    bitmap *bm = createBitmapWithInit(280, 70, 4, COLOR_WHITE);
    fontPrint(f, bm, 0, 0, buf, COLOR_BLACK, 0);
    show_font_to_lcd(lcd, px, py, bm);
}

void passwd_font(int *lcd, font *f, char *buf, int px, int py)
{
    bitmap *bm = createBitmapWithInit(280, 70, 4, COLOR_WHITE);
    fontPrint(f, bm, 0, 0, buf, COLOR_BLACK, 0);
    show_font_to_lcd(lcd, px, py, bm);
}

void map_font(int *lcd, font *f, char *buf, int (*arr_map)[27])
{
    bitmap *bm = createBitmapWithInit(800, 480, 4, getColor(0, 0, 0, 0));
    for (int i = 0; i <= 480; i += 30)
    {
        for (int j = 0; j < 800; j += 30)
        {
            if (arr_map[i / 30][j / 30] == 1)
            {
                fontPrint(f, bm, j, i, buf, getColor(0, 255, 255, 255), 0);
            }
        }
    }

    show_font_to_lcd(lcd, 0, 0, bm);
}

void person_font(int *lcd, font *f, char *buf, int i, int j)
{
    bitmap *bm = createBitmapWithInit(30, 30, 4, getColor(0, 0, 0, 0));
    fontPrint(f, bm, 0, 0, buf, getColor(0, 0, 255, 255), 0);
    show_font_to_lcd(lcd, j * 30, i * 30, bm);
}

void ghost_font(int *lcd, font *f, char *buf, int i, int j)
{
    bitmap *bm = createBitmapWithInit(30, 30, 4, getColor(0, 0, 0, 0));
    fontPrint(f, bm, 0, 0, buf, getColor(0, 255, 0, 0), 0);
    show_font_to_lcd(lcd, j * 30, i * 30, bm);
}