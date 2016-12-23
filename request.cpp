
#include "request.h"
#include "base/base_string.h"
#include "base/base_logger.h"

extern Logger g_logger;

Request::Request(): _msg_id(""), _req_id(""), _req(""), 
	_fd(-1), _ip(""), _port(0), _stmp(0), _app_stmp(0), _session_id("")
{

}


Request::~Request()
{

}


void Request::log()
{
	XCP_LOGGER_INFO(&g_logger, "[req]: msg_id=%s, req_id=%s, fd=%d, stmp:%llu, app_stmp:%llu, app=(%s:%u), session id:%s, req(%u):%s\n", 
		_msg_id.c_str(), _req_id.c_str(), _fd, _stmp, _app_stmp, _ip.c_str(), _port, _session_id.c_str(), _req.size(), _req.c_str());
}


std::string Request::to_string()
{
	return format("[req]: msg_id=%s, req_id=%s, fd=%d, stmp:%llu, app_stmp:%llu, app=(%s:%u), session id:%s, req(%u):%s", 
		_msg_id.c_str(), _req_id.c_str(), _fd, _stmp, _app_stmp, _ip.c_str(), _port, _session_id.c_str(), _req.size(), _req.c_str());
}



