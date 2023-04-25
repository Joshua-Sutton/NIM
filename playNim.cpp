//#include <iostream>
//#include <WinSock2.h>
//#include <ws2tcpip.h>
//#include <iphlpapi.h>
//#include "nim.h"
//#include <vector>
//#include <string>
//#pragma comment (lib, "iphlpapi.lib")
//#pragma comment(lib, "Ws2_32.lib")
//#define DEFAULT_BUFLEN 512
//using std::vector;
//using std::cout;
//using std::cin;
//using std::endl;
//using std::string;
//
//int playNim(int playNim(SOCKET s, char* serverName, sockaddr_in addr, int player))
//{
//	int winner = noWinner;;
//	int opponent;
//	GameBoard board;
//	Move move;
//	bool playerMove;
//	bool boardIsSet;
//	string boardParams = "";
//	char newline
//
//}
















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

   	int winner = noWinner;;
    int opponent;
    GameBoard board;
    Move move;
    bool playerMove;
    bool boardIsSet;
    string boardParams = "";
    char newline;

    if (player == HOST) {
        cout << "Enter the number of piles (3-9) you want to play with:";
        cin >> numPiles;
        _itoa_s(numPiles, setNumPiles, sizeof(setNumPiles), 10);

        boardParams += numPiles;

        vector<int> pile_sizes(numPiles);
        for (int i = 0; i < numPiles; ++i) {
            cout << "Enter the number of rocks for pile " << i + 1 << ": ";
            cin >> pile_sizes[i];
            _itoa_s(pile_sizes[i], setNumRocks, sizeof(setNumRocks), 10);

            boardParams += pile_sizes[i];
        }

        cin.get(newline);
        board.setBoard(boardParams);
        opponent = CHALLENGER;
        playerMove = false;
        boardIsSet = true;
        board.printBoard();

        char sendbuf[DEFAULT_BUFLEN];

       strcpy_s(sendbuf, setNumPiles);
       strcat_s(sendbuf, setNumRocks);

       sendto(s, sendbuf, (int)strlen(sendbuf) + 1, 0, (sockaddr*)&addr, addrSize);

       


        //// Start the game
        //int piles[MAX_PILES];
        //numRocks = atoi(setNumRocks);
        //for (int i = 0; i < numPiles; i++) {
        //    piles[i] = numRocks;
        //}
        //player = 1;
        ////while (true) {
        //    // Display the current state of the game
        //    cout << "Current State:" << endl;
        //    for (int i = 0; i < numPiles; i++) {
        //        cout << "Pile " << i + 1 << ": ";
        //        for (int j = 0; j < pile_sizes[i]; j++) {
        //            cout << "*";
        //        }
        //        cout << endl;
        //    }
        //    cout << endl;

        

        //char sendbuf[DEFAULT_BUFLEN];

        //strcpy_s(sendbuf, setNumPiles);
        //strcat_s(sendbuf, setNumRocks);


        ////Now I need to send these parameters to the client
        //// sendto(s, ?, (int)strlen(?) + 1, 0, (sockaddr*)&addr, addrSize);
        //sendto(s, sendbuf, (int)strlen(sendbuf) + 1, 0, (sockaddr*)&addr, addrSize);



        //Get the player's move
       // cout << "Player " << player << "'s turn. Enter move (pile,RockCount): ";
       // int pileIndex, rocksToRemove;


       // cin >> pileIndex >> rocksToRemove;


       // // Update the game state
       // pile_sizes[pileIndex - 1] -= rocksToRemove;

       // // Check for a winner
       // bool isGameOver = true;
       // for (int i = 0; i < numPiles; i++) {
       //     if (pile_sizes[i] > 0) {
       //         isGameOver = false;
       //         break;
       //     }
       // }
       // if (isGameOver) {
       //     cout << "Player " << player << " wins!" << endl;
       //     //break;
       // }

       // // Now I need to send the move to the other player
       //// sendto(s, ?, (int)strlen(?) + 1, 0, (sockaddr*)&addr, addrSize);

       // char move[DEFAULT_BUFLEN];
       // char setPileIndex[sizeof(numPiles)];
       // char setRocksToRemove[sizeof(pile_sizes)];

       // _itoa_s(pileIndex, setPileIndex, sizeof(setPileIndex), 10);
       // _itoa_s(rocksToRemove, setRocksToRemove, sizeof(setRocksToRemove), 10);

       // strcpy_s(move, setPileIndex);
       // strcat_s(move, setRocksToRemove);
       // sprintf_s(move, "%d %d", pileIndex, rocksToRemove);
       // sendto(s, move, (int)strlen(move) + 1, 0, (sockaddr*)&addr, addrSize);
       // return 0;

       // // Switch to the other player
       // player = (player == 1) ? 2 : 1;


    }
    else {

        //CLIENT SIDE

        opponent = HOST;
        boardIsSet = false;
        playerMove = true;
    }

    while (winner==noWinner)
    {
        if (boardIsSet) {
            if (playerMove) {
                move = board.getMove(player);
                while (move.moveString[0] == 'C') {
                    sendto(s, (char*)move.moveString.c_str(), strlen(move.moveString.c_str()) + 1, 0, (sockaddr*)&addr, addrSize);
                    move = board.getMove(player);
                }
                if (move.moveString[0] == 'F') {
                    sendto(s, (char*)move.moveString.c_str(), strlen(move.moveString.c_str()) + 1, 0, (sockaddr*)&addr, addrSize);
                    winner = opponent;
                    std::cout << "You forfeited!!" << std::endl;
                }
                else {
                    std::cout << "Board after your move:" << std::endl;
                    board.updateBoard(move.pileIndex, move.rocksToRemove);
                    board.printBoard();
                    sendto(s, (char*)move.moveString.c_str(), strlen(move.moveString.c_str()) + 1, 0, (sockaddr*)&addr, addrSize);
                }
            }
            else {
                cout << "Waiting for your opponent..." << endl << endl;
                //Get opponent's move & display board
                int status = wait(s, 120, 0);
                if (status > 0) {
                    char movecstr[80];
                    
                    recvfrom(s, movecstr, (int)strlen(movecstr) + 1, 0, (sockaddr*)&addr, &addrSize);
                    
                    if (movecstr[0] == 'C') {
                        std::cout << "Comment from your opponent: ";

                        //declare iterator to point at the beginning of the array
                        const char* it = std::begin(movecstr);
                        //skip first character
                        ++it;

                        //iterate while (*it) is truthy
                        for (; *it; ++it) std::cout << *it;
                        std::cout << std::endl;

                        //not my turn yet, as it was just a comment what we received.
                        playerMove = !playerMove;
                    }
                    else if (movecstr[0] == 'F' || movecstr[0] == 'f') {
                        std::cout << "Your opponent forfeited!" << std::endl;
                        winner = player;
                    }
                    else {
                        board.updateBoard(movecstr);
                        std::cout << "Current state of the game:" << std::endl;
                        board.printBoard();
                    }
                }
                else {
                    winner = ABORT;
                }
            }
        }
        else {
            cout << "Waiting for board parameters..." << std::endl << std::endl;
            int status = wait(s, 120, 0);
            if (status > 0) {
                playerMove = false;
                boardIsSet = true;
                char boardConf[80];
                recvfrom(s, boardConf, (int)strlen(boardConf) + 1, 0, (sockaddr*)&addr, &addrSize);
              
                board.setBoard(boardConf);
                board.printBoard();
            }
            else {
                winner = ABORT;
            }
        }

        if (winner == ABORT) {
            std::cout << "No response from opponent.  Aborting the game..." <<endl;
        }
        else {
            if (winner == noWinner) winner = board.check4win(player, opponent, playerMove);
        }

        if (winner == player)
            std::cout << "You WIN!" << std::endl;
        else if (winner == opponent)
            std::cout << "I'm sorry.  You lost" << std::endl;


        playerMove = !playerMove;

    }

    return winner;
            //It needs to receive the parameters from the server
            //char gameParams[DEFAULT_BUFLEN];
            //recv(s, gameParams, sizeof(gameParams), 0);
            //int numPiles;//  stonesPerPile
            //sscanf_s(gameParams, "%d", &numPiles);
            //vector<int> pile_sizes(numPiles);
            //sscanf_s(gameParams, "%d", &pile_sizes);
            ////sscanf_s(gameParams, "%d %d", &numPiles, &pile_sizes);

            //// Start the game
            //player = 2;
            //while (true) {
            //    // Receive the current state of the game from the server
            //    char stateBuf[1024];
            //    recvfrom(s, stateBuf, (int)strlen(stateBuf) + 1, 0, (sockaddr*)&addr, &addrSize);
            //    //recv(s, stateBuf, sizeof(stateBuf), 0);
            //    //stateBuf[numPiles * pile_sizes] = '\0';
            //    cout << "Current State:" << endl;
            //    for (int i = 0; i < numPiles; i++) {
            //        cout << "Pile " << i + 1 << ": ";
            //        for (int j = 0; j < pile_sizes[i]; j++) {
            //            if (stateBuf[i * pile_sizes[i] + j] == '*') {
            //                cout << "*";
            //            }
            //            else {
            //                cout << " ";
            //            }
            //        }
            //        cout << endl;
            //    }
            //    cout << endl;

            //    // Check for a winner
            //    bool isGameOver = true;
            //    for (int i = 0; i < numPiles; i++) {
            //        for (int j = 0; j < pile_sizes[i]; j++) {
            //            if (stateBuf[i * pile_sizes[i] + j] == '*') {
            //                isGameOver = false;
            //                break;
            //            }
            //        }
            //        if (!isGameOver) {
            //            break;
            //        }
            //    }
            //    if (isGameOver) {
            //        cout << "Player " << player << " wins!" << endl;
            //        break;
            //    }

            //    // Get the player's move
            //    cout << "Player " << player << "'s turn. Enter move (pile, RockCount): ";
            //    int pileIndex, rocksToRemove;
            //    cin >> pileIndex >> rocksToRemove;

            //    // Send the move to the server
            //    char moveBuf[4];
            //    moveBuf[0] = pileIndex + '0';
            //    moveBuf[1] = ' ';
            //    moveBuf[2] = rocksToRemove + '0';
            //    moveBuf[3] = '\0';
            //    send(s, moveBuf, sizeof(moveBuf), 0);

            //    // Switch players
            //    player = 3 - player;
            //}
        
    }
    



    



        
        
            
    
