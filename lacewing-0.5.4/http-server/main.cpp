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

// 用来响应client端的get请求，为其发送配置数据;这里做出多个接口，针对不同限制的相应也可以在这里完成
void on_get(lacewing::webserver, lacewing::webserver_request request)
{
	if (strcmp(request->url(), "config") == 0)
	{
		// client获取配置数据
		request->writef("config");
		return;
	}
	else if (strcmp(request->url(), "loot") == 0)
	{
		// 用户点击了战利品
		return;
	}
	else if (strcmp(request->url(), "store") == 0)
	{
		// 用户点击了商城
		request->writef("store");
		return;
	}
	else if (strcmp(request->url(), "rank5x5") == 0)
	{
		// 用户进入了自由排位
		return;
	}
	else if (strcmp(request->url(), "rank2x2") == 0)
	{
		// 用户进入了单/双排
		return;
	}
	else if (strcmp(request->url(), "rank3x3") == 0)
	{
		// 用户进入了丛林排位
		return;
	}
	else if (strcmp(request->url(), "rank1100") == 0)
	{
		// 用户进入了云顶之弈排位
		return;
	}
	else
	{
		request->writef("false");
	}

}

// 接收数据
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
	// 从后台拉取配置文件，包括需要监控哪个按钮
	// todo


	// 解析配置，准备数据，用来为client端提供，包括监控哪个按钮，不同的限制被触发时发送给哪个url接口
	// todo


	// 创建httpserver监听get/post请求
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