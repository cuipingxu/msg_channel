
#include "protocol.h"
#include "base/base_time.h"
#include "base/base_convert.h"
#include "base/base_logger.h"
#include "common.h"
#include "json-c/json.h"

extern Logger g_logger;


int XProtocol::req_head(const std::string &req, std::string &cmd, unsigned long long &time, std::string &req_id, std::string &err_info)
{
	err_info = "";
	
	enum json_tokener_error jerr;
	json_object *root = json_tokener_parse_verbose(req.c_str(), &jerr);
	if(jerr != json_tokener_success)
	{
		XCP_LOGGER_INFO(&g_logger, "json_tokener_parse_verbose failed, req:%s, err:%s\n", 
			req.c_str(), json_tokener_error_desc(jerr));
		err_info = "it is invlid json req.";
		return -1;
	}

	//head
	json_object *head = NULL;
	if(!json_object_object_get_ex(root, "head", &head))
	{
		XCP_LOGGER_INFO(&g_logger, "it is invalid req, no head, req(%u):%s\n", req.size(), req.c_str());
		err_info = "it is invalid req, no head.";
		json_object_put(root);
		return -1;
	}

	//cmd
	json_object *_cmd = NULL;
	if(!json_object_object_get_ex(head, "cmd", &_cmd))
	{
		XCP_LOGGER_INFO(&g_logger, "it is invalid req, no cmd, req(%u):%s\n", req.size(), req.c_str());
		err_info = "it is invalid req, no cmd.";
		json_object_put(root);
		return -1;
	}
	if(!json_object_is_type(_cmd, json_type_string))
	{
		XCP_LOGGER_INFO(&g_logger, "it is invalid req, cmd isn't string, req(%u):%s\n", req.size(), req.c_str());
		err_info = "it is invalid req, cmd isn't string.";
		json_object_put(root);
		return -1;

	}
	cmd = json_object_get_string(_cmd);


	//time
	json_object *_time = NULL;
	if(!json_object_object_get_ex(head, "time", &_time))
	{
		time = 0;
	}
	else
	{
		if(!json_object_is_type(_time, json_type_int))
		{
			XCP_LOGGER_INFO(&g_logger, "it is invalid req, time isn't int, req(%u):%s\n", req.size(), req.c_str());
			err_info = "it is invalid req, time isn't int.";
			json_object_put(root);
			return -1;
		}
		time = (unsigned long long)json_object_get_int64(_time);
	}

	
	//req_id
	json_object *_req_id = NULL;
	if(!json_object_object_get_ex(head, "req_id", &_req_id))
	{
		req_id = "";
	}
	else
	{
		if(!json_object_is_type(_req_id, json_type_string))
		{
			XCP_LOGGER_INFO(&g_logger, "it is invalid req, req_id isn't string, req(%u):%s\n", req.size(), req.c_str());
			err_info = "it is invalid req, req_id isn't string.";
			json_object_put(root);
			return -1;
		}
		req_id = json_object_get_string(_req_id);
	}

	//释放内存
	json_object_put(root);
	
	return 0;

}




std::string XProtocol::rsp_msg(const std::string &cmd, const std::string &req_id, const std::string &msg_id,  
	const int err, const std::string &err_info, const std::string &body, bool is_object)
{
	std::string rsp = "";
	
	if(body == "")
	{
		char msg[512] = {0};
		if(req_id == "")
		{
			snprintf(msg, 512, "{\"head\":{\"cmd\":\"%s\", \"time\":%llu, \"msg_id\":\"%s\", \"err\":%d, \"err_info\":\"%s\"}}\n", 
				cmd.c_str(), getTimestamp(), msg_id.c_str(), err, err_info.c_str());		
		}
		else
		{
			snprintf(msg, 512, "{\"head\":{\"cmd\":\"%s\", \"time\":%llu, \"req_id\":\"%s\", \"msg_id\":\"%s\", \"err\":%d, \"err_info\":\"%s\"}}\n", 
				cmd.c_str(), getTimestamp(), req_id.c_str(), msg_id.c_str(), err, err_info.c_str());
		}
		rsp = msg;
	}
	else
	{
		unsigned int len = body.size()+512;
		char *msg = new char[len];
		memset(msg, 0x0, len);
		if(req_id == "")
		{
			if(is_object)
			{
				snprintf(msg, len, "{\"head\":{\"cmd\":\"%s\", \"time\":%llu, \"msg_id\":\"%s\", \"err\":%d, \"err_info\":\"%s\"}, \"body\":%s}\n", 
					cmd.c_str(), getTimestamp(), msg_id.c_str(), err, err_info.c_str(), body.c_str());			
			}
			else
			{
				snprintf(msg, len, "{\"head\":{\"cmd\":\"%s\", \"time\":%llu, \"msg_id\":\"%s\", \"err\":%d, \"err_info\":\"%s\"}, \"body\":\"%s\"}\n", 
					cmd.c_str(), getTimestamp(), msg_id.c_str(), err, err_info.c_str(), body.c_str());
			}
		}
		else
		{
			if(is_object)
			{
				snprintf(msg, len, "{\"head\":{\"cmd\":\"%s\", \"time\":%llu, \"req_id\":\"%s\", \"msg_id\":\"%s\", \"err\":%d, \"err_info\":\"%s\"}, \"body\":%s}\n", 
					cmd.c_str(), getTimestamp(), req_id.c_str(), msg_id.c_str(), err, err_info.c_str(), body.c_str());
			}
			else
			{
				snprintf(msg, len, "{\"head\":{\"cmd\":\"%s\", \"time\":%llu, \"req_id\":\"%s\", \"msg_id\":\"%s\", \"err\":%d, \"err_info\":\"%s\"}, \"body\":\"%s\"}\n", 
					cmd.c_str(), getTimestamp(), req_id.c_str(), msg_id.c_str(), err, err_info.c_str(), body.c_str());
			}
		}
		rsp = msg;
		DELETE_POINTER_ARR(msg);
		
	}

	return rsp;

	
}



int XProtocol::req_body(const std::string &req, std::string &body, std::string &err_info)
{
	err_info = "";
	
	enum json_tokener_error jerr;
	json_object *root = json_tokener_parse_verbose(req.c_str(), &jerr);
	if(jerr != json_tokener_success)
	{
		XCP_LOGGER_INFO(&g_logger, "json_tokener_parse_verbose failed, req:%s, err:%s\n", 
			req.c_str(), json_tokener_error_desc(jerr));
		err_info = "it is invlid json req.";
		return -1;
	}

	//body
	json_object *_body = NULL;
	if(!json_object_object_get_ex(root, "body", &_body))
	{
		XCP_LOGGER_INFO(&g_logger, "it is invalid req, no body, req(%u):%s\n", req.size(), req.c_str());
		err_info = "it is invalid req, no body.";
		json_object_put(root);
		return -1;
	}
	if(!json_object_is_type(_body, json_type_string))
	{
		XCP_LOGGER_INFO(&g_logger, "it is invalid req, body isn't string, req(%u):%s\n", req.size(), req.c_str());
		err_info = "it is invalid req, body isn't string.";
		json_object_put(root);
		return -1;

	}
	body = json_object_get_string(_body);

	//释放内存
	json_object_put(root);

	return 0;

}

