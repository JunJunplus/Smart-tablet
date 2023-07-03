#include "bmp.h"
#include "lcd.h"

int bmp_show(const char *fileName)
{
    // 2、打开图片
    int bmpFd = open(fileName, O_RDONLY);
    if (bmpFd == -1)
    {
        printf("%s open bmp error:%s\n", fileName, strerror(errno));
        return -1;
    }
    // 偏移文件光标位置到  第54个字节开始 ，因为从这里开始，才是图形的颜色数据
    // 文件头+信息头 ---》关于这个bmp文件的信息描述
    BITMAPFILEHEADER fileHeader;
    read(bmpFd, &fileHeader, 14);
    BITMAPINFOHEADER infoHeader;
    read(bmpFd, &infoHeader, 40);

    // printf("图片的宽度:%d 高度:%d 大小:%d 位深:%d\n",infoHeader.biWidth,
    //                                                infoHeader.biHeight,
    //                                              infoHeader.biSizeImage+54,
    //                                             infoHeader.biBitCount);

    // 3、读取图片的数据
    char bmpbuf[800 * 480 * 3] = {0};
    read(bmpFd, bmpbuf, 800 * 480 * 3);
    // 4、将读取出来的图片数据的每个像素点 BGR ---转换成 ARGB
    int tempbuf[800 * 480] = {0};
    for (int i = 0, j = 0; i < 800 * 480; i++, j += 3)
    {
        tempbuf[i] = 0x00 << 24 | bmpbuf[2 + j] << 16 | bmpbuf[1 + j] << 8 | bmpbuf[0 + j];
    }
    // 将图片的数据进行上下 180度颠倒
    // int lcdbuf[800*480] = {0};
    for (int j = 0; j < 480; j++)
    {
        for (int i = 0; i < 800; i++)
        {
            lcd_p[800 * j + i] = tempbuf[(479 - j) * 800 + i];
        }
    }

    // memcpy(lcd_p,lcdbuf,800*480*4);
    if (!lcd_flag)
    {
        lcd_cache_change();
    }

    close(bmpFd);

    return 0;
}

int bmp_show_another(const char *fileName)
{
    int *lcd_p2 = lcd_p;
    if (lcd_flag)
    {
        lcd_p2 += 800 * 480;
    }
    // 2、打开图片
    int bmpFd = open(fileName, O_RDONLY);
    if (bmpFd == -1)
    {
        printf("%s open bmp error:%s\n", fileName, strerror(errno));
        return -1;
    }
    // 偏移文件光标位置到  第54个字节开始 ，因为从这里开始，才是图形的颜色数据
    // 文件头+信息头 ---》关于这个bmp文件的信息描述
    BITMAPFILEHEADER fileHeader;
    read(bmpFd, &fileHeader, 14);
    BITMAPINFOHEADER infoHeader;
    read(bmpFd, &infoHeader, 40);

    // printf("图片的宽度:%d 高度:%d 大小:%d 位深:%d\n",infoHeader.biWidth,
    //                                                infoHeader.biHeight,
    //                                              infoHeader.biSizeImage+54,
    //                                             infoHeader.biBitCount);

    // 3、读取图片的数据
    char bmpbuf[800 * 480 * 3] = {0};
    read(bmpFd, bmpbuf, 800 * 480 * 3);
    // 4、将读取出来的图片数据的每个像素点 BGR ---转换成 ARGB
    int tempbuf[800 * 480] = {0};
    for (int i = 0, j = 0; i < 800 * 480; i++, j += 3)
    {
        tempbuf[i] = 0x00 << 24 | bmpbuf[2 + j] << 16 | bmpbuf[1 + j] << 8 | bmpbuf[0 + j];
    }
    // 将图片的数据进行上下 180度颠倒
    // int lcdbuf[800*480] = {0};
    for (int j = 0; j < 480; j++)
    {
        for (int i = 0; i < 800; i++)
        {
            lcd_p2[800 * j + i] = tempbuf[(479 - j) * 800 + i];
        }
    }

    // memcpy(lcd_p,lcdbuf,800*480*4);

    close(bmpFd);
    lcd_cache_change();
    return 0;
}

int bmp_show_swap(const char *fileName)
{
    if (lcd_flag)
    {
        bmp_show_another(fileName);
    }
    else
    {
        bmp_show(fileName);
    }
}

int bmp_show_scale(const char *fileName, int posX, int posY, int scale)
{
    // 2、打开图片
    int bmpFd = open(fileName, O_RDONLY);
    if (bmpFd == -1)
    {
        printf("%s open bmp error:%s\n", fileName, strerror(errno));
        return -1;
    }
    // 偏移文件光标位置到  第54个字节开始 ，因为从这里开始，才是图形的颜色数据
    // 文件头+信息头 ---》关于这个bmp文件的信息描述
    BITMAPFILEHEADER fileHeader;
    read(bmpFd, &fileHeader, 14);
    BITMAPINFOHEADER infoHeader;
    read(bmpFd, &infoHeader, 40);

    printf("图片的宽度:%d 高度:%d 大小:%d 位深:%d\n", infoHeader.biWidth,
           infoHeader.biHeight,
           infoHeader.biSizeImage + 54,
           infoHeader.biBitCount);

    // 3、读取图片的数据
    char bmpbuf[800 * 480 * 3] = {0};
    read(bmpFd, bmpbuf, 800 * 480 * 3);
    // 4、将读取出来的图片数据的每个像素点 BGR ---转换成 ARGB
    int tempbuf[800 * 480] = {0};
    for (int i = 0, j = 0; i < 800 * 480; i++, j += 3)
    {
        tempbuf[i] = 0x00 << 24 | bmpbuf[2 + j] << 16 | bmpbuf[1 + j] << 8 | bmpbuf[0 + j];
    }
    // 将图片的数据进行上下 180度颠倒
    int lcdbuf[800 * 480] = {0};
    for (int j = 0; j < 480; j++)
    {
        for (int i = 0; i < 800; i++)
        {
            lcdbuf[800 * j + i] = tempbuf[(479 - j) * 800 + i];
        }
    }
    // 将图片进行压缩16倍
    int minbuf[(800 / scale) * (480 / scale)];
    // int minbuf[200*120];
    for (int j = 0; j < (480 / scale); j++)
    {
        for (int i = 0; i < (800 / scale); i++)
        {
            minbuf[j * (800 / scale) + i] = lcdbuf[j * 800 * scale + i * scale];
        }
    }

    for (int j = 0; j < (480 / scale); j++)
    {
        for (int i = 0; i < (800 / scale); i++)
        {
            lcd_p[posY * 800 + posX + j * 800 + i] = minbuf[j * (800 / scale) + i];
        }
    }

    close(bmpFd);

    return 0;
}
// 在指定位置上显示指定大小（<=800 ）的图片
int bmp_show_size(const char *fileName, int posX, int posY)
{
    // 2、打开图片
    int bmpFd = open(fileName, O_RDONLY);
    if (bmpFd == -1)
    {
        printf("%s open bmp error:%s\n", fileName, strerror(errno));
        return -1;
    }
    // 偏移文件光标位置到  第54个字节开始 ，因为从这里开始，才是图形的颜色数据
    // 文件头+信息头 ---》关于这个bmp文件的信息描述
    BITMAPFILEHEADER fileHeader;
    read(bmpFd, &fileHeader, 14);
    BITMAPINFOHEADER infoHeader;
    read(bmpFd, &infoHeader, 40);

    printf("图片的宽度:%d 高度:%d 大小:%d 位深:%d\n", infoHeader.biWidth,
           infoHeader.biHeight,
           infoHeader.biSizeImage + 54,
           infoHeader.biBitCount);

    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;

    // 3、读取图片的数据
    char bmpbuf[width * height * 3];
    read(bmpFd, bmpbuf, width * height * 3);
    // 4、将读取出来的图片数据的每个像素点 BGR ---转换成 ARGB
    int tempbuf[width * height];
    for (int i = 0, j = 0; i <= width * height; i++, j += 3)
    {
        tempbuf[i] = 0x00 << 24 | bmpbuf[2 + j] << 16 | bmpbuf[1 + j] << 8 | bmpbuf[0 + j];
    }
    // 将图片的数据进行上下 180度颠倒
    int lcdbuf[width * height];
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            lcdbuf[width * j + i] = tempbuf[(height - 1 - j) * width + i];
        }
    }

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            lcd_p[posY * 800 + posX + 800 * j + i] = lcdbuf[j * width + i];
        }
    }

    close(bmpFd);

    return 0;
}

int bmp_show_double(int (*lcdbuf)[800 * 480], int x, int i)
{
    int *lcd_temp = lcd_p;

    if (i == 0)
    {
        for (int j = 0; j < 480; j++)
        {
            for (int i = 0; i < x; i++)
            {
                lcd_temp[800 * j + i] = lcdbuf[0][800 * j + i + 799 - x];
                // lcd_p[800 * j + i] = 0x0;
            }
        }
        for (int j = 0; j < 480; j++)
        {
            for (int i = x; i < 800; i++)
            {
                lcd_temp[800 * j + i] = lcdbuf[1][800 * j + i - x];
            }
        }
    }
    else
    {

        for (int j = 0; j < 480; j++)
        {
            for (int i = x; i < 800; i++)
            {
                lcd_temp[800 * j + i] = lcdbuf[0][800 * j + i - x];
            }
        }
        for (int j = 0; j < 480; j++)
        {
            for (int i = 0; i < x; i++)
            {
                lcd_temp[800 * j + i] = lcdbuf[2][800 * j + i + 799 - x];
                // lcd_p[800 * j + i] = 0x0;
            }
        }
    }
    //lcd_cache_change();
}