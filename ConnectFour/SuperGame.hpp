#ifndef SUPERGAME_HPP
#define SUPERGAME_HPP

#include "Game.hpp"
#include "Grid.hpp"

class SuperGame: public Game {
private:
	int player1ScoreAlready;
	int player2ScoreAlready;
public:
	SuperGame();
	// TODO: complete definition of SuperGame class here
	void connectFourFinder(int column);
	void removeBottom(int row, int column);
	void removeHorizontal(int row, int column, Grid::Cell chipType);
	void removeTopLeftBottomRight(int row, int column, Grid::Cell chipType);
	void removeBottomLeftTopRight(int row, int column, Grid::Cell chipType);

	void chipShifter();
	void scoreWinIncrementor(unsigned int player);
	void checkAfterShifted();
	void connectFourFinderAfterShifted(int row, int column);
	void checkIfFull();
};

#endif /* end of include guard: SUPERGAME_HPP */
