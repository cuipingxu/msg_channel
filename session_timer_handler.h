
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

 

#ifndef _SESSION_TIMER_HANDLER_H
#define _SESSION_TIMER_HANDLER_H


#include "base/base_common.h"
#include "base/base_timer_select.h"


USING_NS_BASE;

class Session_Timer_handler : public Select_Timer_handler
{
public:
	Session_Timer_handler();

	virtual ~Session_Timer_handler();

	virtual int handle_timeout(void *args = NULL);
};

#endif


