#include <stdio.h>
#include "cJSON.h"
#include "cJSON.c"
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

int main(void)
{   
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

    return 0;
}