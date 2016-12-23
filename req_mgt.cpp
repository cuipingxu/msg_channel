

#include "req_mgt.h"
#include "base/base_args_parser.h"
#include "base/base_logger.h"
#include "conf_mgt.h"

extern Logger g_logger;
extern StSysInfo g_stSysInfo;


Req_Mgt::Req_Mgt()
{
	_queue = new X_Queue<Request_Ptr>(g_stSysInfo._max_queue_size);
}


Req_Mgt::~Req_Mgt()
{


}

int Req_Mgt::push_req(Request_Ptr req)
{
	return _queue->push(req);
}


int Req_Mgt::get_req(Request_Ptr &req)
{
	return _queue->pop(req);	
}


bool Req_Mgt::full()
{
	return _queue->full();
}


