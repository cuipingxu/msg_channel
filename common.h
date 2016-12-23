
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
 

#ifndef _COMMON_H
#define _COMMON_H

#include "base/base_common.h"
#include "cmd.h"
#include "error_code.h"

USING_NS_BASE;


//---------------------------  全局常量-------------------------------


//----------- 系统限制-----------

const unsigned int MIN_MSG_LEN = 50;             //最小消息长度{"head":{"cmd":"hb", "time":111111, "msg_id":"xxxxxx"}}
const unsigned int MAX_MSG_LEN = 20480;          //最大消息长度20KB
const unsigned int MSG_CNT_PER_SEC = 100;        //每个链接每秒中最多发送几条消息，缺省20条    


//---------------------------  全局枚举-------------------------------



//---------------------------  全局结构体-------------------------------


//系统配置结构体
typedef struct StSysInfo
{
	std::string _id;                             //原始ID
	std::string _new_id;                         //新ID
	std::string _ip;                             //IP地址
	unsigned short _port;                        //独立端口
	unsigned short _thr_num;                     //工作线程个数
	unsigned int _max_queue_size;                //请求队列最大尺寸
	unsigned int _rate_limit;                    //速率限制
	unsigned int _timeout_conn;                  //http连接超时
	unsigned int _timeout_rw;                    //http读写超时
	std::string _url;                            //url地址
	std::string _TZ;                             //时区
	
	StSysInfo():_id(""), _new_id(""), _ip(""), _port(0), _thr_num(0), _max_queue_size(0), 
		_rate_limit(0), _timeout_conn(3), _timeout_rw(3), _url(""), _TZ("")
	{
	}

	void clear()
	{
		_id = "";
		_new_id = "";
		_ip = "";
		_port = 0;
		_thr_num = 0;
		_max_queue_size = 0;
		_rate_limit = 0;
		_timeout_conn = 3;
		_timeout_rw = 3;
		_url = "";
		_TZ = "";
		
	}
	
}StSysInfo;


#endif

