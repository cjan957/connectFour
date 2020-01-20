#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "Grid.hpp"

/*
The game class oversees all game logic and state. Once a game is created it must be assigned two players and a grid
before it is playable. The game starts with Player One's turn. Each time playNextTurn is called the current player's
turn is executed, with each player taking turns alternately. Each time a Player wins, their score and win counters
should be incremented, and both player's scored should be reset when the Game is restarted.

When the Game class is destroyed, it should delete any assigned Grid. The Game does not own assigned Players, so it
should never try to delete them.
*/
class Game {
protected:


	Grid *gameGrid;
	Player *player1;
	Player *player2;

	//vairables help track info
	unsigned int playerWon;
	unsigned int currentPlayer;

	unsigned int gameStatus;
	bool gridSet;
	bool player1Set;
	bool player2Set;

public:
    /*
    The Status enum represents the current core game state; in progress, complete, or invalid. The GS_COMPLETE state can
    be reached either by one player winning, or a draw being reached (no more moves can be made). The `winner` method of
    the Game can be used to determine whether a game was won or drawn.

    The GS_IN_PROGRESS status means the game is currently in progress between two players.

    The status GS_INVALID means the game is not playable. The game will have this status from when it is created until
    it is fully initialized (i.e. two valid players and a grid are assigned).
    */
    enum Status { GS_IN_PROGRESS, GS_COMPLETE, GS_INVALID };

    /*
    Create a new game. The game status will be set to GS_INVALID until both players and a grid have all been set using
    the appropriate setter methods.
    */
    Game();

    /*
    Clean up the Game, destroying any heap-allocated objects owned by this Game, including any Grid assigned to this
    Game.
    */
    virtual ~Game();


    /*
    Set the Grid used by this Game. Any Grid previously assigned to this game will be deleted before the new one is
    assigned. If the Grid to be assigned is a null pointer (0), this method should have no effect on the Game or the
    currently assigned Grid. When the Game class is destroyed, it should delete any assigned Grid.

    If two valid players have been assigned to the Game (player one and player two) already when the grid is assigned,
    assigning the grid will cause the game status to change from GS_INVALID to GS_IN_PROGRESS.
    */
    virtual void setGrid(Grid* grid);

    /*
    Set the specified player as Player One. This method will have no effect if the specified player is a null pointer,
    or if the specified player is already assigned as Player Two.

    If this player is the last to be assigned (Player Two already has a valid player assigned) and a grid has already
    been assigned to this game, then when this player is assigned the game status should be changed from GS_INVALID to
    GS_IN_PROGRESS.
    */
    virtual void setPlayerOne(Player* player);

    /*
    Set the specified player as Player Two. This method will have no effect if the specified player is a null pointer,
    or if the specified player is already assigned as Player One.

    If this player is the last to be assigned (Player One already has a valid player assigned) and a grid has already
    been assigned to this game, then when this player is assigned the game status should be changed from GS_INVALID to
    GS_IN_PROGRESS.
    */
    virtual void setPlayerTwo(Player* player);

    /*
    Restart the current game, clearing the grid and resetting all Game state (including the scores of both players). If
    this method is called with two valid Players and a valid grid assigned, the Game status should be GS_IN_PROGRESS
//    after this method is called. This method should have no effect if the current game status is GS_INVALID.
    */
    virtual void restart();

    /*
    Get the current game status. See the Status enum for more information about the particular values that can be
    returned by this method.
    */
    virtual Status status() const;

    /*
    If the current game status is GS_COMPLETE, the `winner` method will return a const pointer to the winning player.
    If the game is GS_IN_PROGRESS or the outcome of the game is a draw, `winner` will return a null pointer (0).
    */
    virtual const Player* winner() const;

    /*
    Get the player whose move will be executed the next time `playNextTurn` is called. If the game status is not
    GS_IN_PROGRESS, this method should return a null pointer (0).
    */
    virtual const Player* nextPlayer() const;

    /*
    Execute the turn of the next player by attempting to insert a disc into the indicated column of the game grid. If
    the move was successful, this method should return `true`. If the move was could not be completed (e.g. the
    specified column in the grid is full, or the game is invalid or already complete) this method should return `false`.
    Invalid moves (i.e. where this method returns `false`) should have no effect on the state of the Game or the Grid.

    This method should have no effect if the current game status is not GS_IN_PROGRESS.
    */
    virtual bool playNextTurn(unsigned int column);

    virtual void checkIfReadyToStart();

    virtual void connectFourFinder( int column);

    virtual bool underFinder( int row, int column, Grid::Cell chipType);
    virtual bool horizontalFinder( int row, int column, Grid::Cell chipType);
    virtual bool topLeftBottomRightFinder( int row, int column, Grid::Cell chipType);
    virtual bool bottomLeftTopRightFinder( int row, int column, Grid::Cell chipType);

    virtual void checkIfFull();

    virtual void scoreWinIncrementor(unsigned int player);
};

#endif /* end of include guard: GAME_HPP */
