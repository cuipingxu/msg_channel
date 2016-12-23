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

//g++ -o client.bin ./client.cpp -lpthread -ldl -lrt -lz
int main(int argc, char * argv[])
{
	int ret = 0;
	
	struct sockaddr_in address; 
	bzero(&address,sizeof(address));  
	address.sin_family = AF_INET;  
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(4444);  
	
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
	
	unsigned long long sec = 0;
	unsigned long long nsec = 0;
	struct timespec begin;
	clock_gettime((clockid_t)CLOCK_REALTIME, &begin);
	sec  = begin.tv_sec;
	nsec = begin.tv_nsec;
	
	char req[1024] = {0};
	snprintf(req, 1024, "{\"head\":{\"cmd\":\"http_get\",\"time\":1477534852,\"req_id\":\"getRecently1477534852\"},\"body\":\"limit=20&page=1&method=getRecently&route=livestar&platform=livestar&isoss&isphper\"}\n");
	ret = ::send(fd, req, strlen(req), 0);
	if(ret >= 0)
	{
		printf("send success!\n");
	}
	else
	{
		printf("send failed, ret:%d!\n", ret);	
	}
	
	char rsp[1024] = {0};
	memset(rsp, 0x0, 1024);
	ret = ::recv(fd, rsp, 1023, 0);
	if(ret > 0 )
	{
		printf("rsp:%s\n", rsp);
	}
	else if(ret == 0)
	{
		printf("socket is colse!\n");
	}
	else
	{
		printf("recv failed, ret:%d\n", ret);
	}

	unsigned long long sec1 = 0;
	unsigned long long nsec1 = 0;	
	struct timespec end;
	clock_gettime((clockid_t)CLOCK_REALTIME, &end);
	sec1  = end.tv_sec;
	nsec1 = end.tv_nsec;
	
	printf("=== complete to send, sec:%llu, nsec:%llu\n", (sec1-sec), (nsec1-nsec));
		
    return 0;
}
