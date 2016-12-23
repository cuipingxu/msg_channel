
#include "common.h"
#include "base/base_socket_oper.h"
#include "base/base_time.h"
#include "base/base_net.h"
#include "base/base_string.h"
#include "base/base_uid.h"
#include "base/base_logger.h"
#include "req_tcp_event_handler.h"
#include "req_mgt.h"
#include "session_mgt.h"
#include "conf_mgt.h"
#include "msg_oper.h"

extern Logger g_logger;
extern StSysInfo g_stSysInfo;

Req_TCP_Event_Handler::Req_TCP_Event_Handler(): _buf(""), _cnt(0), _pre_time(0)
{

};

Req_TCP_Event_Handler::~Req_TCP_Event_Handler()
{

};

	
//处理建立连接请求事件
int Req_TCP_Event_Handler::handle_accept(int fd)
{
	int nRet = 0;
	
	std::string local_ip = "";
	unsigned short local_port = 0;
	get_local_socket(fd, local_ip, local_port);
	
	std::string remote_ip = "";
	unsigned short remote_port = 0; 
	get_remote_socket(fd, remote_ip, remote_port);
	
	XCP_LOGGER_INFO(&g_logger, "accept from app(fd:%d), %s:%u --> %s:%u\n", 
		fd, remote_ip.c_str(), remote_port, local_ip.c_str(), local_port);
	
	PSGT_Session_Mgt->insert_session(fd);
	
	return nRet;
	
};



/*
处理读事件
telnet 消息结尾\r\n (0xd0xa)
客户端消息结尾\n
*/
int Req_TCP_Event_Handler::handle_input(int fd)
{
	int nRet = 0;

	//获取前端ip 和port
	std::string ip = "";
	unsigned short port = 0;
	get_remote_socket(fd, ip, port);
	
	//读取fd
	char buf[4096] = {0};
	unsigned int buf_len = 4095;
	nRet = Socket_Oper::recv(fd, buf, buf_len, 300000);
	if(nRet == 0)
	{
		XCP_LOGGER_INFO(&g_logger, "rcv close form app(fd:%d, peer:%s:%u), ret:%d\n", fd, ip.c_str(), port, nRet);
		return -1;
	}
	else if(nRet == 1)
	{
		//XCP_LOGGER_INFO(&g_logger, "rcv success from app(fd:%d), req(%u):%s\n", fd, buf_len, buf); 
	}
	else
	{
		XCP_LOGGER_INFO(&g_logger, "rcv failed from app(fd:%d, peer:%s:%u), ret:%d\n", fd, ip.c_str(), port, nRet);
		return 0;
	}
	buf[buf_len] = '\0';

	std::string session_id = "";
	PSGT_Session_Mgt->update_read(fd, buf_len, session_id);
	
	//追加缓存
	_buf += buf;
	std::string::size_type pos = _buf.find("\n");
	while(pos != std::string::npos)
	{
		//解析完整请求串
		std::string req_src = _buf.substr(0, pos);
		_buf.erase(0, pos+1);

		trim(req_src);
					
		if(req_src.size() < MIN_MSG_LEN)
		{
			printf("the req reach min len, req(%u):%s\n", req_src.size(), req_src.c_str());
			//XCP_LOGGER_ERROR(&g_logger, "the req reach min len, req(%u):%s\n", req_src.size(), req_src.c_str());
		}		
		else if(req_src.size() > MAX_MSG_LEN)
		{
			printf("the req reach max len(%u)\n", MAX_MSG_LEN);
			//XCP_LOGGER_ERROR(&g_logger, "the req reach max len(%u)\n", MAX_MSG_LEN);
		}
		else
		{

			/*
			//单链接每秒最多发送3条消息
			unsigned long long cur_time = time(NULL);
			++_cnt;
			if((_cnt > MSG_CNT_PER_SEC) && ((cur_time - _pre_time) < 1))
			{
				XCP_LOGGER_ERROR(&g_logger, "reach max count msg during per sec.\n");
				Msg_Oper::send_msg(fd, "null", "null", "null", ERR_REACH_MAX_MSG_CNT, "reach max count msg during per sec.");
				continue;
			}

			if((cur_time - _pre_time) >= 1)
			{
				_pre_time = cur_time;
				_cnt = 1;
			}
			*/

			//处理正常请求
			Request *req = new Request;
			req->_stmp = getTimestamp();
			req->_req = req_src;
			req->_fd = fd;
			req->_ip = ip;
			req->_port = port;
			req->_session_id = session_id;  //该请求的会话id
			
			XCP_LOGGER_INFO(&g_logger, "===req from app:%s\n", req->to_string().c_str());

			//首先先判断req queue 是否已经满了
			if(!(PSGT_Req_Mgt->full()))
			{	
				nRet = PSGT_Req_Mgt->push_req(req);
				if(nRet != 0)
				{
					XCP_LOGGER_ERROR(&g_logger, "push req into req mgt failed, ret:%d, req(%u):%s\n", nRet, buf_len, buf);
					Msg_Oper::send_msg(fd, session_id, "null", "null", "null", ERR_PUSH_QUEUE_FAIL, "push req into req mgt failed.");
				}
				
			}
			else
			{		
				XCP_LOGGER_ERROR(&g_logger, "req mgt is full, req(%u):%s\n", buf_len, buf);			
				Msg_Oper::send_msg(fd, session_id, "null", "null", "null", ERR_QUEUE_FULL, "req mgt is full.");
			}

		}
		
		pos = _buf.find("\n");
			
	}

	return 0;

};



//处理连接关闭事件
int Req_TCP_Event_Handler::handle_close(int fd)
{
	int nRet = 0;
	
	std::string local_ip = "";
	unsigned short local_port = 0;
	get_local_socket(fd, local_ip, local_port);
	
	std::string remote_ip = "";
	unsigned short remote_port = 0; 
	get_remote_socket(fd, remote_ip, remote_port);

	XCP_LOGGER_INFO(&g_logger, "close from app(fd:%d), %s:%u --> %s:%u\n", 
		fd, remote_ip.c_str(), remote_port, local_ip.c_str(), local_port);
	
	PSGT_Session_Mgt->remove_session(fd);
	
	return nRet;
	
};



Event_Handler* Req_TCP_Event_Handler::renew()
{
	return new Req_TCP_Event_Handler;
};


