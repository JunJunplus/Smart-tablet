#include "touch.h"

int touchFd;
struct input_event inEv;
touchEvInfo_t evInfo;
double firstTime;
touchPos_t firstPos;
int count;
int temp;
int num;
char password[7];
int correct;
bool login;
sys_mode_t sys_mode;
touchPos_t p;
char acc[20];
char pwd[20];
font *f;

bool flag;

void touch_init()
{
    touchFd = open("/dev/input/event0", O_RDWR);
    if (touchFd == -1)
    {
        perror("open event0 fail");
        return;
    }
    // 初始化信息
    ioctl(touchFd, FIONBIO);

    evInfo.endPos.x = evInfo.endPos.y = 0;
    evInfo.startPos.x = evInfo.startPos.y = 0;
    evInfo.startTime.tv_sec = evInfo.endTime.tv_sec = 0;
    evInfo.startTime.tv_nsec = evInfo.endTime.tv_nsec = 0;
    evInfo.evKind = NON;
    sys_mode = SNON;
    firstTime = 0;
    count = 0;
    temp = 0;
    num = 0;
    correct = 0;
    login = false;
    flag = false;
    // font *f = init_font("/dev/fb0");
    // fontSetSize(f, 34);
    f = init_font(60);
    strcpy(password, "312567");
    printf("touch_init\n");
    return;
}

void touch_destory()
{
    close(touchFd);
}

void start_press()
{
    // 无事件发生 && types:3 code:0 则存储开始x
    if (evInfo.evKind == NON && inEv.type == EV_ABS && inEv.code == ABS_X)
    {
        evInfo.startPos.x = inEv.value;
    }
    // 无事件发生 && types:3 code:1 则存储开始y
    if (evInfo.evKind == NON && inEv.type == EV_ABS && inEv.code == ABS_Y)
    {
        evInfo.startPos.y = inEv.value;
    }
    // 无事件发生 && types:1 code:330 value = 1 则按键被按下
    if (evInfo.evKind == NON && inEv.type == EV_KEY && inEv.code == BTN_TOUCH && inEv.value > 0)
    {
        clock_gettime(CLOCK_MONOTONIC, &(evInfo.startTime));
        evInfo.evKind = PRESS;
    }
}

void start_lossen()
{
    // 按键被按下 && types:3 code:0 value:326   则记录结束x
    if (evInfo.evKind == PRESS && inEv.type == EV_ABS && inEv.code == ABS_X)
    {
        evInfo.endPos.x = inEv.value;
    }
    // 按键被按下 && types:3 code:1 value:326   则记录结束y
    if (evInfo.evKind == PRESS && inEv.type == EV_ABS && inEv.code == ABS_Y)
    {
        evInfo.endPos.y = inEv.value;
    }
    // 按键被按下 && types:1 code:330 value:0    松开 记录结束时间
    if (evInfo.evKind == PRESS && inEv.type == EV_KEY && inEv.code == BTN_TOUCH && inEv.value == 0)
    {
        clock_gettime(CLOCK_MONOTONIC, &(evInfo.endTime));
        evInfo.evKind = LOOSEN;
    }
}

// 延时单击函数
void *touch_click(void *c)
{
    // 如果200000微秒内没有下个事件结束则判定为单击
    touchPos_t p = evInfo.startPos;
    temp = *(int *)c + 1;
    usleep(200000);
    if (count == temp)
    {
        // evInfo.evKind = CLICK;
        printf("单击的位置是x : %d- y : %d\n", p.x, p.y);
        if (sys_mode == NON || sys_mode == SACC)
        {
            char c = login_touch(p);
            if (c != ' ')
            {
                if (num == 0)
                {
                    acc_font(lcd_p, f, "", 70, 120);
                }
                if (c == 'c')
                {
                    sys_mode = SPASSWD;
                    num = 0;
                    passwd_font(lcd_p, f, "", 70, 230);

                    return NULL;
                }
                else if (c == 'd')
                {
                    if (num > 0)
                    {
                        num--;
                        acc[num] = '\0';
                        acc_font(lcd_p, f, acc, 70, 120);
                    }
                }
                else
                {
                    acc[num] = c;
                    acc_font(lcd_p, f, acc, 70, 120);
                    num++;
                }
            }
        }
        else if (sys_mode == SPASSWD)
        {
            char c = login_touch(p);
            if (c != ' ')
            {
                if (c == 'c')
                {
                    if (strcmp(acc, "123") == 0 && strcmp(pwd, "456") == 0)
                    {
                        sys_mode = SLOGIN;
                    }
                    else
                    {
                        num = 0;
                        sys_mode = SACC;
                        memset(acc, 0, 20);
                        memset(pwd, 0, 20);
                    }
                }
                else if (c == 'd')
                {
                    if (num > 0)
                    {
                        num--;
                        pwd[num] = '\0';
                        passwd_font(lcd_p, f, pwd, 70, 230);
                    }
                }
                else
                {
                    pwd[num] = c;
                    passwd_font(lcd_p, f, pwd, 70, 230);
                    num++;
                }
            }
        }
        // if (sys_mode == SNON)
        // {
        //     printf("num : %d correct : %d\n", num, correct);
        //     if (num < 7)
        //     {
        //         char c = login_touch(p);

        //         printf("password%s, pass[num]%d, 密码%d\n", password, password[num], c);
        //         num++;

        //         if (correct == 6 && c == 'c')
        //         {

        //             login = true;
        //             return NULL;
        //         }
        //         if (password[num - 1] == c)
        //         {
        //             correct++;
        //         }
        //     }
        //     else
        //     {
        //         num = 0;
        //         correct = 0;
        //     }
        // }
        else if (sys_mode == SLOGIN)
        {
            char c = choose_mode_touch(p);
            if (c == '1')
            {
                sys_mode = SGAME;
            }
            else if (c == '2')
            {
                sys_mode = SVIDEO;
            }
        }
    }
    // else{
    //     evInfo.evKind = DCLICK;
    // }

    return NULL;
}

// 一次完整事件
bool complete_event()
{
    if (evInfo.evKind == LOOSEN)
    {
        // 存储开始到结束的时间差
        double time = evInfo.endTime.tv_sec - evInfo.startTime.tv_sec + ((evInfo.endTime.tv_nsec - evInfo.startTime.tv_nsec) / 1000 / 1000) / 1000.00; // 毫秒
        // 开始到结束的位置差
        int x = evInfo.endPos.x - evInfo.startPos.x;
        int y = evInfo.endPos.y - evInfo.startPos.y;
        // 单击或双击
        if ((evInfo.endPos.x == 0 && evInfo.endPos.x == 0) || (abs(x) < 20 && abs(y) < 20))
        {
            // 双击
            if (((evInfo.startTime.tv_sec + evInfo.startTime.tv_nsec / (double)(1000 * 1000 * 1000)) - firstTime) < 0.3 && abs(firstPos.x - evInfo.startPos.x) < 40 && abs(firstPos.y - evInfo.startPos.y) < 40)
            {
                p = evInfo.startPos;
                evInfo.evKind = DCLICK;
            }
            // 单击
            else if (time < 0.30)
            {
                pthread_t tid;
                pthread_create(&tid, NULL, touch_click, &count); // 延时执行单击函数
                usleep(120);
                pthread_detach(tid);
            }
            firstTime = evInfo.startTime.tv_sec + evInfo.startTime.tv_nsec / (double)(1000 * 1000 * 1000);
            firstPos.x = evInfo.startPos.x;
            firstPos.y = evInfo.startPos.y;
        }
        // 滑动情况
        else if (abs(x) > 20 || abs(y) > 20)
        {
            if (x > 0 && x > abs(y))
            {
                evInfo.evKind = RSLIDE;
            }
            else if ((x < 0 && abs(x) > abs(y)))
            {
                evInfo.evKind = LSLIDE;
            }
            else if ((y > 0 && x > abs(x) < abs(y)))
            {
                evInfo.evKind = DSLIDE;
            }
            else if ((y < 0 && x > abs(x) < abs(y)))
            {
                evInfo.evKind = USLIDE;
            }
        }
        return true;
    }

    return false;
}

int trigger_touch_event()
{
    evInfo.endPos.x = evInfo.endPos.y = 0;
    evInfo.startPos.x = evInfo.startPos.y = 0;
    evInfo.evKind = NON;
    while (1)
    {
        read(touchFd, &inEv, sizeof(inEv));
        p.x = dis_slide(inEv);
        
        if (p.x != -1)
        {
            printf("touch.c p.x :  %d\n", p.x);
        }
        start_press();
        start_lossen();
        if (complete_event())
        {
            count++;
            break;
        }
    }
    return evInfo.evKind;
}

char login_touch(touchPos_t temppos)
{
    touchPos_t pos;
    pos.x = temppos.y * 800 / 1042;
    pos.y = temppos.x * 480 / 600;

    if (pos.x > 100 && pos.x < 180 && pos.y > 420 && pos.y < 510)
    {
        return '1';
    }
    else if (pos.x > 100 && pos.x < 180 && pos.y > 520 && pos.y < 610)
    {
        return '2';
    }
    else if (pos.x > 100 && pos.x < 180 && pos.y > 630 && pos.y < 720)
    {
        return '3';
    }
    else if (pos.x > 210 && pos.x < 280 && pos.y > 420 && pos.y < 510)
    {
        return '4';
    }
    else if (pos.x > 210 && pos.x < 280 && pos.y > 520 && pos.y < 610)
    {
        return '5';
    }
    else if (pos.x > 210 && pos.x < 280 && pos.y > 630 && pos.y < 720)
    {
        return '6';
    }
    else if (pos.x > 300 && pos.x < 360 && pos.y > 420 && pos.y < 510)
    {
        return '7';
    }
    else if (pos.x > 300 && pos.x < 360 && pos.y > 520 && pos.y < 610)
    {
        return '8';
    }
    else if (pos.x > 300 && pos.x < 360 && pos.y > 630 && pos.y < 720)
    {
        return '9';
    }
    else if (pos.x > 380 && pos.x < 430 && pos.y > 430 && pos.y < 550)
    {
        return 'd';
    }
    else if (pos.x > 380 && pos.x < 430 && pos.y > 600 && pos.y < 730)
    {
        return 'c';
    }
    else
    {
        return ' ';
    }
}

char choose_mode_touch(touchPos_t temppos)
{
    touchPos_t pos;
    pos.x = temppos.y * 800 / 1042;
    pos.y = temppos.x * 480 / 600;

    printf("单击的位置是x : %d- y : %d", pos.x, pos.y);
    if (pos.x > 10 && pos.x < 110 && pos.y > 10 && pos.y < 110)
    {
        return '1';
    }
    else if (pos.x > 10 && pos.x < 110 && pos.y > 200 && pos.y < 300)
    {
        return '2';
    }
}

bool login_user()
{
    usleep(300000);
    if (sys_mode == SLOGIN)
    {
        printf("登录成功\n");
        return true;
    }
    else
    {
        printf("登陆失败,重新输入\n");
        return false;
    }
    return false;
}

int long_touch()
{
    ioctl(touchFd, FIONBIO);

    if (read(touchFd, &inEv, sizeof(inEv)) != 0)
    {
        if (!flag && evInfo.evKind != PRESS)
        {
            start_press();
            evInfo.evKind == PRESS;
            flag = true;
        }

        if (inEv.type == EV_KEY && inEv.code == BTN_TOUCH && inEv.value == 0)
        {
            evInfo.startPos.x = 0;
            evInfo.startPos.y = 0;
            flag = false;
            evInfo.evKind == NON;
            return -1;
        }

        if (evInfo.evKind == PRESS && inEv.type == EV_ABS && inEv.code == ABS_Y)
        {
            evInfo.startPos.y = inEv.value;
        }
        //按键被按下 && types:1 code:330 value:0    松开 记录结束时间
        if (evInfo.evKind == PRESS && inEv.type == EV_KEY && inEv.code == BTN_TOUCH && inEv.value == 0)
        {
            evInfo.evKind == NON;
        }

        if (inEv.type == EV_ABS && inEv.code == ABS_X)
        {
            p.x = inEv.value* 800 / 1024;
        }
        if (inEv.type == EV_ABS && inEv.code == ABS_Y)
        {
            p.y = inEv.value* 480 / 600;
        }
    }
    if((p.x > 750 && p.y < 50))
        return -2;
    else
        return abs( p.x);
        //return abs(evInfo.startPos.x * 800 / 1024 - p.x);
}

int dis_slide(struct input_event inEv)
{
    if (inEv.type == EV_ABS && inEv.code == ABS_X)
    {
        return inEv.value * 800 / 1024;
    }
    else
    {
        return -1;
    }
}