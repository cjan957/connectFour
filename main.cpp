// TODO: The intention is to create a playable connect four game out of the student's implementation here.
#include "ConnectFour/Game.hpp"
#include "ConnectFour/SuperGame.hpp"
#include <iomanip>
#include <vector>
#include <iostream>

using namespace std;

Game *game;
Grid *board;
vector <Player*> players;

void printPlayer(vector<Player*> player){
	//print list of player
	cout << setfill('+') << setw(28) << "+" << endl;
	cout << "|" << setfill(' ') << setw(10) << "Player" << "     |   "<< "Score" << "  |" << endl;

	for (unsigned int i = 0; i < player.size();i++)
		cout << "|" << setw(10) << player[i]->getName() << "     |" << setw(5) << player[i]->getWins() << "     |" << endl;
	cout << setfill('+') << setw(28) << "+" << endl;

}
void printBoard(){
	cout << endl;
	cout << "******************************BOARD*********************************" << endl;
	int rows = board->rowCount();
	int columns = board->columnCount();
	for (int i = -1; i < (int) rows; i++)
	{
		for (int j = 0; j < columns + 1; j++){
			if (i == -1 || i == rows){
				if (j != columns)
					cout << "+ + ";
				else
					cout << "+" << endl;
				continue;
			}
			if (j != columns){
				cout << "| ";

				if (board->cellAt(i, j) == Grid::GC_PLAYER_ONE){
					cout << "O ";
				}
				else if (board->cellAt(i, j) == Grid::GC_PLAYER_TWO){
					cout << "X ";
				}
				else{
					cout << "  ";
				}
			}
			else{
				cout << "|" << endl;
			}
		}
		if (i >= 0 && i < rows - 1){
			for (int j = 0; j < columns + 1; j++){
				if (j != columns)
					cout << "|---";
				else
					cout << "|" << endl;
			}
		}
		else if (i == rows - 1){
			for (int j = 0; j < columns + 1; j++){
				if (j != columns)
					cout << "+ + ";
				else
					cout << "+" << endl;
			}
		}
	}
}
void startGame(){
	// check game status
	if (game->status() == Game::GS_INVALID){
		cout << "Please select game players" << endl;
		return;
	}
	if (game->status() == Game::GS_COMPLETE){
		cout << "Game is finished, please start new game" << endl;
		return;
	}
	//if game in progress, start it
	int column;
	while (game->status() == Game::GS_IN_PROGRESS){//until game is in progress
		const Player *P1 = game->nextPlayer();
		cout << "-1: Quit:" << endl; ///exit 
		cout << "Player " << P1->getName() << " :Please insert the column" << endl;
		cin >> column;
		cout << column << endl;
		if (column == -1)
			break;
		game->playNextTurn(column);
		//printBoard();
	}

	if (game->status() == Game::GS_COMPLETE) //if the game is finished
		cout << "The game is completed" << endl;
}
void printGameStatus()
{
	//check status of game
	if (game->status() == Game::GS_INVALID){
		cout << "The game hasn't started yet" << endl;
		return;
	}
	if (game->status() == Game::GS_COMPLETE){
		cout << "The game is completed" << endl;
		return;
	}
	cout << "The game is still in progress" << endl;

}
void setPlayer(string playerNumber){
	cout << "Please select the "<<playerNumber << endl;
	///print list of player
	for (unsigned int i = 0; i < players.size(); i++)
		cout << i + 1 << ") " << players[i]->getName() << endl;
	//get the selected player
	unsigned int number;
	cin >> number;
	cout << number << endl;
	if (number<1 || number>players.size()){
		cout << "Please select correct the number" << endl;
		return;
	}
	//add player to list
	if (playerNumber == "first player")
		game->setPlayerOne(players[number - 1]);
	else if(playerNumber == "second player")
		game->setPlayerTwo(players[number - 1]);
}
void playGame(vector <Player*> players){
	int option,row,column;
	while (1)
	{
		cout << endl << "******************************Menu*********************************" << endl;;

		cout << "Press 1: Enter the game board size:" << endl;
		cout << "Press 2: Select the first player:" << endl;
		cout << "Press 3: Select the second player:" << endl;
		cout << "Press 4: Start the game:" << endl;
		cout << "Press 5: Who is the winner?" << endl;
		cout << "Press 6: what is the game status:" << endl;
		cout << "Press 7: Restart game:" << endl;
		cout << "Press 8: Back to the menu" << endl;
		cin >> option;
		cout << option << endl;
		switch (option){
		case 1:
			//get board size
			cout << "Please enter the number of rows:" << endl;
			cin >> row;
			cout << "Please enter the number of columns:" << endl;
			cin >> column;
			
			board = new Grid(row, column);
			game->setGrid(board);
			break;
		case 2:
			setPlayer("first player");
			break;
		case 3:
			setPlayer("second player");
			break;
		case 4:
			startGame();
			break;
		case 5:
			// if the game is complete publish winner
			if (game->status() == Game::GS_COMPLETE)
				cout << "The winner is " << game->winner()->getName() << endl;
			else
				printGameStatus();
			break;
		case 6:
			printGameStatus();
			break;
		case 7:
			game->restart();
			break;
		case 8:
			return;
			break;
		default:
			cout << "The number is out of range" << endl;
			break;
		}
	}
}
void Insertname(){
	string name;
	//get name 
	cout << "Please Insert the player name:" << endl;
	cin >> name;
	Player *newPlayer = new Player(name);
	//insert name to player list
	players.push_back(newPlayer);
}
int main() {
	//FILE *stream;
	//freopen_s(&stream, "in.txt", "r", stdin);
	//add some initial player
	Player *chelsea = new Player("Chelsea");
	Player *bayern = new Player("Bayern");
	Player *barcelona = new Player("Barcelona");
	Player *madrid = new Player("Madrid");
	players.push_back(barcelona);
	players.push_back(bayern);
	players.push_back(chelsea);
	players.push_back(madrid);
	int option;
	while (1)
	{
		cout << "Press 1: Print current players and scores:" << endl;
		cout << "Press 2: Play connect four game:" << endl;
		cout << "Press 3: Insert Players name:" << endl;
		cout << "Press 4: Play super connect four game:" << endl;
		cout << "Press 5: Quit" << endl;
		cin >> option;
		cout << option << endl;
		switch (option){
			case 1:
				printPlayer(players);
				break;
			case 2:
				game = new Game();
				playGame(players);
				break;
			case 3:
				Insertname();
				break;
			case 4:
				game = new SuperGame();
				playGame(players);
				break;
			case 5:
				return 0;
				break;
			default:
				cout << "The number is out of range" << endl;
				break;
		}
	}
    return 0;
}