
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


//---------------------------  ȫ�ֳ���-------------------------------


//----------- ϵͳ����-----------

const unsigned int MIN_MSG_LEN = 50;             //��С��Ϣ����{"head":{"cmd":"hb", "time":111111, "msg_id":"xxxxxx"}}
const unsigned int MAX_MSG_LEN = 20480;          //�����Ϣ����20KB
const unsigned int MSG_CNT_PER_SEC = 100;        //ÿ������ÿ������෢�ͼ�����Ϣ��ȱʡ20��    


//---------------------------  ȫ��ö��-------------------------------



//---------------------------  ȫ�ֽṹ��-------------------------------


//ϵͳ���ýṹ��
typedef struct StSysInfo
{
	std::string _id;                             //ԭʼID
	std::string _new_id;                         //��ID
	std::string _ip;                             //IP��ַ
	unsigned short _port;                        //�����˿�
	unsigned short _thr_num;                     //�����̸߳���
	unsigned int _max_queue_size;                //����������ߴ�
	unsigned int _rate_limit;                    //��������
	unsigned int _timeout_conn;                  //http���ӳ�ʱ
	unsigned int _timeout_rw;                    //http��д��ʱ
	std::string _url;                            //url��ַ
	std::string _TZ;                             //ʱ��
	
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

