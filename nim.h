#pragma once
#define MAX_NAME 80
#define MAX_PILES 9
#define MIN_PILES 3
#define MAX_ROCKS 20
#define MIN_PILES 1
#define DEFAULT_PORT 29333
#define DEFAULT_BUFLEN 512
#define MAX_SERVERS 100
#define Nim_QUERY "Who?"
#define Nim_NAME "Name="
#define Nim_CHALLENGE "Player="
const int CHALLENGER = 1;
const int HOST = 2;
struct ServerStruct {
	char name[MAX_NAME];
	sockaddr_in addr;
};
int wait(SOCKET s, int seconds, int msec);
sockaddr_in GetBroadcastAddress(char* IPAddress, char* subnetMask);
void getServers(SOCKET s, sockaddr_in broadcastAddress, ServerStruct server[], int& numServers);
int client_main(char* playerName, char* IPAddress, char* subnetMask);
int server_main(char* playerName);
int playNim(SOCKET s, char* serverName, sockaddr_in addr, int player);
