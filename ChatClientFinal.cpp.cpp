// TCP_Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
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

void output_messages(SOCKET sock)
{
	char massage_data[1042];
	while (true)
	{
		cin >> massage_data;
		if (massage_data == 0)
		{
			cout << "couldnt recive data" << endl;
		}
		else
		{
			send(sock, massage_data, 1042,0);//fix
		}
	}
}
void recv_massages(SOCKET sock , char massage_data[1042])
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
    sock = socket(AF_INET, SOCK_STREAM,0);
    //check requierd
	if (sock == INVALID_SOCKET)
	{
		cout << "Couldnt create a socket" << endl;
	}

    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(54000);

    cout << "do you want to connect to the server? (y/n)" << endl;
    cin >> response;
    if (response == "n")
    {
        cout << "bye bye" << endl;

    }
    if (response == "y")
    {
        connect(sock, (SOCKADDR*)&address, sizeof(address));
        succesful = recv(sock, massage, sizeof(massage), NULL);
        converter = massage;
        cout << "massage from the server: \n\n\t" << converter << endl;

    }
    else
    {
        cout << "invalid response!" << endl;
    }
	thread sending(output_messages , sock);
	memset(massage, NULL, 1042);
	thread recving(recv_massages, sock, massage);

	sending.join();
	recving.join();


    system("pause");
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

*/

/*
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
	string ipAddress = "127.0.0.1";			// IP Address of the server
	int port = 54000;						// Listening port # on the server

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server!"<< endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// Do-while loop to send and receive data
	char buf[4096];
	string userInput;

	do
	{
		// Prompt the user for some text
		cout << "> ";
		getline(cin, userInput);

		if (userInput.size() > 0)		// Make sure the user has typed in something
		{
			// Send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
		}

	} while (userInput.size() > 0);

	//close down everything
	
	closesocket(sock);
	WSACleanup();
	return 0;
}

*/

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