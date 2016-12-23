
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
 * Author: cuipingxu918@qq.com
 */
 

#ifndef _MSG_OPER_H
#define _MSG_OPER_H

#include "base/base_common.h"
#include "base/base_thread_mutex.h"

USING_NS_BASE;


class Msg_Oper
{
public:
	
	static int send_msg(int fd, const std::string &session_id, const std::string &cmd, const std::string &req_id, const std::string &msg_id,  
		const int err, const std::string &error_info, const std::string &body="", bool is_object=false);

	static int send_msg(int fd, const std::string &session_id, const std::string &msg);


private:
	static Thread_Mutex _mutex_msg;
	
};


#endif


