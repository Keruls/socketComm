#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS //���Թ���inet_addr()�Ĵ���
#include<WinSock2.h>
#include<iostream>
#include<string>
#include<thread>
#pragma comment(lib,"ws2_32.lib")
namespace msock {
	bool worker();
}
namespace multimsock {
	//������Ϣ�ṹ��
	struct CInfo {
		struct sockaddr_in c_addr;//��master���ӵ�client�ĵ�ַ��Ϣ
		SOCKET fd;//�ļ�������
	};
	
	bool worker();
	bool staff(void* s);
}