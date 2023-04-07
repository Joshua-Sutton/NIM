#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "nim.h"
#pragma comment (lib, "iphlpapi.lib")
#pragma comment(lib, "Ws2_32.lib")
using std::cout;
using std::cin;
using std::endl;

int main(int argc, char* argv[]) {
	WSAData wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		cout << "WSAStartup failed: " << iResult << '\n';
		return 1;
	}

	char playerName[MAX_NAME], choice[2], IPAddress[20] = "", subnetMask[20] ="";

	cout << "What is your name? ";
	cin.getline(playerName, MAX_NAME);

	do {
		cout << endl;
		cout << "Choose an option:" << endl;
		cout << " 1 - Host a game of Tic-Tac-Toe" << endl;
		cout << " 2 - Challenge someone else" << endl;
		cout << " 3 - Quit" << endl;
		cout << "Enter 1, 2 or 3: ";
		cin.getline(choice, 2);
		switch (choice[0]) {
		case '1': server_main(playerName); break;
		case '2': client_main(playerName, IPAddress, subnetMask); break;
		case '3': break;
		default: cout << endl << "Please enter a digit between 1 and 3." <<
			endl;
		}
	} while (choice[0] != '3');
	WSACleanup();
	return 0;
}

int server_main(char* playerName) {
	return 0;
}

int client_main(char* playerName, char* IPAddress, char* subnetMask) {
	return 0;;
}


