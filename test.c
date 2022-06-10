#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "cJSON.c"

#define MAX_LINE 4096
typedef struct userInfo{
    char *userName;
    char *userPasswd;
    int priority;
}userInfo;

struct userInfoList{
    userInfo* next;
    userInfo* pre;
    userInfo* current;
};

char *getFileAll(char *fname)
{
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
void initUserInfo(){
    char*message = getFileAll("./test.json");

    cJSON* cjson_test = NULL;
    cJSON* cjson_name = NULL;
    cJSON* cjson_age = NULL;
    cJSON* cjson_weight = NULL;
    cJSON* cjson_address = NULL;
    cJSON* cjson_address_country = NULL;
    cJSON* cjson_address_zipcode = NULL;
    cJSON* cjson_skill = NULL;
    cJSON* cjson_student = NULL;
    int    skill_array_size = 0, i = 0;
    cJSON* cjson_skill_item = NULL;

    /* 解析整段JSO数据 */
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
        cjson_name = cJSON_GetObjectItem(cjson_skill_item, "userId");
        cjson_age = cJSON_GetObjectItem(cjson_skill_item, "passwd");
        cjson_weight = cJSON_GetObjectItem(cjson_skill_item, "priority");

        printf("userId: %s\n", cjson_name->valuestring);
        printf("passwd:%s\n", cjson_age->valuestring);
        printf("priority:%d\n", cjson_weight->valueint);
    }
};



void userLogin(){

};

userInfo* creatUserInfo(char *name,char *passwd,int priority){
    userInfo* Temp = (userInfo*)malloc(sizeof(userInfo));
    Temp->userName = name;
    Temp->userPasswd = passwd;
    Temp->priority = priority;
    return Temp;
};

void userRegister(){
    char name[20]={0},passwd[20]={0};
    printf("ok，请输入你的用户名称\n");
    printf("ok，请输入你的密码\n");
    printf("ok，请确认你的密码\n");
    
    
    
};


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