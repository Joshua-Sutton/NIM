#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "nim.h"
#include <vector>
#include <string>
#pragma comment (lib, "iphlpapi.lib")
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::string;

int playNim(SOCKET s, char* serverName, sockaddr_in addr, int player) {
	cout << "PLAYING" << endl;
	return 0;
}