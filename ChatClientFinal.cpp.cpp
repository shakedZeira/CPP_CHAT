// TCP_Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// TCP_Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma once
#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <sdkddkver.h>
#include <winsock2.h>
#include <Windows.h>
#include <string>
#include <thread>

using namespace std;
#define SCK_VERSION2 0x202
#define PORT 54000

char* output_messages(SOCKET sock)
{
	bool is_sending = true;
	char massage_data[1042];
	while (is_sending)
	{
		cin >> massage_data;
		if (massage_data == 0)
		{
			cout << "couldnt recive data" << endl;
			is_sending = false;

		}
		else
		{
			send(sock, massage_data, 1042, 0);//fix
		}
	}
	return massage_data;
}
void recv_massages(SOCKET sock, char massage_data[1042])
{
	while (true)
	{
		recv(sock, massage_data, 1042, 0);
		cout << massage_data << endl;
	}
}

int main()
{
	//locals
	long succesful;
	WSADATA WinSockData;
	WORD DLLversion;
	char massage[1042];
	DLLversion = MAKEWORD(2, 1);
	succesful = WSAStartup(DLLversion, &WinSockData);
	//check requierd
	if (succesful != 0)
	{
		cout << "coudlnt open winsock" << endl;
	}
	string response;
	string converter;

	sockaddr_in address;
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	//check requierd
	if (sock == INVALID_SOCKET)
	{
		cout << "Couldnt create a socket" << endl;
	}

	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_family = AF_INET;
	address.sin_port = htons(54000);

	if (connect(sock, (SOCKADDR*)&address, sizeof(address)) == 0)
	{
		cout << "conecting..." << endl;
	}

	thread sending(output_messages, sock);
	//memset(massage, NULL, 1042);
	thread recving(recv_massages, sock, output_messages(sock));
	sending.join();
	recving.join();
	/*
	thread sending(output_messages, sock);
	memset(massage, NULL, 1042);
	thread recving(recv_massages, sock, massage);

	sending.join();
	recving.join();
	*/

	system("pause");
	return 0;
}
