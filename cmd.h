
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
 
#ifndef _CMD_H
#define _CMD_H

const std::string CMD_HB = "hb";                                                                        //心跳
const std::string CMD_REGISTER = "register";                                                            //注册服务
const std::string CMD_UNREGISTER = "unregister";                                                        //注销服务
const std::string CMD_LB = "lb";                                                                        //负载均衡
const std::string CMD_AUTH = "auth";                                                                    //鉴权
const std::string CMD_GET = "http_get";                                                                 //http-get
const std::string CMD_POST = "http_post";                                                               //http-post

#endif

