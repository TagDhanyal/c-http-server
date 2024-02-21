#include "../include/server.h"
#include <stdio.h>
#include <string.h>  
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8081 
#define MAX_RESPONSE_SIZE 1024
#define EXTERNAL_IP_SERVICE_URL "https://api64.ipify.org?format=text"

int retrieve_public_ip(char *ip_buffer, int buffer_size) {
    WSADATA wsaData;
    SOCKET sock;
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    char hostname[] = "api64.ipify.org";

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed\n");
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the IP address of the external IP service
    if (getaddrinfo(hostname, NULL, &hints, &result) != 0) {
        fprintf(stderr, "getaddrinfo failed\n");
        WSACleanup();
        return 1;
    }

    // Loop through all the resolved IP addresses and select the first one
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        struct sockaddr_in *sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
        if (inet_ntop(AF_INET, &(sockaddr_ipv4->sin_addr), ip_buffer, buffer_size) != NULL) {
            break; // Found the IP address, exit the loop
        }
    }

    freeaddrinfo(result); // Free the memory allocated for the result

    WSACleanup();
    return 0;
}

int main() {
    char public_ip[INET_ADDRSTRLEN]; // Buffer to store the public IP address
    if (retrieve_public_ip(public_ip, INET_ADDRSTRLEN) != 0) {
        fprintf(stderr, "Error occurred while retrieving public IP address\n");
        return 1;
    }

    printf("Public IP Address: %s\n", public_ip);

    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serverAddr;
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

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(public_ip); // Assign the obtained public IP address
    serverAddr.sin_port = htons(PORT); // Specify the port number

    // Bind the socket to the local address and port
    if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        fprintf(stderr, "bind failed with error = %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Socket bound successfully\n");

    return 0;
}