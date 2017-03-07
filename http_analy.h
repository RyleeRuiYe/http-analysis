#ifndef _HTTP_ANALY_H
#define _HTTP_ANALY_H

#include <stdbool.h>
#include "proxy_frame.h"

#define HEADMAP 0
#define METHODMAP 1
#define RESPONDMAP 2
//extern int my_map.so  

int getRequestLineLength(char *buf_cli,int packetLength);
void requestLineAnaly(char *buf_cli,firstLine_info *firstLine,int packetLength);
void statusLineAnaly(char *buf_cli,firstLine_info *firstLine,int packetLength);
bool isGet(char *buf_cli,int packetLength);
void getFileName(firstLine_info *firstLine,int len);
void getUrl(http_header_info *headers,firstLine_info *firstLine);
bool isNotNewline(char *p,char *start,int packetLength);
bool isNotSpace(char *p);
	
//包类型
#define REQUEST 1 //请求包
#define RESPOND 2 //响应包
#define DATA  3   //数据包
#define WRONG -1  //出错


void getHeadLine(char *start,char **name,char **value,int packetLength);
int myStrlen(const char *str);


int getContentLength(char *buf_cli,int packetType,int packetLength);
void GetBody(char *buf_cli,char *body,int length,int PacketType,int packetLength);
int getPacketType(char *buf_cli,int packetLength);
int http_analy(int sockfd,http_header_info *headers,firstLine_info *firstLine);

#endif
