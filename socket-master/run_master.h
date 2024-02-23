#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS //忽略关于inet_addr()的错误
#include<WinSock2.h>
#include<iostream>
#include<string>
#include<thread>
#pragma comment(lib,"ws2_32.lib")
namespace msock {
	bool worker();
}
namespace multimsock {
	//连接信息结构体
	struct CInfo {
		struct sockaddr_in c_addr;//与master连接的client的地址信息
		SOCKET fd;//文件操作符
	};
	
	bool worker();
	bool staff(void* s);
}