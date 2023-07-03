#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

#include "kernel_list.h"
#include "lcd.h"
#include "touch.h"
#include "bmp.h"
#include "boot.h"
#include "photo.h"
#include "font.h"
#include "game.h"
//#include "game2.h"

int main()
{
    boot();
    touch_init();
    sys_mode = SACC;
    while (1)
    {
        bmp_show("000.bmp");
        acc_font(lcd_p, f, "", 70, 120);
        int i = 0;
        sys_mode = SNON;

        do
        {
            trigger_touch_event();
            usleep(300000);
            printf("%d\n", sys_mode);
        } while (sys_mode != SACC && sys_mode != SLOGIN);

        if (login_user())
        {
            break;
        }
    }
    // bmp_show("111.bmp");

    while (1)
    {
        bmp_show("111.bmp");

        trigger_touch_event();
        usleep(300000);

        if (sys_mode == SGAME)
        {
            printf("游戏\n");

            game_map();
        
        }
        else if (sys_mode == SVIDEO)
        {
            init_photo();
            small_photo_show(head);

            // slide_photo(head->next);
        }
        sys_mode = SLOGIN;
    }
}
