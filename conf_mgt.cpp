
#include "base/base_os.h"
#include "base/base_convert.h"
#include "base/base_string.h"
#include "base/base_xml_parser.h"
#include "base/base_logger.h"
#include "conf_mgt.h"

extern Logger g_logger;

Conf_Mgt::Conf_Mgt(): _cfg("")
{

}


Conf_Mgt::~Conf_Mgt()
{

}



int Conf_Mgt::init(const std::string &cfg)
{
	int nRet = 0;
	
	_cfg = cfg;
	return refresh();	
}



int Conf_Mgt::refresh()
{
	int nRet = 0;
		
	XML_Parser _parser;
	nRet = _parser.parse_file(_cfg);
	if(nRet != 0)
	{
		printf("init conf mgt failed, ret:%d, cfg:%s\n", nRet, _cfg.c_str());
		XCP_LOGGER_INFO(&g_logger, "init conf mgt failed, ret:%d, cfg:%s\n", nRet, _cfg.c_str());
		return nRet;
	}
	
	
	//---------------------- sysinfo ---------------------
	
	StSysInfo stSysInfo;
	
	//id
	XML_Node node;
	nRet = _parser.get_node("msg_channel/system/id", node);
	if(nRet != 0)
	{
		printf("get id failed, ret:%d\n", nRet);
		return -1;
	}
	stSysInfo._id = node.get_text();
	

	//ip
	nRet = _parser.get_node("msg_channel/system/ip", node);
	if(nRet != 0)
	{
		printf("get ip failed, ret:%d\n", nRet);
	}
	stSysInfo._ip = node.get_text();
	
	
	//port
	nRet = _parser.get_node("msg_channel/system/port", node);
	if(nRet != 0)
	{
		printf("get port failed, ret:%d\n", nRet);
		return -1;
	}
	stSysInfo._port = (unsigned short)atoi(node.get_text().c_str());


	//new id:[id]_[ip_port]
	stSysInfo._new_id = format("%s_%s_%u", stSysInfo._id.c_str(), stSysInfo._ip.c_str(), stSysInfo._port);
	
	//thr_num
	nRet = _parser.get_node("msg_channel/system/thr_num", node);
	if(nRet != 0)
	{
		printf("get thr_num failed, ret:%d\n", nRet);
		get_cpu_number_proc(stSysInfo._thr_num);
	}
	else
	{
		stSysInfo._thr_num = (unsigned short)atoi(node.get_text().c_str());
		if(stSysInfo._thr_num == 0)
		{
			printf("thr_num is 0\n");
			return -1;
		}		
	}

	//max_queue_size
	nRet = _parser.get_node("msg_channel/system/max_queue_size", node);
	if(nRet != 0)
	{
		printf("get max_queue_size failed, ret:%d\n", nRet);
	}
	stSysInfo._max_queue_size = (unsigned int)atoi(node.get_text().c_str());
	if(stSysInfo._max_queue_size < 100000)
	{
		printf("max_queue_size < 100000\n");
		stSysInfo._max_queue_size = 100000;
	}

	//timeout_conn
	nRet = _parser.get_node("msg_channel/system/timeout_conn", node);
	if(nRet != 0)
	{
		printf("get timeout_conn failed, ret:%d\n", nRet);
	}
	stSysInfo._timeout_conn = (unsigned int)atoi(node.get_text().c_str());


	//timeout_rw
	nRet = _parser.get_node("msg_channel/system/timeout_rw", node);
	if(nRet != 0)
	{
		printf("get timeout_rw failed, ret:%d\n", nRet);
	}
	stSysInfo._timeout_rw = (unsigned int)atoi(node.get_text().c_str());
	

	//url
	nRet = _parser.get_node("msg_channel/system/url", node);
	if(nRet != 0)
	{
		printf("get ip failed, ret:%d\n", nRet);
		return -1;
	}
	stSysInfo._url = node.get_text();
	

	//TZ
	nRet = _parser.get_node("msg_channel/system/TZ", node);
	if(nRet != 0)
	{
		printf("get TZ failed, ret:%d\n", nRet);
	}
	else
	{
		stSysInfo._TZ = node.get_text();
	}
	
	if(true)
	{
		Thread_Mutex_Guard guard(_mutex);
		_stSysInfo = stSysInfo;
	}
	
	return 0;

}



//获取副本
StSysInfo Conf_Mgt::get_sysinfo()
{
	Thread_Mutex_Guard guard(_mutex);
	return _stSysInfo;
}

