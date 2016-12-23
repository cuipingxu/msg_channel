
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
 

#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include "base/base_common.h"

USING_NS_BASE;


class XProtocol
{
public:
	static int req_head(const std::string &req, std::string &cmd, unsigned long long &time, std::string &req_id, std::string &err_info);

	static std::string rsp_msg(const std::string &cmd, const std::string &req_id, const std::string &msg_id,  
		const int err, const std::string &err_info, const std::string &body="", bool is_object = false);

	static int req_body(const std::string &req, std::string &body, std::string &err_info);	
};


#endif


