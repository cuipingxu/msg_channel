#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>

#include <string>

//g++ -g -o client.bin ./client.cpp
int main(int argc, char * argv[])
{
	int ret = 0;
	
	if(argc < 2)
	{
		printf("pls input count per second!\n");
		return 0;
	}
	
	unsigned count = (unsigned int)atoi(argv[1]);
	printf("prepare to send %u request during per second ....\n", count);
	
	struct sockaddr_in address; 
	bzero(&address,sizeof(address));  
	address.sin_family = AF_INET;  
	//address.sin_addr.s_addr = inet_addr("47.89.28.172");
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(8080);  
	
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	ret = connect(fd, (struct sockaddr *)&address, sizeof(address));
	if(ret != 0)
	{
		printf("connect failed, fd:%d, ret:%d, errno:%d, errmsg:%s\n", fd, ret, errno, strerror(errno));
		return 0;
	}
	else
	{
		printf("connect success!\n");
	}
	
	unsigned long long begin_time = time(NULL);
	unsigned long long cur_time = 0;
	unsigned int loop = 0;
	
	const char *req = "{\"head\":{\"cmd\":\"http_get\",\"time\":12344556,\"req_id\":\"1482375312499790\"},\"body\":\"anchorId=1000146&appId=1&brand=apple-iPhone7&device_id=DDADCBE9-0348-4EAA-B747-B254CAFFA3C2&method=getRank&net=3&os=iOS-10.0.2&page=1&platform=livestar&sign=84908a961c938f4b52ad25fd9aa157d7&type=total&version=2.0.0&isoss=1&isphper=1\"}\n";
	while(1)
	{		
		ret = ::send(fd, req, strlen(req), 0);
		if(ret >= 0)
		{
			//printf("send success!\n");
		}
		else
		{
			printf("send failed, ret:%d!\n", ret);	
		}
		
		++loop;
	
		cur_time = time(NULL);
		if((cur_time >= (begin_time+1)) ||(loop >= count))
		{
			break;
		}
	}
	
	printf("complete to send request, begin:%llu, end:%llu, count:%u\n", begin_time, cur_time, loop);
		
	while(1)
	{	
		::sleep(1);
	}		
		
    return 0;
}
