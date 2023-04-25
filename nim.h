#pragma once
#include<vector>
#include<string>
#include<iostream>
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
const int noWinner = 0;
const int hostWinner = 1;
const int clientWinner = 2;
const int tie = 3;
const int ABORT = -1;

using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::string;

struct ServerStruct {
	char name[MAX_NAME];
	sockaddr_in addr;
	
};

struct Move {
	string moveString;
	int pileIndex;
	int rocksToRemove;
};

class GameBoard {
	vector<int> board;
	int piles;
public:
	GameBoard() : piles{ 0 } {};
	void setBoard(string pile_size) {
		this->piles = pile_size[0] - '0';
		for (int c = 1; c <= sizeof(pile_size) - 2; c += 2) {
			board.push_back((pile_size[c] - '0') * 10 + (pile_size[c + 1] - '0'));
		}
	}

	vector<int> setBoard() {
		return this->board;
	}

	void updateBoard(int pile, int amountToRemove) {
		if (this->board[pile - 1] != 0)
			this->board[pile - 1] -= amountToRemove;
		else std::cout << "Cannot remove from empty pile" << std::endl;
	}

	void printBoard() {
		int row = 1;
		std::cout << std::endl;
		for (const auto& pile : this->board) {
			std::cout << "Row " << row++ << ": ";

			for (int rock = 0; rock < pile; ++rock) {
				std::cout << " * ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}


	int getPiles() {
		return this->piles;
	}

	Move getMove(const int player) {
		int move;
		string inputOne;
		string move_str;
		string comment = "";
		int pile;
		int rocksToRemove;
		char newline;
		Move returnMove;
		inputOne = "invalid";
		while (inputOne == "invalid") {
			std::cout << "Please input a number 1-" << this->getPiles() << " to select a pile to remove from, The letter 'C' to send a comment, or the letter 'F' to forfeit" << std::endl;
			std::cin >> inputOne;
			std::cin.get(newline);

			//Handle a Comment
			if (inputOne[0] == 'C' || inputOne[0] == 'c') {
				std::cout << "Please type the comment you would like to send. ";
				std::getline(std::cin, comment);
				while (comment.length() > 80) {
					std::cout << "Your comment was too long, please send something shorter. ";
					std::getline(std::cin, comment);
				}
				returnMove.moveString = 'C' + comment;
				return returnMove;
			}

			//Handle a Forfiet
			else if (inputOne[0] == 'F' || inputOne[0] == 'f') {
				returnMove.moveString = "F";
				return returnMove;
			}

			//Get a number of Piles
			else if ((inputOne[0] - '0') >= 1 && (inputOne[0] - '0') <= this->getPiles()) {
				std::string rocks = "";
				char newline;
				pile = inputOne[0] - '0';
				if (this->setBoard()[pile - 1] <= 0) {
					std::cout << "This pile has no rocks! Please chose a different pile" << std::endl;
					inputOne = "invalid";
				}
				else {
					std::cout << "Now enter how many rocks to remove ";
					std::cin >> rocks;
					if (rocks[1] == NULL) {
						rocks = '0' + rocks;
					};
					std::cin.get(newline);
					rocksToRemove = (rocks[0] - '0') * 10 + (rocks[1] - '0');
					while (this->setBoard()[pile - 1] < rocksToRemove || rocksToRemove <= 0) {
						std::cout << "Invalid number of rocks. Please enter a number between 1 and " << this->setBoard()[pile - 1] << std::endl;
						std::cin >> rocks;
						std::cin.get(newline);
						if (rocks[1] == NULL) {
							rocks = '0' + rocks;
						};
						rocksToRemove = (rocks[0] - '0') * 10 + (rocks[1] - '0');
						std::cout << rocksToRemove;
					}
					inputOne += rocks;
					return Move{ inputOne, pile, rocksToRemove };
				}
			}
			else {
				std::cout << "Your input was invalid. Please try again, input a number 1-" << this->getPiles() << std::endl;
				inputOne = "invalid";
			}
		}
	}

	void updateBoard(std::string move) {
		int pile = (move[0] - '0');
		int amountToRemove = (move[1] - '0') * 10 + (move[2] - '0');
		this->updateBoard(pile, amountToRemove);
	}

	//This function needs to be ran when we get there move to know if they won and after our move to know if we won
	int check4win(int player, int oponent, int myMove) {
		//std::cout << "checking for win" << std::endl;
		/* Need to work here */
		int total = 0;
		for (std::size_t i = 0; i < this->board.size(); i++) {
			total = total + this->board[i];
		}

		if (total == 0) {
			//std::cout << "We have a winner!!!" << std::endl;
			if (myMove) {
				return player;
			}
			else {
				return oponent;
			}
		}

		return noWinner;
	}
};


int wait(SOCKET s, int seconds, int msec);
sockaddr_in GetBroadcastAddress(char* IPAddress, char* subnetMask);
void getServers(SOCKET s, sockaddr_in broadcastAddress, ServerStruct server[], int& numServers);
int client_main(char* playerName, char* IPAddress, char* subnetMask);
int server_main(char* playerName);
int playNim(SOCKET s, char* serverName, sockaddr_in addr, int player);
