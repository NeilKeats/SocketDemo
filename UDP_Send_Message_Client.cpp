
/*

Socket客户端代码
向服务器发送消息
服务器IP：127.0.0.1
服务器端口：8887
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
	SOCKET client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (client_socket == INVALID_SOCKET) {
		//如果创建的socket无效，则结束程序
		perror("socket error !\n");
		return 0;
	}

	/*
	创建地址，server_addr，并设置端口和IP
	*/
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	//要连接的服务器端口号 8887
	server_addr.sin_port = htons(8887);
	//绑定服务器的地址127.0.0.1
	InetPton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);

	printf("请输入发送给服务器的消息：\n");

	while (1)
	{

		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		
		// 从键盘中读取输入到BUFFER中
		gets_s(buffer, BUFFER_SIZE);

		//发送数据给服务器
		if (sendto(client_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, sizeof(SOCKADDR)) == -1)
		{
			perror("发送消息失败:\n");
			break;
		}


	}


	// 关闭socket 
	closesocket(client_socket);
	WSACleanup();
	return 0;

}