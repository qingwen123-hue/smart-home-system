#include "weather.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "cJSON.h"
#include "QString"
#include <QDebug>
/* 秘钥，注意！！如果要用这一份代码，这个一定要改为自己的，因为这个我已经故意改错了，防止有人与我公用一个KEY */
#define  KEY    "S4Sv-YSaBFsiJJyFz"		// 这是在心知天气注册后，每个用户自己的一个key
#define LOCATION "guangzhou"
/* GET请求包 */
#define  GET_REQUEST_PACKAGE   "GET https://api.seniverse.com/v3/weather/now.json?key=%s&location=%s&language=zh-Hans&unit=c\r\n\r\n"


#define SERVER_PORT 80 //服务器的端口号
#define SERVER_IP "116.62.81.138" //服务器的 IP 地址



void GetWeather(int &IndoorTem,int &WeatherCode)
{
    struct sockaddr_in server_addr;
    int sockfd;

    server_addr = {0};
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
    char buf[1024] = {0},GetRequestBuf[1024] = {0};
    /* 组合GET请求包 */
    sprintf(GetRequestBuf, GET_REQUEST_PACKAGE, KEY, LOCATION);

    /* 发送数据到服务端 */
    //printf("%s\n",GetRequestBuf);

    send(sockfd, GetRequestBuf, strlen(GetRequestBuf), 0);

    recv(sockfd,buf,sizeof(buf), 0);

    //printf("%s  \n",buf);
    //qDebug() << buf;
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
        return ;
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

    //printf("text:%s\n",cJSON_Print(cjson_now_text));
    //printf("code:%s\n",cjson_now_code->valuestring);
    //printf("temperature:%s\n",cjson_now_temperature->valuestring);
    QString Qtem1(cjson_now_code->valuestring),Qtem2(cjson_now_temperature->valuestring);
    //qDebug() << Qtem1 << " " << Qtem2;
    IndoorTem = Qtem2.toDouble();
    WeatherCode = Qtem1.toDouble();



    free(tem1);
    free(tem2);
    cJSON_Delete(cjson_test);

    close(sockfd);

}

QString CodeToString(int WeatherCode)
{
    switch (WeatherCode)
    {
        case 0 :
            return "白天晴";
        case 1 :
            return "夜晚晴";
        case 4 :
            return "多云";
        case 5 :
            return "晴间多云";
        case 6 :
            return "晚间多云";
        case 7 :
            return "大部多云";
        case 8 :
            return "大部多云";
        case 9 :
            return "阴";
        case 10 :
            return "阵雨";
        case 11 :
            return "雷阵雨";
        case 12 :
            return "雷阵雨伴有冰雹";
        case 13 :
            return "小雨";
        case 14 :
            return "中雨";
        case 15 :
            return "大雨";
        case 16 :
            return "暴雨";
        case 17 :
            return "大暴雨";
        case 18 :
            return "特大暴雨";
        case 19 :
            return "冻雨";
        case 20 :
            return "雨夹雪";
        case 21 :
            return "阵雪";
        case 22 :
            return "小雪";
        case 23 :
            return "中雪";
        case 24 :
            return "大雪";
        case 25 :
            return "暴雪";
        case 26 :
            return "浮尘";
        case 27 :
            return "扬沙";
        case 28 :
            return "沙尘暴";
        case 29 :
            return "强沙尘暴";
        case 30 :
            return "雾";
        case 31 :
            return "霾";
        case 32 :
            return "风";
        case 33 :
            return "大风";
        case 34 :
            return "飓风";
        case 35 :
            return "热带风暴";
        case 36 :
            return "龙卷风";
        case 37 :
            return "冷";
        case 38 :
            return "热";
        case 99 :
            return "未知";
        default :
            return "无效代码";
    }
}


