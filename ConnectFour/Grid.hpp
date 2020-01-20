#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <iostream>

/*
 The Grid class represents a vertically suspended 2D grid of cells in a Connect Four game. Each player may insert discs
 into the top of the grid, where they will fall vertically (due to gravity) to rest on top of the other discs already
 stacked in that column.

 The Grid size can be specified at construction. If the grid dimensions are N x M (rows x columns), the top row of the
 grid where players insert discs is row 0, while the bottom row of the Grid is row (N-1). The left-most column is column
 0, while the right-most column is colum (M-1).

 Each cell of the grid will be represented by the Cell enum. Each cell can be either empty or occupied by a disc owned by
 one of the two players currently playing.
 */
class Grid {
private:
	std::vector<std::vector<int> > gridVector;
	int rowSize;
	int columnSize;
public:
	/*
	 The Cell enum represents the value of a single cell in the Grid. The cell may contain a token from either player
	 one or player two, or it can be empty.
	 */
	enum Cell {
		GC_EMPTY, GC_PLAYER_ONE, GC_PLAYER_TWO
	};

	/*
	 Create a new game Grid with the specified number of rows and columns. All cells should be initialized to the
	 GC_EMPTY state. The grid created should never be less than 4 rows and 4 columns in size. If the number of rows or
	 columns specified is less than 4, the offending dimension should be corrected to be exactly 4. e.g. if a grid of 3x7
	 cells is constructed, the Grid produced by the constructor should actually have the dimensions 4x7.
	 */
	Grid(unsigned int rows, unsigned int columns);

	/*
	 Grid destructor, cleans up any resoucres owned by this class (such as heap-allocated memory)
	 */
	~Grid();

	/*
	 Insert a new disc at the specified column of the Grid. The disc to be inserted is specified by the `disc` parameter.
	 If the disc is GC_EMPTY, nothing will be inserted into the Grid. Otherwise, a disc is inserted for the corresponding
	 player.

	 Once a disc is inserted at the top of the Grid, it propogates vertically straight down until it reaches the bottom
	 of the grid or is blocked by an existing disc directly below it.

	 If the disc to be inserted was GC_EMPTY, there is no space to add a disc to the specified column, or the column
	 specified was outside the bounds of the grid, this method will return `false`. Otherwise if the insertion was
	 successful this method should return `true`.
	 */
	bool insertDisc(unsigned int column, Cell disc);

	/*
	 Get the value of the cell at the specified row and column of the Grid. Returns GC_EMPTY if `row` or `column` are
	 outside the bounds of the grid.
	 */
	Cell cellAt(unsigned int row, unsigned int column) const;

	/*
	 Reset the state of the Grid, removing all player discs and setting the state of all cells to GC_EMPTY.
	 */
	void reset();
	/*
	 Return the number of rows in the grid.
	 */
	unsigned int rowCount() const;

	/*
	 Return the number of columns in the grid.
	 */
	unsigned int columnCount() const;

	void removeChipFromGrid(int row, int column);
	void assignChipToGrid(int row, int column, Cell chip);
};

#endif /* end of include guard: GRID_HPP */
