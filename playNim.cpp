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

//SERVER SIDE

int playNim(SOCKET s, char* serverName, sockaddr_in addr, int player) {

    int numPiles;
    int numRocks;
    int addrSize = sizeof(addr);
    char setNumPiles[10];
    char setNumRocks[20];

    if (player == HOST) {
        cout << "Enter the number of piles:";
        cin >> numPiles;
        _itoa_s(numPiles, setNumPiles, sizeof(setNumPiles), 10);
    }

    vector<int> pile_sizes(numPiles);
    for (int i = 0; i < numPiles; ++i) {
        cout << "Enter the number of rocks for pile " << i + 1 << ": ";
        cin >> pile_sizes[i];
        _itoa_s(pile_sizes[i], setNumRocks, sizeof(setNumRocks), 10);

        //pile_sizes[i] = setNumRocks;
    }

    // Start the game
    int piles[MAX_PILES];
    numRocks = atoi(setNumRocks);
    for (int i = 0; i < numPiles; i++) {
        piles[i] = numRocks;
    }
    player = 1;
    //while (true) {
        // Display the current state of the game
    cout << "Current State:" << endl;
    for (int i = 0; i < numPiles; i++) {
        cout << "Pile " << i + 1 << ": ";
        for (int j = 0; j < pile_sizes[i]; j++) {
            cout << "*";
        }
        cout << endl;
    }
    cout << endl;

    //}

    char sendbuf[DEFAULT_BUFLEN];

    strcpy_s(sendbuf, setNumPiles);
    strcat_s(sendbuf, setNumRocks);


    //Now I need to send these parameters to the client
    // sendto(s, ?, (int)strlen(?) + 1, 0, (sockaddr*)&addr, addrSize);
    sendto(s, sendbuf, (int)strlen(sendbuf) + 1, 0, (sockaddr*)&addr, addrSize);



    //Get the player's move
    cout << "Player " << player << "'s turn. Enter move (pile RockCount): ";
    int pileIndex, rocksToRemove;
    cin >> pileIndex >> rocksToRemove;


    // Update the game state
    pile_sizes[pileIndex - 1] -= rocksToRemove;

    // Check for a winner
    bool isGameOver = true;
    for (int i = 0; i < numPiles; i++) {
        if (pile_sizes[i] > 0) {
            isGameOver = false;
            break;
        }
    }
    if (isGameOver) {
        cout << "Player " << player << " wins!" << endl;
        //break;
    }

    // Switch to the other player
    player = (player == 1) ? 2 : 1;

    // Now I need to send the move to the other player
    // sendto(s, ?, (int)strlen(?) + 1, 0, (sockaddr*)&addr, addrSize);
    char move[256];
    sprintf_s(move, "%d %d", pileIndex, rocksToRemove);
    sendto(s, move, (int)strlen(move)+1, 0, (sockaddr*)&addr, addrSize);
    return 0;


}

//playGame(s);
        
        //CLIENT SIDE
        
        void playGame(SOCKET s, char* serverName, sockaddr_in addr, int player) {

            int addrSize = sizeof(addr);

            //It needs to receive the parameters from the server
            char gameParams[DEFAULT_BUFLEN];
            recv(s, gameParams, sizeof(gameParams), 0);
            int numPiles;//  stonesPerPile
            sscanf_s(gameParams, "%d", &numPiles);
            vector<int> pile_sizes(numPiles);
            sscanf_s(gameParams, "%d", &pile_sizes);
            //sscanf_s(gameParams, "%d %d", &numPiles, &pile_sizes);
            
            // Start the game
            player = 2;
            while (true) {
                // Receive the current state of the game from the server
                char stateBuf[1024];
                recvfrom(s, stateBuf, (int)strlen(stateBuf) + 1, 0, (sockaddr*)&addr, &addrSize);
                //recv(s, stateBuf, sizeof(stateBuf), 0);
                //stateBuf[numPiles * pile_sizes] = '\0';
                cout << "Current State:" << endl;
                for (int i = 0; i < numPiles; i++) {
                    cout << "Pile " << i + 1 << ": ";
                    for (int j = 0; j < pile_sizes[i]; j++) {
                        if (stateBuf[i * pile_sizes[i] + j] == '*') {
                            cout << "*";
                        }
         else {
          cout << " ";
        }
        }
        cout << endl;
        }
        cout << endl;
        
        // Check for a winner
        bool isGameOver = true;
        for (int i = 0; i < numPiles; i++) {
            for (int j = 0; j < pile_sizes[i]; j++) {
                if (stateBuf[i * pile_sizes[i] + j] == '*') {
                    isGameOver = false;
                    break;
                }
            }
            if (!isGameOver) {
                break;
            }
        }
        if (isGameOver) {
            cout << "Player " << player << " wins!" << endl;
            break;
        }
        
        // Get the player's move
        cout << "Player " << player << "'s turn. Enter move (pile, RockCount): ";
        int pileIndex, rocksToRemove;
        cin >> pileIndex >> rocksToRemove;
        
        // Send the move to the server
        char moveBuf[4];
        moveBuf[0]= pileIndex + '0';
        moveBuf[1] = ' ';
        moveBuf[2] = rocksToRemove + '0';
        moveBuf[3] = '\0';
        send(s, moveBuf, sizeof(moveBuf), 0);
        
        // Switch players
        player = 3 - player;
            }
        }
            
    
