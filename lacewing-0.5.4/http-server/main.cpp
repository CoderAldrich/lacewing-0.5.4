#include "include\lacewing.h"
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <string>
#include <shlwapi.h>
#include <string>
#include <fstream> 
#include <iosfwd>

#ifdef _DEBUG
#pragma comment(lib, "..\\msvc\\full static-lib-debug\\liblacewing-static-d.lib")
#else
#pragma comment(lib, "..\\msvc\\full static-lib-release\\liblacewing-static-r.lib")
#endif // DEBUG

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")
#pragma comment(lib,"Secur32.lib")
#pragma comment(lib,"crypt32.lib")
#pragma comment(lib,"shlwapi.lib")

// ������Ӧclient�˵�get����Ϊ�䷢����������;������������ӿڣ���Բ�ͬ���Ƶ���ӦҲ�������������
void on_get(lacewing::webserver, lacewing::webserver_request request)
{
	if (strcmp(request->url(), "config") == 0)
	{
		// client��ȡ��������
		request->writef("config");
		return;
	}
	else if (strcmp(request->url(), "loot") == 0)
	{
		// �û������ս��Ʒ
		return;
	}
	else if (strcmp(request->url(), "store") == 0)
	{
		// �û�������̳�
		request->writef("store");
		return;
	}
	else if (strcmp(request->url(), "rank5x5") == 0)
	{
		// �û�������������λ
		return;
	}
	else if (strcmp(request->url(), "rank2x2") == 0)
	{
		// �û������˵�/˫��
		return;
	}
	else if (strcmp(request->url(), "rank3x3") == 0)
	{
		// �û������˴�����λ
		return;
	}
	else if (strcmp(request->url(), "rank1100") == 0)
	{
		// �û��������ƶ�֮����λ
		return;
	}
	else
	{
		request->writef("false");
	}

}

// ��������
void on_post(lacewing::webserver, lacewing::webserver_request request)
{
	request->writef("Hello %s!  You posted:<br /><br />", request->POST("name"));

	for (lacewing::webserver_request_param param
		= request->POST(); param; param = param->next())
	{
		request->writef("%s = %s", param->name(), param->value());
	}
}

int main()
{
	// �Ӻ�̨��ȡ�����ļ���������Ҫ����ĸ���ť
	// todo


	// �������ã�׼�����ݣ�����Ϊclient���ṩ����������ĸ���ť����ͬ�����Ʊ�����ʱ���͸��ĸ�url�ӿ�
	// todo


	// ����httpserver����get/post����
	lacewing::eventpump eventpump = lacewing::eventpump_new();
	lacewing::webserver webserver = lacewing::webserver_new(eventpump);

	webserver->on_get(on_get);
	webserver->on_post(on_post);

	webserver->host(8888);

	eventpump->start_eventloop();

	lacewing::webserver_delete(webserver);
	lacewing::pump_delete(eventpump);
	return 0;
}