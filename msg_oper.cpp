
#include "msg_oper.h"
#include "base/base_socket_oper.h"
#include "base/base_logger.h"
#include "base/base_string.h"
#include "session_mgt.h"
#include "protocol.h"


extern Logger g_logger;

Thread_Mutex Msg_Oper::_mutex_msg;

int Msg_Oper::send_msg(int fd, const std::string &session_id, const std::string &cmd, const std::string &req_id, const std::string &msg_id,  
	const int err, const std::string &error_info, const std::string &body, bool is_object)
{
	int nRet = 0;
	
	if(!session_id.empty())
	{
		if(!PSGT_Session_Mgt->is_valid_sesssion(session_id))
		{
			XCP_LOGGER_INFO(&g_logger, "it is invalid session id. fd:%d, session_id:%s\n", fd, session_id.c_str());
			return -1;
		}
	}
	
	std::string msg = XProtocol::rsp_msg(cmd, req_id, msg_id, err, error_info, body, is_object);
	unsigned int len = msg.size();
	{
		Thread_Mutex_Guard guard(_mutex_msg);
		nRet = Socket_Oper::send_s(fd, msg.c_str(), len, 0);
		//nRet = Socket_Oper::send_n(fd, msg.c_str(), len, 0, 1000000);
	}
	
	if(nRet == 0)
	{
		//PSGT_Session_Mgt->update_write(fd, len);
		//XCP_LOGGER_INFO(&g_logger, "send rsp msg success. fd:%d, msg(%u):%s\n", fd, msg.size(), msg.c_str());
	}
	else if(nRet == 2)
	{
		XCP_LOGGER_ERROR(&g_logger, "send rsp msg timeout, ret:%d, fd:%d, msg(%u):%s\n", 
			nRet, fd, msg.size(), msg.c_str());
	}
	else
	{
		XCP_LOGGER_ERROR(&g_logger, "send rsp msg failed, ret:%d, fd:%d, msg(%u):%s\n", 
			nRet, fd, msg.size(), msg.c_str());		
	}
	
	return nRet;

}




int Msg_Oper::send_msg(int fd, const std::string &session_id, const std::string &msg)
{
	int nRet = 0;

	if(!session_id.empty())
	{
		if(!PSGT_Session_Mgt->is_valid_sesssion(session_id))
		{
			XCP_LOGGER_INFO(&g_logger, "it is invalid session id. fd:%d, session_id:%s\n", fd, session_id.c_str());
			return -1;
		}
	}
	
	std::string new_req = msg + std::string("\n");
	unsigned int len = new_req.size();
	{
		Thread_Mutex_Guard guard(_mutex_msg);
		nRet = Socket_Oper::send_s(fd, new_req.c_str(), len, 0);
		//nRet = Socket_Oper::send_n(fd, new_req.c_str(), len, 0, 1000000);
	}	

	if(nRet == 0)
	{
		//PSGT_Session_Mgt->update_write(fd, len);
		//XCP_LOGGER_INFO(&g_logger, "send msg success. fd:%d, msg(%u):%s\n", fd, msg.size(), msg.c_str());
	}
	else if(nRet == 2)
	{
		XCP_LOGGER_ERROR(&g_logger, "send msg timeout, ret:%d, fd:%d, msg(%u):%s\n", 
			nRet, fd, msg.size(), msg.c_str());
	}
	else
	{
		XCP_LOGGER_ERROR(&g_logger, "send msg failed, ret:%d, fd:%d, msg(%u):%s\n", 
			nRet, fd, msg.size(), msg.c_str());	
	}
	
	return nRet;

}


