#include "run_master.h"
namespace msock {
	bool worker() {
		/*
		* init lib
		*/
		WSADATA socket_info;
		if (WSAStartup(MAKEWORD(2, 2), &socket_info) == -1) {
			std::cout << "init ws2_32.lib failed: " << WSAGetLastError() << "\n";
		}

		/*
		* -------------socket()---------------
		* AF_INET : IPV4
		* AF_INET6 : IPV6
		* SOCK_STREAM : use for TCP
		* SOCK_DGRAM : use for UDP
		* 0 : automatically select protocol
		*/
		SOCKET mast_socket;
		if ((mast_socket = socket(AF_INET, SOCK_STREAM, 0)) == (SOCKET_ERROR | INVALID_SOCKET)) {
			std::cout << "structer socket error or invalid: " << WSAGetLastError() << "\n";
			WSACleanup();
			return 0;
		}

		//-------------bind()---------------
		struct sockaddr_in addr;//storage address infomation of master-socket
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");//INADDR_ANY, listen any address
		addr.sin_port = htons(6666); //port to listened
		addr.sin_family = AF_INET; //address family
		/*
		m_socket : master-socket
		*/
		if (bind(mast_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
			std::cout << "bind error: " << WSAGetLastError() << "\n";
			closesocket(mast_socket);
			WSACleanup();
			return 0;
		}

		/*
		* -------------listen()---------------
		args_1 : master_socket
		args_2 : max number of client that waiting to link.等待连接的客户端的最大数量
		*/
		if (listen(mast_socket, 256) == -1) {
			std::cout << "set listening failed: " << WSAGetLastError() << "\n";
			closesocket(mast_socket);
			WSACleanup();
			return 0;
		}

		/*
		* -------------accept()---------------
		*/
		sockaddr cli_addr;//client's address
		int size = sizeof(cli_addr);
		SOCKET cli_socket;//client-socket, used for function's arg of send() or receive()
		if ((cli_socket = accept(mast_socket, &cli_addr, &size)) == -1) {
			std::cout << "accept connect failed: " << WSAGetLastError() << "\n";
			closesocket(mast_socket);
			WSACleanup();
			return 0;
		}

		/*
		* -------------send()-----------------
		*/
		char send_message[] = "hello i am master.";
		if (send(cli_socket, send_message, sizeof(send_message), 0) == -1) {
			std::cout << "send failed: " << WSAGetLastError() << "\n";
			closesocket(mast_socket);
			closesocket(cli_socket);
			WSACleanup();
			return 0;
		}

		/*
		* -------------recv()-----------------
		*/
		char recv_message[1024];
		int recv_size = recv(cli_socket, recv_message, 1024, 0);
		if (recv_size <= 0) {
			std::cout << "receive failed: " << WSAGetLastError() << "\n";
			closesocket(mast_socket);
			closesocket(cli_socket);
			WSACleanup();
			return 0;
		}
		std::cout << recv_message << "\n";

		closesocket(mast_socket);
		closesocket(cli_socket);
		WSACleanup();
		return 0;
	}
}

namespace multimsock {
	bool worker() {
		/*
		* init lib
		*/
		WSADATA socket_info;
		if (WSAStartup(MAKEWORD(2, 2), &socket_info) == -1) {
			std::cout << "init ws2_32.lib failed: " << WSAGetLastError() << "\n";
		}

		/*
		* -------------socket()---------------
		* AF_INET : IPV4
		* AF_INET6 : IPV6
		* SOCK_STREAM : use for TCP
		* SOCK_DGRAM : use for UDP
		* 0 : automatically select protocol
		*/
		SOCKET mast_socket;
		if ((mast_socket = socket(AF_INET, SOCK_STREAM, 0)) == (SOCKET_ERROR | INVALID_SOCKET)) {
			std::cout << "structer socket error or invalid: " << WSAGetLastError() << "\n";
			return 0;
		}

		//-------------bind()---------------
		struct sockaddr_in addr;//storage address infomation of master-socket
		addr.sin_addr.s_addr = INADDR_ANY;// link with any ip
		//addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //only link with ip:127.0.0.1
		addr.sin_port = htons(6666); //communication port, when set a port number, client must the same port number
		addr.sin_family = AF_INET; //address family
		/*
		m_socket : master-socket
		*/
		if (bind(mast_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
			std::cout << "bind error: " << WSAGetLastError() << "\n";
			closesocket(mast_socket);
			WSACleanup();
			return 0;
		}

		/*
		* -------------listen()---------------
		* args_1 : master_socket
		* args_2 : max number of client that allow waiting to link.等待连接的客户端的最大数量
		*/
		if (listen(mast_socket, 256) == -1) {
			std::cout << "set listening failed: " << WSAGetLastError() << "\n";
			closesocket(mast_socket);
			WSACleanup();
			return 0;
		}

		/*
		* -------------accept()---------------
		*/
		int len = sizeof(sockaddr_in);
		struct CInfo c_socks[128]; //最多与128个client连接
		int maxsize = sizeof(c_socks) / sizeof(c_socks)[0];
		std::cout << "maxsize: " << maxsize << "\n";
		//初始化
		for (int i = 0; i < maxsize; i++)
		{
			memset(c_socks, 0, sizeof(c_socks[0]));
			c_socks[i].fd = 0;
		}

		while (1) {
			CInfo* temp = nullptr;
			//遍历c_socks，若当前内存块中CInfo.fd为0则说明未使用，然后指针指向该内存
			for (int i = 0; i < maxsize; i++) {
				if (c_socks[i].fd == 0) {
					temp = &c_socks[i];
					break;
				}
			}
			if (temp == nullptr) {
				//若达到最大连接数则延迟3s
				std::cout << "please wait for linking... " << "\n";
				std::this_thread::sleep_for(std::chrono::milliseconds(3000));
				continue;
			}
			//client-socket, used for function's arg of send() or receive()
			SOCKET cli_socket = accept(mast_socket, (struct sockaddr*)&(temp->c_addr), &len);
			if (cli_socket == -1) {
				std::cout << "accept connect failed: " << WSAGetLastError() << "\n";
				continue;
			}
			std::cout << "space: " << temp << "---using." << "\n";
			std::cout << "accept connect success: " << "\n";
			temp->fd = cli_socket;// save fd
			std::thread t(staff, temp);
			t.detach();

		}
		WSACleanup();//执行后将输出10004并卸载Winsock，再次调用将输出10093
	}


	bool staff(void* temp) {
		CInfo* s = (CInfo*)temp;
		char* ip;//当前客户端ip地址
		int port;//当前客户端通信端口号
		ip = inet_ntoa(s->c_addr.sin_addr);
		port = ntohs(s->c_addr.sin_port);
		/*
		* -------------send()-----------------
		*/
		char send_message[] = "hello i am master.";
		if (send(s->fd, send_message, sizeof(send_message), 0) == -1) {
			std::cout << "send failed: " << WSAGetLastError() << "\n";
		}

		/*
		* -------------recv()-----------------
		*/
		while (1)
		{
			char recv_message[1024];
			int recv_size = recv(s->fd, recv_message, 1024, 0);
			if (recv_size == 0) {
				std::cout << "close connect successfully: " << WSAGetLastError() << "\n";
				break;
			}
			else if (recv_size < 0) {
				std::cout << "close connect error: " << WSAGetLastError() << "\n";
				break;
			}
			std::cout << ip << ":" << port << "---receive: " << recv_message << "\n";

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		closesocket(s->fd);
		s->fd = 0;//将当前指向的内存块置为可用于存储连接信息
		return 0;
	}

}