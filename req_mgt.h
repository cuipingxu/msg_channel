
/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: 89617663@qq.com
 */

 

#ifndef _REQ_MGT_H
#define _REQ_MGT_H

#include "base/base_common.h"
#include "base/base_queue.h"
#include "base/base_singleton_t.h"
#include "base/base_thread_mutex.h"
#include "request.h"

USING_NS_BASE;

class Req_Mgt
{
public:
	Req_Mgt();

	~Req_Mgt();

	int push_req(Request_Ptr req);

	int get_req(Request_Ptr &req);

	bool full();
	
private:
	X_Queue<Request_Ptr> *_queue;
	Thread_Mutex _mutex;

	//_msg_id_  <---> Request  ����worker �첽ͨѶ
	std::map<std::string, Request_Ptr> _reqs;
	
};

#define PSGT_Req_Mgt Singleton_T<Req_Mgt>::getInstance()


#endif


