#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS//���Թ���inet_addr()�Ĵ���
#include<iostream>
#include<WinSock2.h>
#include<string>
#include<thread>
#pragma comment(lib,"ws2_32.lib")
namespace mSock {
	bool worker();
}