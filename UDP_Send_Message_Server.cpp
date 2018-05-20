
/*

Socket服务器端代码
服务器端口8887
接收Client发送的消息
*/

#include <tchar.h>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>   
#include <string.h>   
#include <Ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

#define BUFFER_SIZE 2048
#define FILE_NAME_MAX_SIZE 512 

int main(int argc, char* argv[])
{
	/*
	初始化WSA，使得程序可以调用windows socket
	*/
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	/*
	创建监听用套接字，server_socket
	类型是UDP
	并检测是否创建成功
	*/
	SOCKET server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (server_socket == INVALID_SOCKET) {
		//如果创建的socket无效，则结束程序
		perror("socket error !\n");
		return 0;
	}

	/*
	创建地址，server_addr，并设置端口和IP
	*/
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	//端口号 8887
	server_addr.sin_port = htons(8887);
	//INADDR_ANY表示任意IP地址
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;


	//将socket与地址server_addr绑定
	if (bind(server_socket, (LPSOCKADDR)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
		perror("bind error !\n");
		return 0;
	}


	while (1)
	{
		// 定义客户端的socket地址结构
		sockaddr_in client_addr;
		int client_addr_length = sizeof(client_addr);

		// recv函数接收数据到缓冲区buffer中 
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		if (recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_addr_length) == -1)
		{
			perror("接收消息失败:\n");
			break;
		}

		char IP_BUFFER[256];
		memset(IP_BUFFER, 0, 256);
		InetNtop(AF_INET, &client_addr.sin_addr, IP_BUFFER, 256);
		printf("从%s：%d处收到消息:%s\n", IP_BUFFER, ntohs(client_addr.sin_port), buffer);

	}


	// 关闭服务器socket 
	closesocket(server_socket);
	WSACleanup();
	return 0;

}