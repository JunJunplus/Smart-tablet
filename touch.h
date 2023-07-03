#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <pthread.h>

#include <linux/input.h>

#include "font.h"
#include "lcd.h"

/*
触摸点尺寸 1024-600
types:3 code:0 value:528    x
types:3 code:1 value:326    y
types:1 code:330 value:1    按下
types:1 code:330 value:0    松开
*/
//触摸事件种类
typedef enum touchEvKind
{
    NON,                //无事件
    PRESS,              //按下
    LOOSEN,             //松开
    CLICK,              //单击
    DCLICK,             //双击
    LSLIDE,             //左滑
    RSLIDE,             //右滑
    USLIDE,             //上滑
    DSLIDE,             //下滑
    LLSLIDE,
    RRSLIDE
}touchEvkind_t;

typedef enum sys_mode
{
    SNON,
    SACC,
    SPASSWD,
    SLOGIN,
    SCHOOSE,
    SVIDEO,
    SGAME
}sys_mode_t;

//触摸点坐标
typedef struct touchPos
{
    int x;
    int y;
}touchPos_t;

//触摸事件信息
typedef struct touchEvInfo
{
    struct timespec startTime;      //事件开始时间
    struct timespec endTime;        //事件结束时间
    touchPos_t startPos;            //开始位置
    touchPos_t endPos;              //结束位置
    int evKind;                     //事件种类
}touchEvInfo_t;

extern touchPos_t firstPos;                //记录上一次按压的位置
extern double firstTime;                   //记录上一次按压的时间
extern int touchFd;                        //触摸屏的文件描述符
extern struct input_event inEv;            //输入事件结构体
extern touchEvInfo_t evInfo;               //触摸事件信息
extern int count;                       
extern int num;
extern char password[7];
extern int correct;
extern bool login;
extern sys_mode_t sys_mode;
extern touchPos_t p;
extern bool flag;

extern font *f;


extern char acc[20];
extern char pwd[20];

//初始化
extern void touch_init();

//销毁
extern void touch_destory();

//触发触摸事件
extern int trigger_touch_event();

extern char login_touch(touchPos_t temppos);

extern bool login_user();

extern char choose_mode_touch(touchPos_t temppos);

extern int long_touch();

extern int dis_slide(struct input_event inEv);