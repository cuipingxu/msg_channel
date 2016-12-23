#include "base/base_common.h"
#include "base/base_args_parser.h"
#include "base/base_reactor.h"
#include "base/base_signal.h"
#include "base/base_logger.h"
#include "base/base_time.h"
#include "base/base_os.h"
#include "base/base_utility.h"

#include "admin.h"
#include "conf_mgt.h"
#include "processor.h"
#include "req_tcp_event_handler.h"
#include "session_timer_handler.h"

USING_NS_BASE;

Logger g_logger;         //�ڲ���־��ӡ��
StSysInfo g_stSysInfo;

void usage()
{
    fprintf(stderr, 
        "Usage: ./msg_channel.bin -s cfg\n\n"
        "  -s  Auth_cfg\n\n"
        "  build time: %s %s\n"
        "  Bug reports, feedback, etc, to: 89617663@qq.com\n"
        "\n",
        __DATE__, __TIME__
    );
};



int main(int argc, char * argv[])
{

	int nRet = 0;

	//(1) usage
    if (argc != 3) 
	{
        usage();
        return 0;
    }


	//�������������
	set_random_seed();
	
	
	//(2) �ź�����
	sigset_t sigset;
	add_signal_in_set(sigset, 2, SIGPIPE, SIGUSR2);
	nRet = block_process_signal(sigset);
	if(nRet != 0)
	{
		printf("block_process_signal failed, ret:%d\n", nRet);
		return 0;
	}

	//(3) �����������в���
	printf("--- prepare to start parse arg ---\n");
	Args_Parser args_parser;
	args_parser.parse_args(argc, argv);
	args_parser.show();

	std::string cfg = "";
	if(!args_parser.get_opt("s", &cfg))
	{
		printf("get_opt cfg failed!\n");
		return 0;
	}	
	printf("=== complete to start parse arg ===\n");


	//(4) ��ʼ������ģ��
	printf("--- prepare to init conf mgt ---\n");
	nRet = PSGT_Conf_Mgt->init(std::string("../conf/")+cfg);
	if(nRet != 0)
	{
		printf("init conf mgt failed, ret:%d\n", nRet);
		return 0;
	}
	g_stSysInfo = PSGT_Conf_Mgt->get_sysinfo();
	printf("===== complete to init conf mgt =====\n");


	//(5)����TZ
	if(g_stSysInfo._TZ != "")
	{
		char tz[100] = {0};
		snprintf(tz, 100, "TZ=%s", (g_stSysInfo._TZ).c_str());
		nRet = putenv(tz);
		if(nRet != 0)
		{
			printf("putenv(%s) failed, ret:%d\n", tz, nRet);
			return nRet;
		}
		else
		{
			printf("putenv(%s) success\n", tz);
		}
	}
	tzset();
	printf("date:%s\n", FormatDateTimeStr().c_str());


	//(6) ��ʼ��logger
	nRet = g_logger.init("./../log/", g_stSysInfo._new_id, MAX_LOG_SIZE, 3600);
	if(nRet != 0)
	{
		printf("init debug logger failed, ret:%d\n", nRet);
		return nRet;
	}


	XCP_LOGGER_INFO(&g_logger, "=============================================\n");
	XCP_LOGGER_INFO(&g_logger, "        date:%s\n", FormatDateTimeStr().c_str());
	XCP_LOGGER_INFO(&g_logger, "        process:%d, thread:%llu \n", get_pid(), get_thread_id());
	XCP_LOGGER_INFO(&g_logger, "        prepare to start msg_channel! ...\n");
	XCP_LOGGER_INFO(&g_logger, "=============================================\n");

	
	//(7) ���������̳߳�
	XCP_LOGGER_INFO(&g_logger, "--- prepare to start processor ---\n");
	Processor processor;
	nRet = processor.init(NULL, g_stSysInfo._thr_num);
	if(nRet != 0)
	{
		printf("processor init failed, ret:%d\n", nRet);
		return nRet;
	}
		
	nRet = processor.run();
	if(nRet != 0)
	{
		printf("processor run failed, ret:%d\n", nRet);
		return nRet;
	}
	XCP_LOGGER_INFO(&g_logger, "=== complete to start processor ===\n");
	

	//(8) ����req tcp reactor
	XCP_LOGGER_INFO(&g_logger, "--- prepare to start req reactor(tcp) ---\n");
	Req_TCP_Event_Handler *req_handler = new Req_TCP_Event_Handler;
	StReactorAgrs args_req;
	args_req.port = g_stSysInfo._port;
	Reactor *reactor_req = new Reactor(req_handler);
	nRet = reactor_req->init(&args_req);
	if(nRet != 0)
	{
		printf("init req reactor(tcp) failed, ret:%d\n", nRet);
		return nRet;
	}
		
	nRet = reactor_req->run();
	if(nRet != 0)
	{
		printf("req reactor(tcp) run failed, ret:%d\n", nRet);
		return nRet;
	}
	XCP_LOGGER_INFO(&g_logger, "=== complete to start req reactor(tcp) ===\n");


	//(9) ����Admin  --- ����ˢ������
	XCP_LOGGER_INFO(&g_logger, "--- prepare to start Admin ---\n");
	Admin admin;
	nRet = admin.init(NULL);
	if(nRet != 0)
	{
		printf("Admin init failed, ret:%d\n", nRet);
		return nRet;
	}
		
	nRet = admin.run();
	if(nRet != 0)
	{
		printf("Admin run failed, ret:%d\n", nRet);
		return nRet;
	}
	XCP_LOGGER_INFO(&g_logger, "=== complete to start Admin ===\n");
	

	//(10)����session timer  --- 30����
	XCP_LOGGER_INFO(&g_logger, "--- prepare to start session timer ---\n");
	Select_Timer *timer_session = new Select_Timer;
	Session_Timer_handler *session_thandler = new Session_Timer_handler;
	nRet = timer_session->register_timer_handler(session_thandler, 30000000);
	if(nRet != 0)
	{
		printf("register session timer handler falied, ret:%d\n", nRet);
		return nRet;
	}
		
	nRet = timer_session->init();
	if(nRet != 0)
	{
		printf("session timer init failed, ret:%d\n", nRet);
		return nRet;
	}

	//nRet = timer_session->run();
	if(nRet != 0)
	{
		printf("session timer run failed, ret:%d\n", nRet);
		return nRet;
	}
	XCP_LOGGER_INFO(&g_logger, "=== complete to start session timer ===\n");
	
	
	//(11) �������
	XCP_LOGGER_INFO(&g_logger, "===== complete to start msg_channel! =====\n");
	
	while(true)
	{
		::sleep(3);
	}
	
	return 0;
	
}


