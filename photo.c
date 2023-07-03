#include "photo.h"
struct list_head *head;
bool slide_open;

void init_photo()
{
    DIR *dir = opendir("./photo");
    if (dir == NULL)
    {
        perror("opendir fail");
    }

    char fileWithEnd[] = ".bmp";

    head = malloc(sizeof(struct list_head));
    INIT_LIST_HEAD(head);
    int count = 0;
    struct dirent *dirinfo;

    while (1)
    {
        dirinfo = readdir(dir);
        if (dirinfo == NULL)
        {
            break;
        }
        if (dirinfo->d_type == DT_REG && endWith(dirinfo->d_name, fileWithEnd))
        {
            printf("%s\n", dirinfo->d_name);
            Node_t *node = malloc(sizeof(Node_t));
            strcpy(node->fileName, dirinfo->d_name);
            list_add_tail(&(node->list), head);
            count++;
        }
    }

    Node_t *pos, *n;
    // 打印目录下bmp文件
    list_for_each_entry(pos, head, list)
    {
        printf("%s 文件已打开\n", pos->fileName);
        char filename[40] = "./photo/";
        strcpy(pos->fileName, strcat(filename, pos->fileName));
        pos->fd = open(pos->fileName, O_RDWR);
        if (pos->fd == -1)
        {
            perror("open fail!!");
            return;
        }
    }
}

void small_photo_show(struct list_head *phead)
{
    int mode = 0;

    int i = 0, j = 0, count = 0;
    struct list_head *arr[4];
    while (count % 4 == 0)
    {

        bmp_show("112.bmp");
        for (i = 0; i < 800; i += 400)
        {
            for (j = 0; j < 480; j += 240)
            {
                printf("count : %d\n", count);
                if (phead != head)
                {
                    arr[count] = phead;
                    count = (count + 1) % 4;
                    bmp_show_scale(list_entry(phead, Node_t, list)->fileName, i, j, 3);
                }
                else
                {
                    phead = phead->next;
                    arr[count] = phead;
                    count = (count + 1) % 4;
                    bmp_show_scale(list_entry(phead, Node_t, list)->fileName, i, j, 3);
                }
                phead = phead->next;
            }
        }
        while (1)
        {
            int mode = trigger_touch_event();
            // usleep(300000);
            printf("mode : %d\n", mode);
            printf("x : %d - y : %d", evInfo.startPos.x, evInfo.startPos.y);
            if (mode == USLIDE)
            {
                printf("return");
                return;
            }

            if (LSLIDE == mode)
            {
                phead = phead->next;
                break;
            }
            else if (RSLIDE == mode)
            {
                phead = arr[0]->prev->prev->prev->prev->prev;
                break;
            }
            else if (mode == DCLICK)
            {

                if (p.x < 400 && p.y < 240)
                    return slide_photo(arr[0]);
                else if (p.x > 400 && p.y < 240)
                    return slide_photo(arr[2]);
                else if (p.x < 400 && p.y > 240)
                    return slide_photo(arr[1]);
                else if (p.x > 400 && p.y > 240)
                    return slide_photo(arr[3]);
            }
        }
    }

    // while(1)
    // {
    //     mode = trigger_touch_event();
    //     switch (mode)
    //     {
    //     // case CLICK:
    //     //     printf("[单击]\n");
    //     //     break;
    //     case DCLICK:
    //         printf("[[双击]]\n");
    //         break;
    //     case LSLIDE:
    //         phead = phead->prev;
    //         if (phead == head)
    //             phead = phead->prev;
    //         bmp_show_swap(list_entry(phead, Node_t, list)->fileName);
    //         printf("显式图片的文件名字：%s\n", list_entry(phead, Node_t, list)->fileName);

    //         break;
    //     case RSLIDE:
    //         phead = phead->next;
    //         if (phead == head)
    //             phead = phead->next;
    //         bmp_show_swap(list_entry(phead, Node_t, list)->fileName);

    //         printf("显式图片的文件名字：%s\n", list_entry(phead, Node_t, list)->fileName);
    //         break;
    //     case DSLIDE:
    //         printf("下滑v\n");
    //         break;
    //     case USLIDE:
    //         printf("上滑^\n");
    //         break;
    //     default:
    //         break;
    //     }
    // }
}

void photo_show(struct list_head *phead)
{
    int mode = 0;
    bmp_show_swap(list_entry(phead, Node_t, list)->fileName);
    while (1)
    {
        if (evInfo.evKind == PRESS)
        {
            printf("photo.c 177 PRESS YES\n");
            slide_photo(phead);
        }
        mode = trigger_touch_event();
        printf("mode : %d", mode);

        if (mode == DCLICK)
        {
            slide_open = false;
            return small_photo_show(phead);
        }
        switch (mode)
        {
        // case CLICK:
        //     printf("[单击]\n");
        //     break;
        // case PRESS:
        case DCLICK:
            printf("[[双击]]\n");
            break;
        case LSLIDE:

            break;
        case RSLIDE:
            break;
        case DSLIDE:
            phead = phead->prev;
            if (phead == head)
                phead = phead->prev;
            bmp_show_swap(list_entry(phead, Node_t, list)->fileName);
            printf("显式图片的文件名字：%s\n", list_entry(phead, Node_t, list)->fileName);

            break;
        case USLIDE:
            phead = phead->next;
            if (phead == head)
                phead = phead->next;
            bmp_show_swap(list_entry(phead, Node_t, list)->fileName);

            printf("显式图片的文件名字：%s\n", list_entry(phead, Node_t, list)->fileName);
            break;
        default:
            break;
        }
    }
}

void double_photo(struct list_head *phead)
{
    char *file1, *file2, *file3;

    lcdbuf = (int(*)[800 * 480]) malloc(800 * 480 * 4 * 3);
    // if (phead != head)
    // {
        file1 = list_entry(phead, Node_t, list)->fileName;
        if (phead->prev != head)
        {
            file3 = list_entry(phead->prev, Node_t, list)->fileName;
        }
        else
        {
            file3 = list_entry(phead->prev->prev, Node_t, list)->fileName;
        }

        //phead = phead->next;
        if (phead->next != head)
        {
            file2 = list_entry(phead->next, Node_t, list)->fileName;
        }
        else
        {
            phead = phead->next;
            file2 = list_entry(phead->next, Node_t, list)->fileName;
            // if (phead->prev->prev != head)
            // {
            //     file3 = list_entry(phead->prev->prev, Node_t, list)->fileName;
            // }
            // else
            // {
            //     file3 = list_entry(phead->prev->prev->prev, Node_t, list)->fileName;
            // }
        }
    // }
    // else
    // {
    //     phead = phead->next;
    //     file1 = list_entry(phead, Node_t, list)->fileName;
    // }

    int bmpFd = open(file1, O_RDONLY);
    if (bmpFd == -1)
    {
        printf("%s open bmp error:%s\n", file1, strerror(errno));
        return;
    }

    lseek(bmpFd, 54, SEEK_SET);
    int width = 800, height = 480;

    // 3、读取图片的数据
    char bmpbuf[width * height * 3];
    read(bmpFd, bmpbuf, width * height * 3);
    // 4、将读取出来的图片数据的每个像素点 BGR ---转换成 ARGB
    int tempbuf[width * height];
    for (int i = 0, j = 0; i <= width * height; i++, j += 3)
    {
        tempbuf[i] = 0x00 << 24 | bmpbuf[2 + j] << 16 | bmpbuf[1 + j] << 8 | bmpbuf[0 + j];
    }

    for (int j = 0; j < 480; j++)
    {
        for (int i = 0; i < 800; i++)
        {
            lcdbuf[0][800 * j + i] = tempbuf[(479 - j) * 800 + i];
        }
    }

    int bmpFd2 = open(file2, O_RDONLY);
    if (bmpFd2 == -1)
    {
        printf("%s open bmp error:%s\n", file2, strerror(errno));
        return;
    }
    // 偏移文件光标位置到  第54个字节开始 ，因为从这里开始，才是图形的颜色数据
    // 文件头+信息头 ---》关于这个bmp文件的信息描述
    lseek(bmpFd2, 54, SEEK_SET);

    // 3、读取图片的数据
    read(bmpFd2, bmpbuf, width * height * 3);
    // 4、将读取出来的图片数据的每个像素点 BGR ---转换成 ARGB

    for (int i = 0, j = 0; i <= width * height; i++, j += 3)
    {
        tempbuf[i] = 0x00 << 24 | bmpbuf[2 + j] << 16 | bmpbuf[1 + j] << 8 | bmpbuf[0 + j];
    }

    for (int j = 0; j < 480; j++)
    {
        for (int i = 0; i < 800; i++)
        {
            lcdbuf[1][800 * j + i] = tempbuf[(479 - j) * 800 + i];
            // lcdbuf[1][800 * j + i] = 0x00FFFFFF;
        }
    }

    int bmpFd3 = open(file3, O_RDONLY);
    if (bmpFd3 == -1)
    {
        printf("%s open bmp error:%s\n", file2, strerror(errno));
        return;
    }
    // 偏移文件光标位置到  第54个字节开始 ，因为从这里开始，才是图形的颜色数据
    // 文件头+信息头 ---》关于这个bmp文件的信息描述
    lseek(bmpFd3, 54, SEEK_SET);

    // 3、读取图片的数据
    read(bmpFd3, bmpbuf, width * height * 3);
    // 4、将读取出来的图片数据的每个像素点 BGR ---转换成 ARGB

    for (int i = 0, j = 0; i <= width * height; i++, j += 3)
    {
        tempbuf[i] = 0x00 << 24 | bmpbuf[2 + j] << 16 | bmpbuf[1 + j] << 8 | bmpbuf[0 + j];
    }

    for (int j = 0; j < 480; j++)
    {
        for (int i = 0; i < 800; i++)
        {
            lcdbuf[2][800 * j + i] = tempbuf[(479 - j) * 800 + i];
            // lcdbuf[1][800 * j + i] = 0x00FFFFFF;
        }
    }
}

void slide_photo(struct list_head *phead)
{
    int mode = 0;
    int start = 0, last = 0;
    double_photo(phead);
    bool flag_slide = true;
    int x = 0;
    bmp_show_double(lcdbuf, 0, 1);
    p.x = 0;
    while (1)
    {
        // int mode = trigger_touch_event();
        while (1)
        {
            x = long_touch();
            //printf("photo : x%d\n", x);
            if (x!= 0 && x != -1 && x != last && x != -2)
            {
                if (flag_slide)
                {
                    start = x;
                    flag_slide = false;
                }
                printf("x 356: %d\n", p.x);
                if (start > 400)
                    bmp_show_double(lcdbuf, x, 0);
                else
                    bmp_show_double(lcdbuf, x, 1);
                last = x;
                // printf("显式图片的文件名字：%s\n", list_entry(phead, Node_t, list)->fileName);
            }
            if (x == -1)
            {
                p.x = 0;
                 //flag_slide = true;
                break;
            }
            else if (x == -2)
            {
                small_photo_show(phead);
                return;
            }
        }
        if (!flag_slide)
        {
            printf("start : %d last : %d\n", start, last);
            if (start > 400 && last < 400)
            {
                for (int i = last; i > 0; i -= 10)
                {
                    bmp_show_double(lcdbuf, i, 0);
                }
                bmp_show_double(lcdbuf, 0, 0);
                phead = phead->next;
                if (phead == head)
                    phead = phead->next;
                free(lcdbuf);
                double_photo(phead);
            }
            if (start < 400 && last > 400)
            {
                for (int i = last; i < 800; i += 10)
                {
                    bmp_show_double(lcdbuf, i, 1);
                }
                bmp_show_double(lcdbuf, 800, 1);
                phead = phead->prev;
                if (phead == head)
                    phead = phead->prev;
                free(lcdbuf);
                double_photo(phead);
            }
            if (start > 400 && last > 400)
            {
                for (int i = last; i < 800; i += 10)
                {
                    bmp_show_double(lcdbuf, i, 0);
                }
                bmp_show_double(lcdbuf, 800, 0);
            }
            if (start < 400 && last < 400)
            {
                for (int i = last; i >= 0; i -= 10)
                {
                    bmp_show_double(lcdbuf, i, 1);
                }
                bmp_show_double(lcdbuf, 0, 1);
            }
            flag_slide = true;
        }
        if (mode == USLIDE)
            break;
        // sleep(3);
    }
}

void *slide_photo_pth(void *phead)
{
    if (slide_open)
    {
        slide_photo((struct list_head *)phead);
    }
}