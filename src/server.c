#include "../include/server.h"

#include <stdio.h>
#include <string.h>  
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
  
  WSADATA wsaData;
  SOCKET sock = INVALID_SOCKET;
  struct sockaddr_in serverAddr;
  int iFamily = AF_INET;
  int iType = 1;
  int iProtocol = 0;

  printf("\nInitialising Winsock...");
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    fprintf(stderr, "WSAStartup failed\n");
    return 1;
  }
  printf("\nInitialised....");

  sock = socket(iFamily, iType, iProtocol);
    if (sock == INVALID_SOCKET) 
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
        WSACleanup();
    else {
        wprintf(L"socket function succeeded\n");
}
return 0;
}

//WSACleanup();