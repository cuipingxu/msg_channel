
#include "processor.h"
#include "req_mgt.h"
#include "common.h"
#include "protocol.h"
#include "base/base_net.h"
#include "base/base_string.h"
#include "base/base_logger.h"
#include "base/base_url_parser.h"
#include "base/base_uid.h"
#include "base/base_utility.h"
#include "base/base_curl.h"
#include "msg_oper.h"
#include "conf_mgt.h"

extern Logger g_logger;
extern StSysInfo g_stSysInfo;

Processor::Processor()
{

}


Processor::~Processor()
{

}


int Processor::do_init(void *args)
{
	int nRet = 0;

	return nRet;
}




int Processor::svc()
{
	int nRet = 0;
	
	//获取req 并且处理
	Request_Ptr req;
	nRet = PSGT_Req_Mgt->get_req(req);
	if(nRet != 0)
	{
		return 0;
	}
	
	XCP_LOGGER_INFO(&g_logger, "prepare to process req from app, %s\n", req->to_string().c_str());

	//解析请求串
	std::string cmd = "";
	unsigned long long time = 0;
	std::string req_id = "";
	std::string err_info = "";
	nRet = XProtocol::req_head(req->_req, cmd, time, req_id, err_info);
	if(nRet != 0)
	{
		XCP_LOGGER_INFO(&g_logger, "it is invalid req, req:%s\n", req->to_string().c_str());
		Msg_Oper::send_msg(req->_fd, req->_session_id, "null", "null", req->_msg_id, ERR_INVALID_REQ, err_info);
		return 0;
	}

	//更新req 信息
	req->_req_id = req_id;
	req->_app_stmp = time;

	if(cmd == CMD_AUTH)
	{
		XCP_LOGGER_INFO(&g_logger, "-------- auth --------\n");
	}
	else if(cmd == CMD_GET || cmd == CMD_POST)
	{
		XCP_LOGGER_INFO(&g_logger, "-------- get --------\n");

		std::string body = "";
		nRet = XProtocol::req_body(req->_req, body, err_info);
		if(nRet != 0)
		{
			XCP_LOGGER_INFO(&g_logger, "it is invalid req, req:%s\n", req->to_string().c_str());
			Msg_Oper::send_msg(req->_fd, req->_session_id, cmd, req_id, req->_msg_id, ERR_INVALID_REQ, err_info);
			return 0;
		}

		XCP_LOGGER_INFO(&g_logger, "prepare to http_get, req:%s\n", req->to_string().c_str());
		
		std::string new_url = g_stSysInfo._url + std::string("/?") + body;
		std::string rsp = "";
		nRet = Curl_Tool::http_get(new_url, rsp, g_stSysInfo._timeout_conn, g_stSysInfo._timeout_rw);
		if(nRet != 0)
		{
			XCP_LOGGER_INFO(&g_logger, "http_get failed, req:%s\n", req->to_string().c_str());
			Msg_Oper::send_msg(req->_fd, req->_session_id, cmd, req_id, req->_msg_id, nRet, "http_get failed");
			return 0;			
		}
		XCP_LOGGER_INFO(&g_logger, "http_get success, req:%s\n", req->to_string().c_str());
		Msg_Oper::send_msg(req->_fd, req->_session_id, cmd, req_id, req->_msg_id, nRet, "http_get success", rsp, true);
		
		
	}
	/*
	else if(cmd == CMD_POST)
	{
		XCP_LOGGER_INFO(&g_logger, "-------- post --------\n");
		
		std::string body = "";
		nRet = XProtocol::req_body(req->_req, body, err_info);
		if(nRet != 0)
		{
			XCP_LOGGER_INFO(&g_logger, "it is invalid req, req:%s\n", req->to_string().c_str());
			Msg_Oper::send_msg(req->_fd, req->_session_id, cmd, req_id, req->_msg_id, ERR_INVALID_REQ, err_info);
			return 0;
		}

		XCP_LOGGER_INFO(&g_logger, "prepare to http_post, req:%s\n", req->to_string().c_str());
		
		std::string rsp = "";
		nRet = Curl_Tool::http_post_msg(g_stSysInfo._url, body, rsp, g_stSysInfo._timeout_conn, g_stSysInfo._timeout_rw);
		if(nRet != 0)
		{
			XCP_LOGGER_INFO(&g_logger, "http_post failed, req:%s\n", req->to_string().c_str());
			Msg_Oper::send_msg(req->_fd, req->_session_id, cmd, req_id, req->_msg_id, nRet, "http_post failed");
			return 0;			
		}
		XCP_LOGGER_INFO(&g_logger, "http_post success, req:%s\n", req->to_string().c_str());
		Msg_Oper::send_msg(req->_fd, req->_session_id, cmd, req_id, req->_msg_id, nRet, "http_post success", rsp, true);

		
	}
	*/
	else
	{
		XCP_LOGGER_INFO(&g_logger, "invalid cmd(%s) from app\n", cmd.c_str());
		Msg_Oper::send_msg(req->_fd, req->_session_id, cmd, req_id, req->_msg_id, ERR_INVALID_REQ, "invalid cmd.");
	}
	
	return 0;
	
}


