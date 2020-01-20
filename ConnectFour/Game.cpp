#include "Game.hpp"
#include "Grid.hpp"
#include <iostream>
using namespace std;

// TODO: implement Game class here
Game::Game() {

	this->gameGrid = 0;
	this->player1 = 0;
	this->player2 = 0;

	this->currentPlayer = 1;
	this->playerWon = 0;
	this->gameStatus = 2;
	this->gridSet = false;
	this->player1Set = false;
	this->player2Set = false;
}

Game::~Game() {
	delete gameGrid;
}

void Game::setGrid(Grid* grid) {
	if (status() != GS_IN_PROGRESS && grid != 0) { //make sure that the game is not currently in progress
		if (gridSet) {
			delete gameGrid;
		}
		gameGrid = grid;
		gridSet = true;
	}
	checkIfReadyToStart();
}

void Game::setPlayerOne(Player* player) {
	if (player != 0 && player != this->player2) { //make sure that the player passed in is not player 2
		player1 = player;
		player1Set = true;
	}
	checkIfReadyToStart();
}

void Game::setPlayerTwo(Player* player) {
	if (player != 0 && player != this->player1) { //make sure that the player passed in is not player1
		player2 = player;
		player2Set = true;
	}
	checkIfReadyToStart();
}

void Game::restart() {
	if (status() != GS_INVALID) { //clear appropriate variables etc. This method does nothing if status is GS_INVALID
		gameGrid->reset();
		player1->resetScore();
		player2->resetScore();
		checkIfReadyToStart();
		this->playerWon = 0;
		this->currentPlayer = 1; //reset first player to player1
	}
}

Game::Status Game::status() const {
	//obtain the gameStatus by checking gameStatus variable, gameStatus holds int value corresponding to the enum values
	if (this->gameStatus == 0) {
		return GS_IN_PROGRESS;
	} else if (this->gameStatus == 1) {
		return GS_COMPLETE;
	} else {
		return GS_INVALID; //3
	}
}

const Player* Game::winner() const {
	if (status() == GS_IN_PROGRESS || this->playerWon == 0) { //check if there's a winner by checking playerWon variable
		return 0;
	} else {
		if (playerWon == 1) {
			return player1; //return appropriate player
		} else { //player 2 won
			return player2;
		}
	}
}

//return the next player afther the current turn
const Player* Game::nextPlayer() const {
	if (status() == GS_IN_PROGRESS) {
		if (currentPlayer == 1) {
			return player1;
		} else {
			return player2;
		}
	} else {
		return 0;
	}

}

bool Game::playNextTurn(unsigned int column) {
	if (gameStatus == GS_IN_PROGRESS) {
		unsigned int discWasInserted = 0;
		if (currentPlayer == 1) {
			//if player is player1 insert GC_PLAYER_ONE into a grid at a specified column
			discWasInserted = gameGrid->insertDisc(column, Grid::GC_PLAYER_ONE);
		} else {
			discWasInserted = gameGrid->insertDisc(column, Grid::GC_PLAYER_TWO);
		}
		//if the disc was successfully inserted in to the column
		if (discWasInserted) {
			//call connectFoundFinder function to find connectFour combination
			connectFourFinder(column);
			//switch the player
			if (currentPlayer == 1) {
				currentPlayer = 2;
			} else {
				currentPlayer = 1;
			}
			//call checkIfFull function to check whether a chip can be inserted at any column next time playNextTurn is called.
			checkIfFull();
			return true;
		} else { //if disc was failed to be inserted
			return false;
		}
	} else {
		return false;
	}
}

void Game::checkIfFull() {
	unsigned int anotherTurnAvailable = 0;
	//Check to make sure that the top row (row0) is all occupied. If yes, then it means that the grid is full
	for (int i = 0; i < gameGrid->columnCount() - 1; i++) {
		if (gameGrid->cellAt(0, i) == Grid::GC_EMPTY) {
			anotherTurnAvailable = 1;
		}
	}
	if (!anotherTurnAvailable) {
		//if the gird is full (i.e. all column of row 0 is all occupied), set the game status to complete with
		//no player won
		playerWon = 0;
		gameStatus = GS_COMPLETE;
	}
}

void Game::connectFourFinder(int column) {
	unsigned int row = 0;
	//obtain the row that the recently inserted chip is in. This is done by checking to see which now of the
	//given column is occupied by a chip. The very first chip found must be the recently inserted chip.
	while ((gameGrid->cellAt(row, column) == Grid::GC_EMPTY)
			&& (row <= gameGrid->rowCount() - 1)) {
		row++;
	}

	//see what type of chip is in the given row,column position
	Grid::Cell insertedChip = gameGrid->cellAt(row, column);

	//check for possible combinations
	bool firstCondition = underFinder(row, column, insertedChip);
	bool secondCondition = horizontalFinder(row, column, insertedChip);
	bool thirdCondition = topLeftBottomRightFinder(row, column, insertedChip);
	bool fourthCondition = bottomLeftTopRightFinder(row, column, insertedChip);

	//if any of the condition has been met, incrase the player's score by 1
	if (firstCondition || secondCondition || thirdCondition
			|| fourthCondition) {
		gameStatus = 1; //Set game status to GS_COMPLETE
		if (currentPlayer == 1) {
			scoreWinIncrementor(1);
		} else if (currentPlayer == 2) {
			scoreWinIncrementor(2);
		}
	}
}

bool Game::underFinder(int row, int column, Grid::Cell chipType) {

	/*
	 * Check to see whether the 3 chips under the given chip (at a given row,column position) are of the same type
	 * as the given chip.
	 */

	unsigned int underFinder_failed = 0;
	if ((row + 3) <= (gameGrid->rowCount() - 1)) { //if there are at least 3 spaces under the recently inserted chip
		for (int i = row + 1; i <= row + 3; i++) {
			if (gameGrid->cellAt(i, column) != chipType) { //check for chipType
				underFinder_failed = 1; //if any chip under the inserted chip is not of the same type set this var to 1
			}
		}
		if (underFinder_failed) {
			return false;
		} else {
			return true;
		}
	} else { //if there are less than 3 spaces below the inserted chip, connect four is not possible, return false
		return false;
	}
}

bool Game::horizontalFinder(int row, int column, Grid::Cell chipType) {

	int chipCounter = 1;
	int columnSize = gameGrid->columnCount();

	//Check whether the chip on the left hand side of the given chip is of the same type.
	for (int i = column - 1; i >= 0; i--) {
		if (gameGrid->cellAt(row, i) != chipType) {
			break;
		}
		chipCounter++; //count up the number of chips that are of the same type as the given chip
	}

	//Check whether the chip on the right hand side of the given chip is of the same type
	for (int j = column + 1; j < columnSize; j++) {
		if (gameGrid->cellAt(row, j) != chipType) {
			break;
		}
		chipCounter++;
	}

	//if chipCounter is more than or equal to 4 then connectFour pattern has been formed
	if (chipCounter >= 4) {
		return true;
	} else {
		return false;
	}
}

bool Game::topLeftBottomRightFinder(int row, int column, Grid::Cell chipType) {
	int chipCounter = 0;
	int columnSize = gameGrid->columnCount();
	int rowSize = gameGrid->rowCount();

	int rowCount = row;
	int columnCount = column;

	//Check type of chips at diagonal, going up to TopLeft. While loop prevents the program from going over the boundary
	while (rowCount >= 0 && columnCount >= 0) {
		if (gameGrid->cellAt(rowCount, columnCount) == chipType) {
			chipCounter++;
			rowCount--;
			columnCount--;
		} else {
			break;
		}
	}

	/* Start counting 1 row below and 1 column to the right, this is becuase the given chip (at row,column passed into the
	 *  function) has already been taken into account in the previous while loop
	 */
	rowCount = row + 1;
	columnCount = column + 1;

	//Check type of chips at diagonal, going down to the Right
	while (rowCount < rowSize && columnCount < columnSize) {
		if (gameGrid->cellAt(rowCount, columnCount) == chipType) {
			chipCounter++;
			rowCount++;
			columnCount++;
		} else {
			break;
		}
	}

	if (chipCounter >= 4) {
		return true;
	} else {
		return false;
	}
}

bool Game::bottomLeftTopRightFinder(int row, int column, Grid::Cell chipType) {

	int chipCounter = 0;
	int columnSize = gameGrid->columnCount();
	int rowSize = gameGrid->rowCount();

	int rowCount = row;
	int columnCount = column;

	while (columnCount >= 0 && rowCount < rowSize) { //Going down the rows to the left of the grid
		if (gameGrid->cellAt(rowCount, columnCount) == chipType) {
			chipCounter++;
			rowCount++;
			columnCount--;
		} else {
			break;
		}
	}

	rowCount = row - 1;
	columnCount = column + 1;

	while (columnCount < columnSize && rowCount >= 0) { //tgoing up the rows to the right of the grid
		if (gameGrid->cellAt(rowCount, columnCount) == chipType) {
			chipCounter++;
			rowCount--;
			columnCount++;
		} else {
			break;
		}
	}

	if (chipCounter >= 4) {
		return true;
	} else {
		return false;
	}
}

void Game::checkIfReadyToStart() {
	if (gridSet == true && player1Set == true && player2Set == true) {
		gameStatus = 0; //when everything is ready, set game status to GS_IN_PROGRESS
	}
}

//Increment approprate player's score
void Game::scoreWinIncrementor(unsigned int player) {
	if (player == 1) {
		playerWon = 1;
		player1->increaseWins();
		player1->increaseScore();
	} else {
		playerWon = 2;
		player2->increaseWins();
		player2->increaseScore();
	}
}
