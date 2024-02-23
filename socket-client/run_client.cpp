#include "run_client.h"

namespace mSock {
	bool worker() {
		
		/*
		* init lib
		*/
		WSADATA socket_info;
		if (WSAStartup(MAKEWORD(2, 2), &socket_info) == -1) {
			std::cout << "init ws2_32.lib failed: " << WSAGetLastError() << "\n";
			return FALSE;
		}
		std::cout << "WSAStartup" << "\n";
		/*
		* -------------socket()---------------
		* AF_INET : IPV4
		* AF_INET6 : IPV6
		* SOCK_STREAM : use for TCP
		* SOCK_DGRAM : use for UDP
		* 0 : automatically select protocol
		*/
		SOCKET cli_socket;
		if ((cli_socket = socket(AF_INET, SOCK_STREAM, 0)) == (SOCKET_ERROR | INVALID_SOCKET)) {
			std::cout << "structer socket error or invalid: " << WSAGetLastError() << "\n";
			return FALSE;
		}
		std::cout << "socket" << "\n";

		//-------------connect()---------------
		struct sockaddr_in addr;//storage address infomation of master-socket
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");//communication address, must set an ip but not "INADDR_ANY"
		addr.sin_port = htons(6666); //communication port£¬must equal to master's comm port
		addr.sin_family = AF_INET; //address family
		if (connect(cli_socket, (struct sockaddr*)&addr, sizeof(sockaddr_in)) == -1) {
			std::cout << "send failed: " << WSAGetLastError() << "\n";
			return FALSE;
		}
		std::cout << "connect success." << "\n";

		/*
		* -------------send()-----------------
		*/
		std::string send_message = "hello i am client.";
		while(1){
			if (send(cli_socket, send_message.c_str(), sizeof(send_message), 0) == -1) {
				std::cout << "send failed: " << WSAGetLastError() << "\n";
				break;
			}
			//std::cin >> send_message;
			if (send_message == "exit") {
				exit(0);
			}
			else if (send_message == "goon") {
				break;
			}
			Sleep(1000);
		}

		/*
		* -------------recv()-----------------
		*/
		char recv_message[1024];
		int recv_size = recv(cli_socket, recv_message, 1024, 0);
		if (recv_size == 0) {
			std::cout << "close connect successfully: " << WSAGetLastError() << "\n";
		}
		else if (recv_size < 0) {
			std::cout << "close connect error: " << WSAGetLastError() << "\n";
		}
		else{
			std::cout << recv_message << "\n";
		}
		Sleep(3000);//ÑÓ³Ù3s
		closesocket(cli_socket);
		WSACleanup();
		return TRUE;
	}
}