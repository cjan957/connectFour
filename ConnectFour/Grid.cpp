#include "Grid.hpp"
#include <iostream>
using namespace std;

Grid::Grid(unsigned int rows, unsigned int columns) {
	//Force the size of the grid to be at least 4x4
	if (rows < 4) {
		rows = 4;
	}
	if (columns < 4) {
		columns = 4;
	}
	gridVector.resize(columns, std::vector<int>(rows, GC_EMPTY));
	this->rowSize = rows;
	this->columnSize = columns;
}

Grid::~Grid() {

}

bool Grid::insertDisc(unsigned int column, Cell disc) {
	//determine the maximum number of elements.
	int i = rowCount() - 1;

	//check for out of bound area & empty chip
	if (disc == GC_EMPTY || column >= columnSize) {
		return false;
	} else {
		//insert the chip at a given column, making sure that it's is put on top of any chips that are already in there
		while (gridVector[column][i] != GC_EMPTY && i >= 0) {
			i--;
		}
		if (i == -1) {
			return false;
		} else {
			gridVector[column][i] = disc;
			return true;
		}
	}
}

Grid::Cell Grid::cellAt(unsigned int row, unsigned int column) const {
	//check for out of bound row/column
	if (row >= rowSize || column >= columnSize) {
		return GC_EMPTY;
	} else {
		//check the chip's Type at a given row, column.
		int chipType = gridVector[column][row];
		if (chipType == 0) {
			return GC_EMPTY;
		} else if (chipType == 1) {
			return GC_PLAYER_ONE;
		} else {
			return GC_PLAYER_TWO;
		}
	}
}

//use in superGame, remove a chip from the grid (by replacing it with GC_EMPTY)
void Grid::removeChipFromGrid(int row, int column) {
	gridVector[column][row] = GC_EMPTY;
}

void Grid::reset() {
	gridVector.clear();
	gridVector.resize(columnSize, std::vector<int>(rowSize, GC_EMPTY));
}

unsigned int Grid::rowCount() const {
	return rowSize;
}

unsigned int Grid::columnCount() const {
	return columnSize;
}

//assign a chip into the specified position of the grid
void Grid::assignChipToGrid(int row, int column, Cell chip) {
	gridVector[column][row] = chip;
}
