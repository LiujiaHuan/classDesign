#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cJSON.h"
#include "cJSON.c"

#define MAX_LINE 4096
// userInfo结构体 保存用户信息
typedef struct userInfo
{
    char *userName;
    char *userPasswd;
    int priority;
} userInfo;

typedef struct userAffair
{
    char *time;
    char *affair;
} userAffair;

typedef struct affairList
{
    struct affairList *next;
    struct affairList *pre;
    userAffair *current;
} affairList;

// userInfoList 用户信息的链表 data指向用户信息结构体
typedef struct userInfoList
{
    struct userInfoList *next;
    struct userInfoList *pre;
    userInfo *current;
} userInfoList;

/*创建一个UserInfoList的Node*/
userInfoList *createUINode()
{
    userInfoList *temp = (userInfoList *)malloc(sizeof(userInfoList));
    temp->current = NULL;
    temp->next = NULL;
    temp->pre = NULL;
    return temp;
}

/*创建一个AffairList的Node*/
affairList *createUALnode()
{
    affairList *temp = (affairList *)malloc(sizeof(affairList));
    temp->current = NULL;
    temp->next = NULL;
    temp->pre = NULL;
    return temp;
}

/*创建一个UserInfo结构体*/
userInfo *createUserInfo(char *name, char *passwd, int priority)
{
    userInfo *Temp = (userInfo *)malloc(sizeof(userInfo));
    Temp->userName = name;
    Temp->userPasswd = passwd;
    Temp->priority = priority;
    return Temp;
}

userAffair *createAffair(char *time, char *affair)
{
    userAffair *Temp = (userAffair *)malloc(sizeof(userAffair));
    Temp->time = time;
    Temp->affair = affair;
    printf("create Affair %s %s \n", Temp->affair, Temp->time);
    return Temp;
}

/*为userInfoList每一个User维护一个json文件存放事务*/
void createFile(char *name)
{
    FILE *fp;
    if ((fp = fopen(name, "w")) == NULL) /*建立新文件 */
    {
        printf("cannot open file\n"); /*建立新文件出错误信息*/
        exit(1);                      /*终止调用过程、关闭所有文件*/
    }
    fclose(fp);
}

/*获取文件全部内容 返回一个指针*/
char *getFileAll(char *fname)
{
    FILE *fp;
    char *str;
    char txt[1000];
    int filesize;
    if ((fp = fopen(fname, "r")) == NULL)
    {
        printf("打开文件%s错误\n", fname);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);

    filesize = ftell(fp);
    str = (char *)malloc(filesize);
    str[0] = 0;

    rewind(fp);
    while ((fgets(txt, 1000, fp)) != NULL)
    {
        strcat(str, txt);
    }
    fclose(fp);
    return str;
}

/*初始化一个userInfoList*/
userInfoList *initUserInfoList()
{
    userInfoList *beginNode = createUINode();
    return beginNode;
};

/*初始化一个AffairList*/
affairList *initAffairList()
{
    affairList *beginNode = createUALnode();
    return beginNode;
};

/*给userInfoList尾插新节点*/
userInfoList *appendUserInfo(userInfoList *L, userInfo *newInfo)
{
    // printf("222\n");
    userInfoList *root = L; //保存一下根节点 一会return回去
    while (L->next != NULL)
    {
        // printf("111\n");
        L = L->next; //遍历到存有数据的最后一个节点
    }
    userInfoList *newNode = createUINode();
    L->next = newNode;
    newNode->current = newInfo;
    newNode->next = NULL;
    newNode->pre = L;
    return root;
}

/*遍历user AffairList表*/
void iterAffairList(affairList *L)
{
    if (L->next == NULL)
    {
        printf("***当前列表没有事件！***");
        return;
    }
    L = L->next; //越过 伪头节点
    while (L != NULL)
    {
        printf("time: %s affair:  %s \n", L->current->time, L->current->affair);
        L = L->next;
    }
}

/*给affairList尾插新节点*/
affairList *appendAffairList(affairList *L, userAffair *newInfo)
{
    // printf("222\n");
    affairList *root = L; //保存一下根节点 一会return回去
    while (root->next != NULL)
    {
        // printf("111\n");
        root = root->next; //遍历到存有数据的最后一个节点
    }
    affairList *newNode = createUALnode();
    root->next = newNode;
    newNode->current = newInfo;
    newNode->next = NULL;
    newNode->pre = root;

    printf("-------------\n");
    iterAffairList(L);
    printf("--------------\n");
    return L;
}

/*遍历userInfoList表*/
void iterUserInfoList(userInfoList *L)
{
    L = L->next; //越过 伪头节点
    while (L != NULL)
    {
        printf("userId: %s passwd: %s  priority: %d\n", L->current->userName, L->current->userPasswd, L->current->priority);
        L = L->next;
    }
}

/* 初始化读取并维护一个userInfoList链表*/
userInfoList *initUserInfo()
{
    userInfoList *L = initUserInfoList();
    // if(L->next==NULL){
    //     printf("?????");
    // }
    char *message = getFileAll("./user.json");

    cJSON *cjson_test = NULL;
    cJSON *cjson_userId = NULL;
    cJSON *cjson_passwd = NULL;
    cJSON *cjson_priority = NULL;
    int skill_array_size = 0, i = 0;
    cJSON *cjson_skill_item = NULL;

    /* 解析整段JSON数据 */
    cjson_test = cJSON_Parse(message);
    if (cjson_test == NULL)
    {
        printf("parse fail.\n");
        return L;
    }

    if (!cjson_test)
    {
        printf("no json\n");
        return L;
    }
    if (!cJSON_IsArray(cjson_test))
    {
        printf("no Array\n");
        return L;
    }
    /* 解析数组 */

    skill_array_size = cJSON_GetArraySize(cjson_test);
    // printf("%d",skill_array_size);
    for (i = 0; i < skill_array_size; i++)
    {
        cjson_skill_item = cJSON_GetArrayItem(cjson_test, i);
        /* 依次根据名称提取JSON数据（键值对） */
        cjson_userId = cJSON_GetObjectItem(cjson_skill_item, "userId");
        cjson_passwd = cJSON_GetObjectItem(cjson_skill_item, "passwd");
        cjson_priority = cJSON_GetObjectItem(cjson_skill_item, "priority");
        //根据json文件创建新的userInfo
        userInfo *newUserInfo = createUserInfo(cjson_userId->valuestring, cjson_passwd->valuestring, cjson_priority->valueint);
        L = appendUserInfo(L, newUserInfo); //根据json解析数据添加节点
        // createFile(cjson_userId->valuestring);
    }
    // iterUserInfoList(L);
    return L;
};

//初始化 UserAffair表
affairList *initUserAffair(userInfo *U)
{
    char path[30] = "./jsonFile/";
    strcat(path, U->userName);
    strcat(path, ".json");
    char *fileContent = getFileAll(path);
    affairList *L = initAffairList();

    printf("解析 %s.json ", U->userName);
    cJSON *cjson_test = NULL;
    cJSON *cjson_time = NULL;
    cJSON *cjson_affair = NULL;
    int skill_array_size = 0, i = 0;
    cJSON *cjson_skill_item = NULL;

    /* 解析整段JSON数据 */
    cjson_test = cJSON_Parse(fileContent);
    if (cjson_test == NULL)
    {
        printf("parse fail.\n");
        return L;
    }

    if (!cjson_test)
    {
        printf("no json\n");
        return L;
    }
    if (!cJSON_IsArray(cjson_test))
    {
        printf("no Array\n");
        return L;
    }
    /* 解析数组 */

    skill_array_size = cJSON_GetArraySize(cjson_test);
    // printf("%d",skill_array_size);
    for (i = 0; i < skill_array_size; i++)
    {
        cjson_skill_item = cJSON_GetArrayItem(cjson_test, i);
        /* 依次根据名称提取JSON数据（键值对） */
        cjson_time = cJSON_GetObjectItem(cjson_skill_item, "time");
        cjson_affair = cJSON_GetObjectItem(cjson_skill_item, "affair");
        //根据json文件创建新的userInfo
        userAffair *newAffair = createAffair(cjson_time->valuestring, cjson_affair->valuestring);
        L = appendAffairList(L, newAffair); //根据json解析数据添加节点
    }
    return L;
};

//删除 用户事件
affairList *deleteAffair(affairList *L, char *affair)
{
    int flag = 0;
    affairList *root = L;
    L = L->next;
    while (L != NULL)
    {
        if (!strcmp((L->current->affair), affair))
        {
            if (L->next != NULL)
            {
                L->pre->next = L->next;
                L->next->pre = L->pre;
            }
            else
            { //处理 当前节点是尾结点的情况
                L->pre->next = L->next;
            }
            free(L->current);
            free(L);
            flag = 1; //限制  一次调用只能删除一项内容  防止引起其他难处理的问题
            printf("-------------删除完毕！-------------\n");
            break;
        }
        L = L->next;
    }
    if (!flag)
    {
        printf("-------------没有这个事件-------------\n");
    }
    return root;
}

//删除用户
void deleteUser(userInfoList *L, char *affair)
{
    int flag = 0;
    L = L->next; //越过 伪头节点
    while (L != NULL)
    {
        if (!strcmp((L->current->userName), affair))
        {
            if (L->next != NULL)
            {
                L->pre->next = L->next;
                L->next->pre = L->pre;
            }
            else
            {
                L->pre->next = NULL; //处理 当前节点是尾结点的情况
            }
            printf("ok\n");
            free(L->current);
            free(L);
            flag = 1;
            break;
        }
        L = L->next;
    }
    if (!flag)
    {
        printf("-------------没有这个事件-------------\n");
    }
    return;
}

//检查用户登录，并根据用户json文件初始化用户affair链表,然后开始监听log进行操作
void userLogin(char *userName, char *passwd, userInfoList *L, int fflag)
{
    int flag = 0; //验证登录标志位
    L = L->next;  //越过 伪头节点

    //检查登录账号密码是否正确
    while (L != NULL)
    {
        if (!strcmp(L->current->userName, userName) && !strcmp(L->current->userPasswd, passwd))
        {
            printf("--------------登陆成功！欢迎%s-------------------\n", userName);
            flag = 1;
            break;
        }
        L = L->next;
    }
    if (!flag)
    {
        printf("密码错误！");
        return;
    }
    //根据登录用户初始化 事件链表
    affairList *Affair = initUserAffair(L->current);

    //前后端分离模式
    while (fflag == 0)
    {
        // iterAffairList(Affair);
        printf("Listening\n");
        int fd = open("log.txt", O_RDWR);
        const char s[2] = "-";
        char *token;
        char *time, *affair;
        char temp[20] = {0};
        int len = read(fd, temp, 20);

        if (temp[0] == '1')
        {
            printf("***检测到新增事件***%s\n", temp);
            token = strtok(temp, s);
            token = strtok(NULL, s);
            time = (char *)malloc(sizeof(token));
            memcpy(time, token, sizeof(token));

            token = strtok(NULL, s);
            affair = (char *)malloc(sizeof(token));
            memcpy(affair, token, sizeof(token));

            //添加新事件
            userAffair *new = createAffair(time, affair);
            printf("new affair is %s %s\n", new->affair, new->time);
            //新事件添加到事件列表
            Affair = appendAffairList(Affair, new);
        }

        if (temp[0] == 'd')
        {
            printf("***检测到删除事件***\n");
            token = strtok(temp, s);
            token = strtok(NULL, s);
            time = (char *)malloc(sizeof(token));
            memcpy(time, token, sizeof(token));

            token = strtok(NULL, s);
            affair = (char *)malloc(sizeof(token));
            memcpy(affair, token, sizeof(token));

            deleteAffair(Affair, affair);
        }
        ftruncate(fd, 0);
        close(fd);
        sleep(1);
    }
    int dowhat;
    //前后端不分离
    while (fflag == 1)
    {
        char time[20], affair[20], temp[30], s[2] = "-";
        char *token, *Time, *AAffair;
        printf("接下来要做什么？\n----------1 查看所有事项 2 增加一个事项 3删除一个事项 4退出------------\n");
        scanf("%d", &dowhat);
        switch (dowhat)
        {
        case 1:
            iterAffairList(Affair);
            break;
        case 2:
            printf("请输入时间\n");
            scanf("%s", &time);
            printf("请输入事件\n");
            scanf("%s", &affair);
            userAffair *new = createAffair(time, affair);
            Affair = appendAffairList(Affair, new);
            printf("已新建事件：%s  %s\n", new->time, new->affair);
            break;
        case 3:
            printf("请输入你要删除哪个事件？\n");
            scanf("%s", &temp);
            Affair = deleteAffair(Affair, temp);
            break;
        case 4:
            printf("-----good bye %s--------\n", userName);
            return;
        }
    }
}

int main()
{
    createFile("./log.txt");
    int flag = 0;
    userInfoList *user = initUserInfo();
    iterUserInfoList(user);
    

    
    
    //链表写回测试
    
    
    printf("请输入运行方式 0 纯后端 1 前后端分离");
    scanf("%d", &flag);
    while (!flag)
    {
        char userName[20], passwd[20], name[10] = {0};
        char *Name, *Passwd;
        int funcFlag;
        printf("你要干什么?\n ----------1是注册奥 2是登录 3查看全部用户名单 4删除用户---------------\n");
        scanf("%d", &funcFlag);
        if (funcFlag > 5 || funcFlag < 0)
        {
            printf("error");
            exit(1);
        }
        switch (funcFlag)
        {
        //注册用户
        case 1:
            printf("ok 请输入你的用户名称\n");
            scanf("%s", &name);
            Name = (char *)malloc(sizeof(name));
            memcpy(Name, name, sizeof(name));
            printf("ok 请输入你的密码\n");
            scanf("%s", &passwd);

            Passwd = (char *)malloc(sizeof(passwd));
            memcpy(Passwd, passwd, sizeof(passwd));

            printf("ok 请确认你的密码\n");
            //创建新用户信息
            userInfo *new = createUserInfo(Name, Passwd, 10);
            //用户链表追加节点 保存用户信息（user->current指向用户信息结构体）
            user = appendUserInfo(user, new);
            printf("-----------注册成功--------------");
            break;

        //登录
        case 2:
            printf("请输入你的用户名！\n");
            scanf("%s", &userName);
            printf("请输入你的密码\n");
            scanf("%s", &passwd);

            //进入login状态    开始下一阶段循环
            userLogin(userName, passwd, user, 1);
            break;

        //查看所有用户
        case 3:
            iterUserInfoList(user);
            break;

        //删除用户
        case 4:
            printf("输入你要删除的用户");
            scanf("%s", &name);
            printf("输入账户密码");
            scanf("%s", &passwd);
            int flag = 0; //验证登录标志位
            userInfoList *L = user;
            L = L->next; //越过 伪头节点

            //检查登录账号密码是否正确
            while (L != NULL)
            {
                if (!strcmp(L->current->userName, name) && !strcmp(L->current->userPasswd, passwd))
                {
                    printf("删除%s\n", name);
                    flag = 1;
                    break;
                }
                L = L->next;
            }
            if (!flag)
            {
                printf("密码错误！");
            }
            if (flag)
            {
                deleteUser(user, name);
                printf("------------删除完毕！----------");
            }
            break;
        }
    }

    //前后端分离模式
    while (flag)
    {
        printf("---Listening---\n");
        char *username,*passWd,*token,temp[30] = {0};
        const char s[2] = "-";
        int fd = open("log.txt", O_RDWR);
        int len = read(fd, temp, 30);

        switch (temp[0]){
            case 'l':
            printf("检测到登录事件");
            token = strtok(temp, s); //操作"位”
            token = strtok(NULL, s); // user 信息
            username = token;
            token = strtok(NULL, s); // passwd信息
            passWd = token;
            printf("username: %s passwd %s", username, passWd);
            ftruncate(fd, 0);
            close(fd);

            //进入login状态   开始下一阶段循环
            userLogin(username, passWd, user, 0);
            break;
        case 'r':
            printf("***检测到注册事件***\n");
            token = strtok(temp, s);
            token = strtok(NULL, s);
            username = (char *)malloc(sizeof(token));
            memcpy(username, token, sizeof(token));

            token = strtok(NULL, s); // passwd信息
            passWd = (char *)malloc(sizeof(token));
            memcpy(passWd, token, sizeof(token));

            userInfo *new = createUserInfo(username, passWd, 10);
            appendUserInfo(user, new);
            printf("注册成功，当前用户列表为：\n");
            iterUserInfoList(user);
            break;
        }
        ftruncate(fd, 0);
        close(fd);
        sleep(1);
    }
}