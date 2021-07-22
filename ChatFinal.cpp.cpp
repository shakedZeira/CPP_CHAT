//multi client chat with select



#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include <mutex>

#define maxsockets 6
#define PORT 54000

#pragma comment (lib,"ws2_32.lib")
using namespace std;
void broadcastMassage(SOCKET client_Socket, char* data, vector<SOCKET> socketList)
{
    
    for (int i = 0; i < socketList.size(); i++)
    {
        if (socketList[i] != client_Socket)
        {
            ostringstream ss;
            ss << "SOCKET #" << socketList[i] << ": " << data << "\r\n";
            string strOut = ss.str();
            send(socketList[i], strOut.c_str(), strOut.size() + 1, 0);
        }
    }
}

void recive_messages(SOCKET client_Socket , vector<SOCKET> socketList)
{
    char data[4096];
    memset(data, NULL, 4096);

    while (true)
    {
        if (recv(client_Socket, data, 4096, 0) <= 0)
        {
            cout << "couldnt recive message!" << endl;
        }
        else
        {
            cout << data << endl;
        }
        broadcastMassage(client_Socket, data ,socketList);
        memset(data, NULL, 1024);
    }
}

int main()
{
    //creating a sockets vector
    vector<SOCKET> SocketList;
    //creating a Threads vector
    vector<thread> ThreadList;
    //initalize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0)
    {
        cout << "Cant initlize winsock" << endl;
    }
    else
    {
        cout << "WinSock Initilized" << endl;
    }

    //create a socket 
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    SocketList.push_back(listening);
    if (listening == INVALID_SOCKET)
    {
        cout << "cant create a socket" << endl;
    }
    else
    {
        cout << "created a socket" << endl;
    }

    //bind the ip address and port to the socket
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening, (sockaddr*)&address, sizeof(address));

    //tell winsock the socket is for listening
    listen(listening, SOMAXCONN);
    cout << "shhh!!!! , the server is listening" << endl;

    while (true)
    {
       
        int socketCount = SocketList.size();


        for (int i = 0; i < socketCount; i++)
        {
            SOCKET sock = SocketList[i];

            if (sock == listening)
            {
                //accept a new connection
                int clientSize = sizeof(address);
                SOCKET client = accept(listening, (sockaddr*)&address, &clientSize);
                //cout << inet_ntoa(address.sin_addr) << " ," << ntohs(address.sin_port) << endl;
                //add the new conncetion to the list of connected clients
                SocketList.push_back(client);
                ThreadList.push_back(thread(recive_messages, client, SocketList));
                //send a welcome a massage to the client 
                string welcome_massage = "WELCOME TO THE CHAT \r\n";
                send(client, welcome_massage.c_str(), welcome_massage.size() + 1, 0);
                //broadcast we have a new connection
                //string broadcast_massage = "Connected to the chat";

            }
            else
            {
                /*
                char buffer[4096];
                ZeroMemory(buffer, 4096);
                int bytesIN = recv(sock, buffer, 4096, 0);
                if (bytesIN <= 0)
                {
                    //drop the client
                    closesocket(sock);
                    SocketList.pop_back();
                }
                */
                recive_messages(SocketList[i],SocketList);
                /*
                else
                {
                    
                    //send massage to other clients
                    for (int i = 0; i < SocketList.size(); i++)
                    {
                        SOCKET outsock = SocketList[i];


                        if (outsock != listening && outsock != sock)
                        {
                            ostringstream ss;
                            ss << "SOCKET #" << sock << ": " << buffer << "\r\n";
                            string strOut = ss.str();
                            send(outsock, strOut.c_str(), strOut.size() + 1, 0);
                        }
                    }
                    
                }
                */

            }
        }
    }

    //cleanup winsock
    WSACleanup();

    return 0;

}


/*
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "server.hpp"
#include <mutex>

using namespace std;

mutex m;

Server::Server(int port)
{
    this->port = port;

    //init winsock, WSA variables - environment supporting socket programming on windows.
    WORD version = MAKEWORD(2, 2); //version 2.2 or 0x0202
    int validws = WSAStartup(version, &wsData);
    if (validws != 0) //check if works
    {
        cerr << "Couldn't initialize winsock" << endl;
        exit(1);
    }

    //create socket - SOCKET is a number, a file handler, means that the writing is going to be into that file num
    connection_s = socket(AF_INET, SOCK_STREAM, 0);
    listening_s = socket(AF_INET, SOCK_STREAM, 0); //0 can be also replaced with IPPROTO_TCP
    // Bind an IP address and a port to the socket.  
    addr.sin_family = AF_INET; // version 4
    addr.sin_port = htons(port); //host to network short - network uses big endian, so it converts.
    addr.sin_addr.S_un.S_addr = ip_addr; //any ip - INADDR_ANY, or inet_addr(*address*)

    if (listening_s == INVALID_SOCKET)
    {
        cerr << "couldn't create a socket." << endl;
        exit(1);
    }
    if (bind(listening_s, (sockaddr*)&addr, sizeof(addr)) < 0)
    {
        cerr << "Error binding socket." << endl;
        exit(1);
    }
    //tell winsock the socket is for listening
    listen(listening_s, SOMAXCONN); // the socket is for listening, being able to listen to maximum amount.
    cout << "Listening to incoming connections." << endl;
}
void Server::accept_con()
{
    sockaddr_in client_addr;
    int clientSize;

    while (true)
    {
        clientSize = sizeof(client_addr);
        connection_s = accept(listening_s, (sockaddr*)&client_addr, &clientSize); //puts the client info into addr.
        cout << "Connection from " << "(" << inet_ntoa(client_addr.sin_addr) << " , " << ntohs(client_addr.sin_port) << ")" << endl;

        add_client(connection_s);

        if (connection_s == INVALID_SOCKET)
            break;

        threads.emplace_back(thread(&Server::recieve_messages, this, connection_s)); //and the object itself, the third is the parameters but
                                                                              //there are none.
        threads.back().detach();
    }
}
void Server::add_client(SOCKET client)
{
    clients.emplace_back(client); //emplace back
}
void Server::recieve_messages(SOCKET client)
{
    char local_data[BUFF_SIZE];
    ZeroMemory(local_data, BUFF_SIZE);

    while (true)
    {
        if (recv(client, local_data, BUFF_SIZE, 0) <= 0)
            break;
        cout << "message recieved: " << local_data << endl;
        if (strstr(local_data, "/exit"))
            break;
        broadcast_message(client, local_data);
        ZeroMemory(local_data, BUFF_SIZE);
    }
}
Server::~Server()
{
    close();
}
void Server::broadcast_message(SOCKET& client, char* data)
{
    std::lock_guard<mutex> lock_guard(m);
    for (auto c : clients)
    {
        if (c != client)
            send(c, data, BUFF_SIZE, 0); //instead of clients[i], use clients.at()
    }
}
void KeyListener(Server& s)
{
    const int ESC = 27;
    while (true)
    {
        if (_kbhit())
        {
            if (_getch() == ESC)
            {
                s.~Server();
                break;
            }
        }

    }
}
void Server::close()
{
    closesocket(listening_s);
    closesocket(connection_s);
    WSACleanup();
}

*/











