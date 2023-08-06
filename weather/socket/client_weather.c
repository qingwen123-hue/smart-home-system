#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "cJSON.h"
/* 秘钥，注意！！如果要用这一份代码，这个一定要改为自己的，因为这个我已经故意改错了，防止有人与我公用一个KEY */
#define  KEY    "S4Sv-YSaBFsiJJyFz"		// 这是在心知天气注册后，每个用户自己的一个key
#define LOCATION "guangzhou"
/* GET请求包 */
#define  GET_REQUEST_PACKAGE   "GET https://api.seniverse.com/v3/weather/now.json?key=%s&location=%s&language=zh-Hans&unit=c\r\n\r\n"
	



#define SERVER_PORT 80 //服务器的端口号
#define SERVER_IP "116.62.81.138" //服务器的 IP 地址

int main(void)
{
    struct sockaddr_in server_addr = {0};
    char buf[1024] = {0},GetRequestBuf[1024] = {0};
    int sockfd;
    int ret;
    /* 打开套接字，得到套接字描述符 */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > sockfd) 
    {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    /* 调用 connect 连接远端服务器 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT); //端口号
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);//IP 地址
    ret = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (0 > ret) 
    {
        perror("connect error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("服务器连接成功...\n\n");
    /* 向服务器发送数据 */

    /* 组合GET请求包 */
    sprintf(GetRequestBuf, GET_REQUEST_PACKAGE, KEY, LOCATION);

    /* 发送数据到服务端 */
    printf("%s\n",GetRequestBuf);

    send(sockfd, GetRequestBuf, strlen(GetRequestBuf), 0);

    recv(sockfd,buf,sizeof(buf), 0);

    //printf("%s  \n",buf);

    cJSON* cjson_test = NULL;
    cJSON* cjson_results = NULL;
    cJSON* cjson_now = NULL;
    cJSON* cjson_now_text = NULL;
    cJSON* cjson_now_code = NULL;
    cJSON* cjson_now_temperature = NULL;

    /* 解析整段JSO数据 */
    cjson_test = cJSON_Parse(buf);
    if(cjson_test == NULL)
    {
        printf("parse fail.\n");
        return -1;
    }

    //printf("test:%s\n",cJSON_Print(cjson_test));

    cjson_results = cJSON_GetObjectItem(cjson_test, "results");

    char * tem1 = cJSON_Print(cjson_results);
    //printf("results:%s\n",tem1);

    char * tem2 = (char *)malloc((strlen(tem1) - 2) * sizeof(char));
    memcpy(tem2,tem1+1,(strlen(tem1) - 2));
    cjson_results = cJSON_Parse(tem2);
    cjson_now = cJSON_GetObjectItem(cjson_results, "now");

    
    //printf("now:%s\n",cJSON_Print(cjson_now));
    cjson_now_text = cJSON_GetObjectItem(cjson_now,"text");
    cjson_now_code = cJSON_GetObjectItem(cjson_now,"code");
    cjson_now_temperature = cJSON_GetObjectItem(cjson_now,"temperature");

    printf("text:%s\n",cJSON_Print(cjson_now_text));
    //printf("code:%s\n",cjson_now_code->valuestring);
    //printf("code:%d\n",cjson_now_code->valueint);
    //printf("temperature:%s\n",cjson_now_temperature->valuestring);
    free(tem1);
    free(tem2);
    cJSON_Delete(cjson_test);

    close(sockfd);
    exit(EXIT_SUCCESS);
}