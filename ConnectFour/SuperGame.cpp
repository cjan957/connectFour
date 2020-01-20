#include "SuperGame.hpp"
using namespace std;

// TODO: implement SuperGame class here
SuperGame::SuperGame() {
	this->player1ScoreAlready = 0;
	this->player2ScoreAlready = 0;
}

void SuperGame::connectFourFinder(int column) {
	int row = 0;

	//finding the row number of the latest inserted chip, by finding the row position with a chip from the top.
	//as it must be the recently inserted chip.
	while ((gameGrid->cellAt(row, column) == Grid::GC_EMPTY)
			&& (row <= gameGrid->rowCount() - 1)) {
		row++;
	}
	Grid::Cell insertedChip = gameGrid->cellAt(row, column);
	bool firstCondition = underFinder(row, column, insertedChip);
	bool secondCondition = horizontalFinder(row, column, insertedChip);
	bool thirdCondition = topLeftBottomRightFinder(row, column, insertedChip);
	bool fourthCondition = bottomLeftTopRightFinder(row, column, insertedChip);
	if (firstCondition) {
		removeBottom(row, column); //removes chips directly below the given chip
	}
	if (secondCondition) {
		removeHorizontal(row, column, insertedChip); //removes chips horizontally
	}
	if (thirdCondition) {
		removeTopLeftBottomRight(row, column, insertedChip); //removes chips in diagonal from topLeft to bottom right (\)
	}
	if (fourthCondition) {
		removeBottomLeftTopRight(row, column, insertedChip); //removes chips in diagonal from bottom left up to top right (/)
	}

	if (firstCondition || secondCondition || thirdCondition
			|| fourthCondition) {

		chipShifter(); //shift all the chip down if there's a space under each chip
		if (currentPlayer == 1) {
			scoreWinIncrementor(1);
		} else if (currentPlayer == 2) {
			scoreWinIncrementor(2);
		}
		checkAfterShifted(); //check each chip for any possible connect4 connection
	}
}

void SuperGame::checkAfterShifted() {
	//Grid::Cell emptyChip = Grid::GC_EMPTY;
	int rowSize = gameGrid->rowCount();
	int colSize = gameGrid->columnCount();
	for (int i = 0; i < rowSize; i++) {
		for (int j = 0; j < colSize; j++) {

			//Check every chip on the grid for any possible connections, skipping any position that's empty
			if (gameGrid->cellAt(i, j) != Grid::GC_EMPTY) {
				connectFourFinderAfterShifted(i, j);
			}
		}
	}
}

//This function check for any possible connect4 connections AT A GIVEN ROW & COLUMN POSITION
void SuperGame::connectFourFinderAfterShifted(int row, int column) {

	Grid::Cell insertedChip = gameGrid->cellAt(row, column);

	bool firstCondition = underFinder(row, column, insertedChip);
	bool secondCondition = horizontalFinder(row, column, insertedChip);
	bool thirdCondition = topLeftBottomRightFinder(row, column, insertedChip);
	bool fourthCondition = bottomLeftTopRightFinder(row, column, insertedChip);
	if (firstCondition) {
		removeBottom(row, column);
	}
	if (secondCondition) {
		removeHorizontal(row, column, insertedChip);
	}
	if (thirdCondition) {
		removeTopLeftBottomRight(row, column, insertedChip);
	}
	if (fourthCondition) {
		removeBottomLeftTopRight(row, column, insertedChip);
	}

	if (firstCondition || secondCondition || thirdCondition
			|| fourthCondition) {
		chipShifter();
		if (insertedChip == Grid::GC_PLAYER_ONE) {
			//check if player 1's score has already been increased, this prevents any duplicate increase in score
			//while the chips are falling down.
			if (!player1ScoreAlready) {
				scoreWinIncrementor(1);
				player1ScoreAlready = 1;
			}
		} else if (insertedChip == Grid::GC_PLAYER_TWO) {
			if (!player2ScoreAlready) {
				scoreWinIncrementor(2);
				player2ScoreAlready = 1;
			}
		}
		//checkAfterShifted();
	}

}

//Removing the 3 chips under the given chip by calling removeChipFromGrid function defined in grid class
void SuperGame::removeBottom(int row, int column) {
	for (int i = row; i <= row + 3; i++) {
		gameGrid->removeChipFromGrid(i, column);
	}
}

void SuperGame::removeHorizontal(int row, int column, Grid::Cell chipType) {

	int columnSize = gameGrid->columnCount();

	int head_column = column;
	int tail_column = column;

	//check the chips on the left of the given chip to see whether there are of the same type
	//each time a nearby chip type matches with the given chip, the position of the nearby chip
	//is saved into head_column variable.
	for (int i = column - 1; i >= 0; i--) {
		if (gameGrid->cellAt(row, i) != chipType) {
			break;
		}
		head_column = i;
	}

	//Similar to the previous loop function, but going to the right of the grid instead.
	for (int j = column + 1; j < columnSize; j++) {
		if (gameGrid->cellAt(row, j) != chipType) {
			break;
		}
		tail_column = j;
	}

	//remove all the chips that are within the range (head and tail) determined by the previous loops
	for (int k = head_column; k <= tail_column; k++) {
		gameGrid->removeChipFromGrid(row, k);
	}
}
void SuperGame::removeTopLeftBottomRight(int row, int column,
		Grid::Cell chipType) {

	int columnSize = gameGrid->columnCount();
	int rowSize = gameGrid->rowCount();

	int head_column = column;
	int head_row = row;
	int tail_column = column;
	int tail_row = row;

	int rowCount = row;
	int columnCount = column;

	//How these loops work is similar to the horizontalRemove function but since this function check for chips
	//diagonally, both row and column need to be changed every iteration.
	while (rowCount >= 0 && columnCount >= 0) {
		if (gameGrid->cellAt(rowCount, columnCount) == chipType) {
			head_column = columnCount;
			head_row = rowCount;
			rowCount--;
			columnCount--;
		} else {
			break;
		}

	}

	rowCount = row + 1;
	columnCount = column + 1;

	while (rowCount < rowSize && columnCount < columnSize) { //bottom Right
		if (gameGrid->cellAt(rowCount, columnCount) == chipType) {
			tail_column = columnCount;
			tail_row = rowCount;
			rowCount++;
			columnCount++;
		} else {
			break;
		}
	}

	int rowToRemove = head_row;
	for (int i = head_column; i <= tail_column; i++) {
		gameGrid->removeChipFromGrid(rowToRemove, i);
		rowToRemove++;
	}
}
void SuperGame::removeBottomLeftTopRight(int row, int column,
		Grid::Cell chipType) {

	int columnSize = gameGrid->columnCount();
	int rowSize = gameGrid->rowCount();

	int head_column = column;
	int head_row = row;
	int tail_column = column;
	int tail_row = row;

	int rowCount = row + 1;
	int columnCount = column - 1;

	//Similar to the previous functions but going in a different direction
	while (columnCount >= 0 && rowCount < rowSize) { //bottomLeft
		if (gameGrid->cellAt(rowCount, columnCount) == chipType) {
			head_column = columnCount;
			head_row = rowCount;
			rowCount++;
			columnCount--;
		} else {
			break;
		}
	}

	rowCount = row - 1;
	columnCount = column + 1;

	while (columnCount < columnSize && rowCount >= 0) { //topRight
		if (gameGrid->cellAt(rowCount, columnCount) == chipType) {
			tail_column = columnCount;
			tail_row = rowCount;
			rowCount--;
			columnCount++;
		} else {
			break;
		}
	}

	int rowToRemove = head_row;
	for (int i = head_column; i <= tail_column; i++) {
		gameGrid->removeChipFromGrid(rowToRemove, i);
		rowToRemove--;
	}
}

void SuperGame::chipShifter() {

	//Shift all the chips down if there's space under them.
	Grid::Cell emptyChip = Grid::GC_EMPTY;
	//int stilShiftable = 1;
	int rowSize = gameGrid->rowCount();
	int colSize = gameGrid->columnCount();
	int noOfShift = 0;
	while (noOfShift < rowSize) {
		for (int i = rowSize - 2; i >= 0; i--) {
			for (int j = 0; j < colSize; j++) {
				//Check to see if the row below a specific column is empty
				if (gameGrid->cellAt(i + 1, j) == emptyChip) {
					//if it is empty then we shift the chip!
					gameGrid->assignChipToGrid(i + 1, j,
							gameGrid->cellAt(i, j));
					//and replace the shifted chip's position with empty space
					gameGrid->assignChipToGrid(i, j, emptyChip);
				}
			}
		}
		noOfShift++; //loop for the size of the row. This makes sure that all the chips are fully shifted
	}
}

void SuperGame::scoreWinIncrementor(unsigned int player) {
	if (player == 1) {
		player1->increaseScore();
	} else {
		player2->increaseScore();
	}
	//checkIfDone();
}

void SuperGame::checkIfFull() {
	//Similar to the Game checkIfFull but with extra functinality due to the fact that the score counting of
	//supergame is different from the standard game

	//Reset player1/2scoreAlready variable, these variables were used to make sure that no more than 1 point
	//is given to a player after the chips dropped and connectFour links have been 'accidentally' made.
	player1ScoreAlready = 0;
	player2ScoreAlready = 0;

	unsigned int anotherTurnAvailable = 0;
	unsigned int columnSize = gameGrid->columnCount();
	for (int i = 0; i < columnSize; i++) {
		if (gameGrid->cellAt(0, i) == Grid::GC_EMPTY) {
			anotherTurnAvailable = 1;
		}
	}

	//if no more turn is available (Grid is full), determine the winner by checking their scores.
	if (!anotherTurnAvailable) {
		gameStatus = GS_COMPLETE;
		if (player1->getScore() > player2->getScore()) { //player 1 win
			player1->increaseWins();
			playerWon = 1;
		} else if (player1->getScore() < player2->getScore()) {
			player2->increaseWins();
			playerWon = 2;
		}
	}
}

