#ifndef __LCD_H
#define __LCD_H

#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>



#define COLOR_BLACK     0x00000000
#define COLOR_WHITE     0x00FFFFFF
#define COLOR_RED       0x00FF0000
#define COLOR_GREEN     0x0000FF00
#define COLOR_BLUE      0x000000FF
#define COLOR_YELLOW    0x00FFFF00
#define COLOR_CYAN      0x0000FFFF
#define COLOR_MAGENTA   0x00FF00FF

//全局变量的声明
extern int *lcd_p;


//函数的声明
extern int lcd_init();
extern int lcd_destory();
int lcd_pos_size_pixel(int color,int x,int y,int width,int height);
extern bool lcd_flag;
extern void lcd_cache_change();
#endif