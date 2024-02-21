#include "../include/server.h"
#include <stdio.h>
#include <string.h>  
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

//#pragma comment(lib, "ws2_32.lib")

#define PORT 8081 
#define MAX_RESPONSE_SIZE 1024

struct http_request
{
    short http_code;
};


int main() {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    char* message;
    struct sockaddr_in globalServerAddr; // Global declaration
    int iFamily = AF_INET;
    int iType = SOCK_STREAM; // Use SOCK_STREAM for TCP
    int iProtocol = IPPROTO_TCP;

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed\n");
        return 1;
    }
    printf("\nInitialised....");

    sock = socket(iFamily, iType, iProtocol);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "socket function failed with error = %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    globalServerAddr.sin_family = AF_INET;
    globalServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Use localhost IP address
    globalServerAddr.sin_port = htons(PORT); // Specify the port number

    // Bind the socket to the local address and port
    if (bind(sock, (struct sockaddr *)&globalServerAddr, sizeof(globalServerAddr)) == SOCKET_ERROR) {
        fprintf(stderr, "bind failed with error = %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Socket bound successfully\n");

    // Listen for incoming connections
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        fprintf(stderr, "listen failed with error = %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Listening for incoming connections...\n");

    // Accept incoming connections
    SOCKET clientSocket;
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    while (clientSocket = accept(sock, (struct sockaddr *)&clientAddr, &clientAddrLen))
    {
        puts("Connection accepted");

		//Reply to the client
		message = "200 ok\n";
		send(clientSocket , message , strlen(message) , 0);
    }
    if (clientSocket == INVALID_SOCKET) {
        fprintf(stderr, "accept failed with error = %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Connection accepted from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    // Close the listening socket
    closesocket(sock);
    WSACleanup();

    return 0;
}