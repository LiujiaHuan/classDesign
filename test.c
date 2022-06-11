#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "cJSON.c"

#define MAX_LINE 4096
//userInfo结构体 保存用户信息
typedef struct userInfo{
    char *userName;
    char *userPasswd;
    int priority;
}userInfo;



//userInfoList 用户信息的链表 data指向用户信息结构体
typedef struct userInfoList{
    struct userInfoList* next;
    struct userInfoList* pre;
    userInfo* current;
}userInfoList;



/*创建一个UserInfoList的Node*/
userInfoList* createUINode(){
    userInfoList* temp = (userInfoList*)malloc(sizeof(userInfoList));
    temp->current = NULL;
    temp->next = NULL;
    temp->pre = NULL;
    return temp;
}



/*创建一个UserInfo结构体*/
userInfo* createUserInfo(char *name,char *passwd,int priority){
    userInfo* Temp = (userInfo*)malloc(sizeof(userInfo));
    Temp->userName = name;
    Temp->userPasswd = passwd;
    Temp->priority = priority;
    return Temp;
}



/*为userInfoList每一个User维护一个json文件存放事务*/
void createFile(char* name){
    char path[20]="./jsonFile/";
    char temp[]=".json";
    FILE *fp;
    strcat(path,name);
    strcat(path,temp);
    if((fp=fopen(path,"w"))==NULL) /*建立新文件 */
    {
        printf("cannot open file\n"); /*建立新文件出错误信息*/
        exit(1); /*终止调用过程、关闭所有文件*/
    }
    fclose(fp);
}



/*获取文件全部内容 返回一个指针*/
char *getFileAll(char *fname){
	FILE *fp;
	char *str;
	char txt[1000];
	int filesize;
	if ((fp=fopen(fname,"r"))==NULL){
		printf("打开文件%s错误\n",fname);
		return NULL;
	}
 
	fseek(fp,0,SEEK_END); 
 
	filesize = ftell(fp);
	str=(char *)malloc(filesize);
	str[0]=0;
 
	rewind(fp);
	while((fgets(txt,1000,fp))!=NULL){
		strcat(str,txt);
	}
	fclose(fp);
	return str;
}



/*初始化一个userInfoList*/
userInfoList* initUserInfoList(){
    userInfoList* beginNode = createUINode();
    return beginNode;
};



/*给userInfoList尾插新节点*/
userInfoList* appendUserInfo(userInfoList* L,userInfo* newInfo){
    // printf("222\n");
    userInfoList* root = L;//保存一下根节点 一会return回去
    while(L->next != NULL){
        printf("111\n");
        L = L->next;    //遍历到存有数据的最后一个节点
    }
    userInfoList* newNode = createUINode();
    L->next = newNode;
    newNode->current = newInfo;
    newNode->next = NULL;
    newNode->pre = L;
    return root;
}



/*遍历userInfoList表*/
void iterUserInfoList(userInfoList* L){
    L = L->next;
    while(L!=NULL){
        printf("userId： %s passwd： %s  priority： %d\n",L->current->userName,L->current->userPasswd,L->current->priority);
        L = L->next;
    }
}



/* 初始化读取并维护一个userInfoList链表*/
void initUserInfo(){
    userInfoList* L = initUserInfoList();
    // if(L->next==NULL){
    //     printf("?????");
    // }
    char*message = getFileAll("./user.json");

    cJSON* cjson_test = NULL;
    cJSON* cjson_userId = NULL;
    cJSON* cjson_passwd = NULL;
    cJSON* cjson_priority = NULL;
    cJSON* cjson_address_country = NULL;
    cJSON* cjson_address_zipcode = NULL;
    cJSON* cjson_skill = NULL;
    int    skill_array_size = 0, i = 0;
    cJSON* cjson_skill_item = NULL;

    /* 解析整段JSON数据 */
    cjson_test = cJSON_Parse(message);
    if(cjson_test == NULL)
    {
        printf("parse fail.\n");
        return -1;
    }

    if(!cjson_test) {
        printf("no json\n");
        return -1;
    }
    if (!cJSON_IsArray(cjson_test)){
        printf("no Array\n");
        return -1;
    }
    /* 解析数组 */

    skill_array_size = cJSON_GetArraySize(cjson_test);
    printf("%d",skill_array_size);
    for(i = 0; i < skill_array_size; i++)
    {
        cjson_skill_item = cJSON_GetArrayItem(cjson_test, i);
            /* 依次根据名称提取JSON数据（键值对） */
        cjson_userId = cJSON_GetObjectItem(cjson_skill_item, "userId");
        cjson_passwd = cJSON_GetObjectItem(cjson_skill_item, "passwd");
        cjson_priority = cJSON_GetObjectItem(cjson_skill_item, "priority");
        //根据json文件创建新的userInfo
        userInfo* newUserInfo = createUserInfo(cjson_userId->valuestring, cjson_passwd->valuestring, cjson_priority->valueint);
        L = appendUserInfo(L, newUserInfo);     //根据json解析数据添加节点 
        createFile(cjson_userId->valuestring);
    }
    iterUserInfoList(L);
};



void userLogin(){

}


void userRegister(){
    char name[20]={0},passwd[20]={0};
    printf("ok，请输入你的用户名称\n");
    printf("ok，请输入你的密码\n");
    printf("ok，请确认你的密码\n");
}


int main(){
    initUserInfo();
    int funcFlag;
    printf("你要干什么？1是注册奥\n");
    scanf("%d",&funcFlag);
    if(funcFlag>5 || funcFlag<0){
        printf("error");
        exit(1);
    }
    switch(funcFlag){
        case 1:
            userRegister();
    }
}