#include "lcd.h"

// 在源文件中定义全局变量
int *lcd_p;
int lcdFd;

bool lcd_flag;
struct fb_var_screeninfo vinfo; // 显卡设备的可变属性结构体

int lcd_init()
{
    // 1、打开液晶屏文件 --/dev/fb0
    lcdFd = open("/dev/fb0", O_RDWR);
    if (lcdFd == -1)
    {
        perror("open lcd error");
        return -1;
    }
    // 2进行内存映射(分配两倍缓存空间)
    lcd_p = (int *)mmap(NULL, 800 * 480 * 4 * 2, PROT_READ | PROT_WRITE, MAP_SHARED, lcdFd, 0);
    if (lcd_p == (void *)-1)
    {
        perror("mmap error");
        return -1;
    }
    // 3初始化缓存区域
    ioctl(lcdFd, FBIOGET_VSCREENINFO, &vinfo); // 获取可变属性
    vinfo.xoffset = 0;
    vinfo.yoffset = 0;
    ioctl(lcdFd, FBIOPAN_DISPLAY, &vinfo);
    lcd_flag = true;
}

void lcd_cache_change()
{
    if (lcd_flag)
    {
        //交换到另一半缓存区域
        vinfo.xoffset = 0;
        vinfo.yoffset = 480;
        ioctl(lcdFd, FBIOPAN_DISPLAY, &vinfo);
        lcd_flag = false;
    }
    else
    {
        //交换到另一半缓存区域
        vinfo.xoffset = 0;
        vinfo.yoffset = 0;
        ioctl(lcdFd, FBIOPAN_DISPLAY, &vinfo);
        lcd_flag = true;
    }
}

int lcd_destory()
{
    // 关闭液晶屏
    close(lcdFd);
    // 解除内存映射
    munmap(lcd_p, 800 * 480 * 4 * 2);
}

// 在指定位置显示指定大小的颜色数据，交换缓存，解决闪屏问题
int lcd_pos_size_pixel(int color, int x, int y, int width, int height)
{
#if 0
    for(int j=0; j<height; j++)
    {
        for(int i=0; i<width; i++)
        {
            lcd_p[y*800+x+j*800 + i] = color;
        }
    }
#endif
    if (lcd_flag)
    {
        for (int j = y + 480; j < (y + height) + 480; j++)
        {
            for (int i = x; i < (x + width); i++)
            {
                lcd_p[j * 800 + i] = color;
            }
        }
        lcd_cache_change();
    }
    else
    {
        for (int j = y; j < (y + height); j++)
        {
            for (int i = x; i < (x + width); i++)
            {
                lcd_p[j * 800 + i] = color;
            }
        }
        lcd_cache_change();
    }
}
