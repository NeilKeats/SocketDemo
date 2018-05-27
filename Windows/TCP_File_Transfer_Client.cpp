
/*
	Socket客户端代码
	服务器127.0.0.1通信，端口8888
	建立连接后，发送给服务器，需要传输的文件路径
	若文件路径存在，接收服务器发送的文件流，发送完毕则关闭连接。
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
	并检测是否创建成功
	*/
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); ;
	if (client_socket == INVALID_SOCKET) {
		//如果创建的socket无效，则结束程序
		perror("socket error !");
		return 0;
	}

	/*
	创建地址结构，server_addr，并设置端口和IP
	*/
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	//要连接的服务器端口号 8888
	server_addr.sin_port = htons(8888);
	//指定服务器的地址127.0.0.1
	InetPton(AF_INET, "127.0.0.1",&server_addr.sin_addr.s_addr);
	
	//与地址server_addr建立连接
	if (connect(client_socket, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)))
	{
		perror("connect error !\n");
		return 0;
	}

	char REMOTE_file_name[FILE_NAME_MAX_SIZE + 1];
	memset(REMOTE_file_name, 0, FILE_NAME_MAX_SIZE + 1);
	printf("请输入要获取的服务器文件路径:\n");
	scanf_s("%s", REMOTE_file_name, FILE_NAME_MAX_SIZE);

	char LOCAL_file_name[FILE_NAME_MAX_SIZE + 1];
	memset(LOCAL_file_name, 0, FILE_NAME_MAX_SIZE + 1);
	printf("请输入保存文件的本地路径:\n");
	scanf_s("%s", LOCAL_file_name, FILE_NAME_MAX_SIZE);

	char buffer[BUFFER_SIZE];
	memset(buffer,0 , BUFFER_SIZE);
	strncpy_s(buffer, REMOTE_file_name, strlen(REMOTE_file_name)>BUFFER_SIZE ? BUFFER_SIZE : strlen(REMOTE_file_name));

	// 向服务器发送buffer中的数据 
	if (send(client_socket, buffer, BUFFER_SIZE, 0) < 0)
	{
		perror("发送文件名失败:");
		exit(1);
	}

	// 打开文件，准备写入 
	FILE *fp;
	errno_t F_ERR = fopen_s(&fp, LOCAL_file_name, "wb");
	if (F_ERR != 0)
	{
		printf("文件打开失败：%s\n", LOCAL_file_name);
		exit(1);
	}

	// 从服务器接收数据到buffer中 
	// 每接收一段数据，便将其写入文件中，循环直到文件接收完并写完为止 
	memset(buffer,0, BUFFER_SIZE);
	int length = 0;
	while ((length = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0)
	{
		if (fwrite(buffer, sizeof(char), length, fp) < length)
		{
			printf("文件写入失败：%s\n", LOCAL_file_name);
			break;
		}
		memset(buffer, 0, BUFFER_SIZE);
	}

	printf("\n成功从服务器接收文件\n存入本地目录：%s\n", REMOTE_file_name, LOCAL_file_name);


	// 接收成功后，关闭文件，关闭socket、WSA 
	fclose(fp);
	closesocket(client_socket);
	WSACleanup();
	system("pause");
	return 0;

}