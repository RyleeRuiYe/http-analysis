#include "http_analy.h"
#include <stdio.h>
extern int searchMap(int, char *);

//不是回车换行返回true
bool isNotNewline(char *p,char *start,int packetLength)
{
	//if(p+1>=start+packetLength)
		//return false;//越界返回false,是的调用处的循环终止
	if((*p)=='\r'&&(*(p+1))=='\n'){//回车换行//p+1越界  回车:回到行首
		return false;
	}
	//if((*p)=='\n')//有的只会发送换行符
	//	return false;
	return true;
}


//不是空格返回true
bool isNotSpace(char* p)
{
	return !(*p==' ');//空格
}


/*获取请求行的长度,含回车换行*/
int getRequestLineLength(char *buf_cli,int packetLength)
{
	char *p;
	int length=0;
	p=buf_cli;//记录起始位置
	while(((p+2)<(buf_cli+packetLength))&&isNotNewline(p,buf_cli,packetLength))
	{
		p+=1;
		length++;
	}
	length=length+2;
	return length;
}


/*获取请求报文的方法，url和版本*/
void requestLineAnaly(char *buf_cli,firstLine_info *firstLine,int packetLength)
{
	char *pa,*pb;//pa记录起始位置,pb移动
	pa=pb=buf_cli;
    int len=0;//记录字节数
    printf("asdf %d \n",__LINE__);
	while(isNotSpace(pb)&&pb<(buf_cli+packetLength))//找到请求报文第一个空格
	{
		pb+=1;
		len++;
	}
	 printf("asdf %d \n",__LINE__);
	 firstLine->method = (char *)malloc(len+1);
	 memset(firstLine->method,0,len+1);
	memcpy(firstLine->method,pa,len);//请求方法
	printf("method is:%s\n",firstLine->method);
	len=0;
	pa=pb+1;
	pb=pa;
	 printf("asdf %d \n",__LINE__);
	while(isNotSpace(pb)&&pb<(buf_cli+packetLength))//找到请求报文第二个空格
	{
		pb+=1;
		len++;
	}
	 printf("asdf %d \n",__LINE__);
	 firstLine->url = (char *)malloc(len+1);
	 memset(firstLine->url,0,len+1);
	memcpy(firstLine->url,pa,len);//待处理的url
	printf("!url is:%s\n",firstLine->url);
	getFileName(firstLine,len);

	len=0;
	pa=pb+1;
	pb=pa;
	 printf("asdf %d \n",__LINE__);
	while(isNotNewline(pb,buf_cli,packetLength)&&pb<(buf_cli+packetLength))
	{
		pb+=1;
		len++;
	}
	 printf("asdf %d \n",__LINE__);
	 firstLine->version=(char *)malloc(len+1);
	memcpy(firstLine->version,pa,len);//版本
	printf("version is:%s\n",firstLine->version);
	printf("asdf %d \n",__LINE__);
}


/*获取响应报文的版本，响应码和短语*/
void statusLineAnaly(char *buf_cli,firstLine_info *firstLine,int packetLength)
{
	char *pa,*pb;//pa记录起始位置,pb移动
	pa=pb=buf_cli;
        int len=0;//记录字节数
	while(isNotSpace(pb)&&pb<(buf_cli+packetLength))//找到请求报文第一个空格
	{
		pb+=1;
		len++;
	}
	memcpy(firstLine->version,pa,len);//版本
	printf("version is:%s\n",firstLine->version);
	len=0;
	pa=pb+1;
	pb=pa;
	while(isNotSpace(pb)&&pb<(buf_cli+packetLength))//找到报文第二个空格
	{
		pb+=1;
		len++;
	}
	memcpy(firstLine->respondCode,pa,len);//响应码
	printf("respondCode is:%s\n",firstLine->respondCode);
	len=0;
	pa=pb+1;
	pb=pa;
	while(isNotNewline(pb,buf_cli,packetLength)&&pb<(buf_cli+packetLength))
	{
		pb+=1;
		len++;
	}
	memcpy(firstLine->phrase,pa,len);//短语
	printf("phrase is:%s\n",firstLine->phrase);
}


/*判断请求方法是否为GET*/  
//GET方法的请求报文无实体内容
bool isGet(char *buf_cli,int packetLength)
{
	char *pa,*pb;//pa记录起始位置,pb移动
	pa=pb=buf_cli;
    	int len=0;//记录字节数
	while(isNotSpace(pb)&&pb<(buf_cli+packetLength))//找到请求报文第一个空格
	{
		pb+=1;
		len++;
	}
	return !(strncmp("GET",pa,len));
}

/*获取文件名*/
void getFileName(firstLine_info *firstLine,int len)
{
	char *p=firstLine->url+len-1;
	int fileNameLen=0;
	char *trans;//中转
	printf("### url = %s, len = %d\n", firstLine->url, len);
	while(p >= firstLine->url && (*p)!='/')
	{
		p--;
		fileNameLen++;
	}
	p++;
//	fileNameLen--;
	printf("getfname %d\n",__LINE__);
	firstLine->file_name = (char *)malloc(fileNameLen+1);
	 memset(firstLine->file_name,0,fileNameLen+1);
	 printf("getfname %d\n",__LINE__);
	memcpy(firstLine->file_name,p-1,fileNameLen);
	trans = (char *)malloc(len+1);
	 memset(trans,0,len+1);
	 printf("getfname %d\n",__LINE__);
	memcpy(trans,firstLine->url,len);
	//firstLine->url = (char *)malloc(len+1);
	 //memset(firstLine->url,0,len+1);
	 printf("getfname %d\n",__LINE__);
	memcpy(firstLine->url,trans,len-fileNameLen);
	printf("### fin: url = %s, len = %d\n", firstLine->url, len-fileNameLen);
}

/*获取真实的网址*/
void getUrl(http_header_info *headers,firstLine_info *firstLine)
{
	int i=0;
	char *tp1;
	char *tp2;
	int length;
//	/*
	while(i < 25 && headers[i].name!=HOST)
		i++;
//	*/
	printf("i=%d",i);
	printf("### 0...\n");
//	while(i < 25 && strcmp(headers[i].name, "HOST") != 0){
//		i++;
//	}
	printf("### 1...\n");
	tp1 = firstLine->url;
	length = strlen(firstLine->url) + headers[i].length + 1;
	printf("%d",headers[i].length);
	firstLine->url = (char *)malloc(length);
	printf("### length = %d\n", length);
	if(firstLine->url == NULL) {
		printf("### error...\n");
		return;
	}
	printf("### 2...\n");
	memset(firstLine->url, 0, length);
	tp2 = firstLine->url;
	tp2 += sprintf(tp2, tp1,NULL);
//	tp2 += strlen(tp1);
	tp2 += sprintf(tp2, headers[i].value,NULL);
	printf("### 3...\n");
	printf("get url %s\n",firstLine->url);
//	strcat(firstLine->url,headers[i].value);
	return;
}


/*取一行的首部字段名和值*/
void getHeadLine(char *start,char **name,char **value,int packetLength)
{	
	int len=0;
	printf("packetLength:%d\n",packetLength);
	printf("get head line %d\n",__LINE__);
	//print_data(start, packetLength);
	while((len < packetLength) && isNotSpace(start+len))
	{
		len++;
		printf("e  %c~",start[len]);
	}
	printf("get head line = %d\n",__LINE__);
//	printf("*name = %d, start = %d, len = %d\n",sizeof(*name), sizeof(start),len);
	*name = (char *)malloc(len+1);
	if(!*name) return;
	memset(*name, 0, len+1);
	memcpy(*name,start,len);//取首部字段名

	printf("get head line %d\n",__LINE__);
	len=0;
	start=start+len+1;
	printf("get head line %d\n",__LINE__);
	while(isNotNewline(start+len,start,packetLength)&&((start+len)<start+packetLength))
	{
		len++;
	}
	printf("get head line %d\n",__LINE__);
	*value = (char*)malloc(len+1);
	memcpy(*value,start,len);//取首部字段名对应的值
	printf("value = %s, start = %s\n",*value, start);
	//printf("start  %d",strlstart);
	printf("get head line %d\n",__LINE__);
}


/*获取字符串的长度*/
int myStrlen(const char *str)
{
	int len=0;//str判断空
	while(*(str+len)!='\0')
		len++;
	return len;
}



/*获取主体内容的长度
packetType 包类型
recLength 收到数据包长度*/
int getContentLength(char *buf_cli,int packetType,int packetLength)
{
	char *pa, *pb;
    pa=buf_cli;
    int contentLength;
	int i=0;
    if(packetType==RESPOND)
    {
        while((pa<(buf_cli+packetLength))&&isNotNewline(pa,buf_cli,packetLength)&&isNotNewline(pa+2,buf_cli,packetLength))//判断越界和头部结束
			pa+=1;
	    if((pa+4)<(buf_cli+packetLength))
	    {
	        pa+=4;
			pb=pa;
	        if(*pa!=0)
	        {
			    while(*pa!=0)
			    pa+=1;
			    contentLength=myStrlen(pb)-myStrlen(pa);
			    return contentLength;
	        }
	    }
	    else contentLength=0;  
    }
	else if(packetType==DATA)
        contentLength=packetLength;

		else contentLength=0;

    return contentLength;
}


/*获取头部的长度*/
// int getHeadLength(char *buf_cli,int packetType,int packetLength)
// {
// 	int headLength=0;
// 	if(packetType==DATA)
// 		return 0;
// 	int dataLength=getContentLength(buf_cli,packetType,packetLength);
// 	headLength=packetLength-dataLength;
// 	return headLength;
// }


/*
函数功能：取出http报文的主体部分，放入缓冲区body
输入参数：http报文缓冲区，body缓冲区，主体长度。主体可能是文本也可能是二进制
未使用*/
void GetBody(char *buf_cli,char *body,int length,int PacketType,int packetLength)
{
    char *pa;
    pa=buf_cli;
	if(PacketType==RESPOND){
		while((pa<(buf_cli+RECV_BUF_SIZE))&&isNotNewline(pa,buf_cli,packetLength)&&isNotNewline(pa+2,buf_cli,packetLength))
		//判断越界和头部结束
		pa+=1;
		if(pa<(buf_cli+RECV_BUF_SIZE))
		{
			pa+=4;
			if(*(pa)!=0)
			{
				memcpy(body,pa,length);
				printf("%s\n",body);
			}
		}
	}
	if(PacketType==DATA){
		memcpy(body,pa,length);
		printf("%s\n",body);
	}
	
}


/*
函数功能：根据rec得到TCP传输内容判断数据包类型
函数输入：HTTP报文，起始行变量
函数输出：数据包类型
*/
int getPacketType(char *buf_cli,int packetLength)
{
    char *pa,*pb;//pa记录起始位置,pb移动
    pa=pb=buf_cli;
    int len=0;//记录字节数
    char *method=NULL;
    char *respondCode=NULL;
	printf("analy line %d \n",__LINE__);
	while(isNotSpace(pb)&&pb<(buf_cli+packetLength))//找到请求报文第一个空格
	{
		pb+=1;
		len++;
	}
	printf("analy line %d \n",__LINE__);
	method=(char*)malloc(len+1);
	memcpy(method,pa,len);
	printf("method = %s\n",method);
	if(searchMap(METHODMAP, method) >= 0)//出现方法的为请求包
		{
		printf("analy line %d \n",__LINE__);
		return REQUEST;
}
    
	len=0;
	pa=pb+1;
	pb=pa;
	while(isNotSpace(pb)&&pb<(buf_cli + packetLength))//找到请求报文第二个空格
	{
		pb+=1;
		len++;
	}
	respondCode=(char*)malloc(len+1);
	memcpy(respondCode,pa,len);
	printf("respondcode = %s",respondCode);
	if(searchMap(RESPONDMAP,respondCode) >= 0)//出现的为请求包
		return RESPOND;

    return DATA;
}


/*http报文分析主函数
sockfd: socket描述符
headers: 首部信息数组  
firstLine: 起始行(报文的第一行)信息结构体指针
返回：包类型*/
int http_analy(int sockfd,http_header_info *headers,firstLine_info *firstLine)
{
	int packetType;
	int contentLength;
	int requestLineLength;
	char *name;//首部字段名
	char *value;//值
	int headerIndex;//头部信息在headers数组的下标
	int len;//值的长度
	int packetLength;
	//memset(&firstLine,"",sizeof(firstLine));	

	printf("analy line %d \n",__LINE__);
	data_buf *dptr = NULL;
	dptr = find_sock_data(sockfd);
	if (dptr == NULL)
	{
		printf("wrong sockfd\n");
		return -1;
	}
	printf("analy line %d \n",__LINE__);
	packetLength=dptr->len;
	printf("analy line %d \n",__LINE__);
	requestLineLength=getRequestLineLength(dptr->buf,packetLength);//获取请求行的长度	
	printf("analy line %d \n",__LINE__);
	printf("requestLineLength %d\n",requestLineLength);
	int s;
	for(s=0;s<35;s++)
	{
	getHeadLine(dptr->buf+requestLineLength,&name,&value,packetLength);//取一行的首部字段名和值
	printf("### getHeadLine value = %s\n", value);
	printf("analy line %d \n",__LINE__);
	len=myStrlen(value);
	printf("analy line %d \n",__LINE__);

	headerIndex=searchMap(HEADMAP, name);//在map中查找对应的首部字段名
	if(headerIndex>=0)
	{
		printf("analy line %d \n",__LINE__);
		headers[headerIndex].value=(char*)malloc((len+1)*sizeof(char));
		if(headers[headerIndex].value == NULL)
		{
			printf("Malloc Error\n");
			return WRONG;
		}
		
		//headers+len=NULL;
		printf("****value = %s, len = %d, headerIndex = %d\n",value, len, headerIndex);
		memcpy(headers[headerIndex].value, value, len);//向首部信息数组header中赋首部字段值
		printf("****headers value = %s\n",headers[headerIndex].value);
		headers[headerIndex].length=len;//赋首部字段值的长度
	}	
    }
	printf("analy line = %d\n",__LINE__);
	packetType=getPacketType(dptr->buf,packetLength);//获取包类型	
	//printf("the requestLineLength is:%d\n",requestLineLength);//输出请求行的长度

    headers[24].intValue=getContentLength(dptr->buf,packetType,packetLength);
	printf("The content length is %d\n",(*(headers+24)).intValue);//主体长度

	headers[23].intValue=packetLength - headers[24].intValue;
	printf("The head length is %d\n",headers[23].intValue);//头部长度

	switch(packetType)
	{
		case REQUEST://请求包操作
			printf("The PacketType is:REQUEST\n");
			if(isGet(dptr->buf,packetLength))					
				printf("the method is GET\n");
				printf("asdf %d \n",__LINE__);
			requestLineAnaly(dptr->buf,firstLine,packetLength);//请求报文请求行分析			
			printf("analy line %d \n",__LINE__);
			getFileName(firstLine, strlen(firstLine->url));
			getUrl(headers,firstLine);
			printf("analy line %d \n",__LINE__);
			return REQUEST;							

		case RESPOND:
			printf("The PacketType is:RESPOND\n");
			statusLineAnaly(dptr->buf,firstLine,packetLength);//响应报文状态行分析
			if(firstLine->respondCode[0]!='2'&&firstLine->respondCode[0]!='3')//响应码不在200~399之间出错
				return WRONG; 
			return RESPOND;

		default:
			printf("The PacketType is:DATA\n");
			return DATA;
	}		

}


// int main(){
//         char *t1 = "Authorization:";
//         char *t2 = "Cookie:";
//         int res1 = searchMap(1, t1);
//         int res2 = searchMap(1, t2);
//         printf("res1 = %d, res2 = %d\n", res1, res2);
//         return 0;
// }
