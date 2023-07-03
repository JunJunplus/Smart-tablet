#include "boot.h"


int endWith(char *originString, char *end)
{
    // 参数校验
    if (originString == NULL || end == NULL || strlen(end) > strlen(originString))
    {
        // printf("参数异常，请重新输入！\n");
        return 0;
    }

    int n = strlen(end);
    int m = strlen(originString);
    int i;
    for (i = 0; i < n; i++)
    {
        if (originString[m - i - 1] != end[n - i - 1])
        {
            return 0;
        }
    }
    return 1;
}

void boot()
{
    DIR *dir = opendir("./boot_bmp");
    if (dir == NULL)
    {
        perror("opendir fail");
    }    

    char fileWithEnd[] = ".bmp";

    struct list_head *head = malloc(sizeof(struct list_head));
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

    Node_t *pos, n;
    // 打印目录下bmp文件
    list_for_each_entry(pos, head, list)
    {
        printf("%s 文件已打开\n", pos->fileName);
        char filename[40] = "./boot_bmp/";
        strcpy(pos->fileName, strcat(filename, pos->fileName));
        pos->fd = open(pos->fileName, O_RDWR);
        if (pos->fd == -1)
        {
            perror("open fail!!");
            return;
        }
    }

    lcd_init();

    list_for_each_entry(pos, head, list)
    {
        bmp_show_size(pos->fileName,0,0);
        printf("%s\n", pos->fileName);
        usleep(20*1000);
    }

    //lcd_destory();
}
