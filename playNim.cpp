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

int ValidateData(GameBoard board, int rocksToRemove, int pileNum, int player) {
    int winner = noWinner;

    if (board.setBoard()[pileNum] < rocksToRemove) {
        winner = player;
    }
    if (board.setBoard()[pileNum] < 1) {
        winner = player;
    }

    return winner;
}

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
    char sendbuf[DEFAULT_BUFLEN];

    if (player == HOST) {
        cout << "Enter the number of piles (3-9) you want to play with:";
        cin >> numPiles;
        _itoa_s(numPiles, setNumPiles, sizeof(setNumPiles), 10);

        boardParams += setNumPiles;

        vector<int> pile_sizes(numPiles);
        for (int i = 0; i < numPiles; ++i) {
            cout << "Enter the number of rocks for pile " << i + 1 << ": ";
            cin >> pile_sizes[i];
            _itoa_s(pile_sizes[i], setNumRocks, sizeof(setNumRocks), 10);
            

            boardParams += setNumRocks;
        }

        cin.get(newline);
        board.setBoard(boardParams);
        opponent = CHALLENGER;
        playerMove = false;
        boardIsSet = true;
        board.printBoard();


        //strcpy_s(sendbuf, setNumPiles);
        //strcat_s(sendbuf, setNumRocks);

        //sendto(s, sendbuf, (int)strlen(sendbuf) + 1, 0, (sockaddr*)&addr, addrSize);
        sendto(s, (char*)boardParams.c_str(), strlen(boardParams.c_str()) + 1, 0, (sockaddr*)&addr, addrSize);


    }
    else {

        //CLIENT SIDE

        opponent = HOST;
        boardIsSet = false;
        playerMove = true;
    }

    while (winner == noWinner)
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
            std::cout << "No response from opponent.  Aborting the game..." << endl;
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

}
    
    



    



        
        
            
    
