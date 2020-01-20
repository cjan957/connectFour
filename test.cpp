#include <iostream>
#include <cctype>
#include <cstdlib>
#include <vector>

// flags to enable tests for the later parts of the assignment
#define ENABLE_T1_TESTS
#define ENABLE_T2_TESTS
#define ENABLE_T3_TESTS
#define ENABLE_T4_TESTS

// include headers for classes being tested
#ifdef ENABLE_T1_TESTS
#include "ConnectFour/Player.hpp"
#endif /*ENABLE_T1_TESTS*/
#ifdef ENABLE_T2_TESTS
#include "ConnectFour/Player.hpp"
#include "ConnectFour/Grid.hpp"
#endif /*ENABLE_T2_TESTS*/
#ifdef ENABLE_T3_TESTS
#include "ConnectFour/Player.hpp"
#include "ConnectFour/Grid.hpp"
#include "ConnectFour/Game.hpp"
#endif /*ENABLE_T3_TESTS*/
#ifdef ENABLE_T4_TESTS
#include "ConnectFour/Player.hpp"
#include "ConnectFour/Grid.hpp"
#include "ConnectFour/SuperGame.hpp"
#endif /*ENABLE_T4_TESTS*/

using namespace std;

// This sigma value is the acceptable range of error for floating point (double) calculations
const double SIGMA = 0.001;

/*
The outcome of each test case is described by this enum, which should make the result more obvious
than if a boolean value was used in its place.
*/
enum TestResult { TR_PASS, TR_FAIL };

/*
Helper macro to eliminate a lot of of code repetition. Macros aren't the best, but this is an easy
way to get what we want.
*/

#define ASSERT(condition)                                                         \
    if ((condition) == false) {                                                   \
        cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << "\n"                \
             << "\tExpression '" << #condition << "' evaluated to false" << endl; \
        return TR_FAIL;                                                           \
    }

/*
Helper macro to simplify the checking of double values against expected results. Directly comparing
values would result in brittle tests, so we have to instead check if the value we got is acceptably
close to the expected value.
*/
#define EXPECT_SIGMA(value, expected)                                                                            \
    {                                                                                                            \
        double value_eval = value;                                                                               \
        double expected_eval = expected;                                                                         \
        if (value_eval <= (expected_eval - SIGMA) || value_eval > (expected_eval + SIGMA)) {                     \
            cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << "\n"                                           \
                 << "\tFloating-point value " << value_eval << " differed from expected value " << expected_eval \
                 << " by more than " << SIGMA << "\n"                                                            \
                 << "     with operands '" << #value << "' and '" << #expected << "'" << endl;                   \
            return TR_FAIL;                                                                                      \
        }                                                                                                        \
    }

// only use this if Grid.hpp has been included (tasks 2, 3 and 4)
#ifdef GRID_HPP
/*
Helper function to verify grid state matches a given pattern, rather than having to check each cell manually. Expects a
string to match the target grid against. Each character in the space represents a cell in the grid. A value of ' '
(space) represents an empty cell, '1' represents a disc belonging to player one, and '2' represents a disc belonging to
player two.

e.g. (for a 6x7 grid)

"       "
"       "
"       "
"  22  1"
" 2111 2"
"1122112"
*/
bool verifyGridState(const Grid& grid, const std::string& state) {
    // return false if the grid size doesn't match the size of the given grid state
    if (state.size() != (grid.rowCount() * grid.columnCount())) {
        return false;
    }
    for (unsigned int r = 0; r < grid.rowCount(); ++r) {
        for (unsigned int c = 0; c < grid.columnCount(); ++c) {
            // convert grid coordinates to linear state coordinates
            unsigned int i = (r * grid.columnCount()) + c;
            
            Grid::Cell cell = Grid::GC_EMPTY;
            if (state[i] == '1') {
                cell = Grid::GC_PLAYER_ONE;
            } else if (state[i] == '2') {
                cell = Grid::GC_PLAYER_TWO;
            }
            if (grid.cellAt(r, c) != cell) {
                return false;
            }
        }
    }
    return true;
}

#endif /*GRID_HPP*/

#ifdef ENABLE_T1_TESTS
/*
Test Player construction (and destruction).
*/
TestResult test_PlayerConstruction() {
    // scope the instance to explicitly invoke the destructor before the return
    {
        Player p1("Nick");
        // prevent the compiler from optimizing out the instance
        (void)p1;
    }

    return TR_PASS;  //test0
}

/*
Test Player name is initialized correctly.
*/
TestResult test_PlayerName() {
    Player p1("Nick");
    ASSERT(p1.getName() == "Nick");

    return TR_PASS;  //test1
}

/*
Test Player score is initialized correctly.
*/
TestResult test_PlayerScore() {
    Player p1("Nick");
    ASSERT(p1.getScore() == 0);

    return TR_PASS;  //test2
}

/*
Test Player score is incremented correctly.
*/
TestResult test_PlayerIncreaseScore() {
    Player p1("Nick");
    ASSERT(p1.getScore() == 0);
    p1.increaseScore();
    ASSERT(p1.getScore() == 1);
    p1.increaseScore();
    ASSERT(p1.getScore() == 2);
    
    return TR_PASS;  //test3
}

/*
Test Player score is reset correctly.
*/
TestResult test_PlayerResetScore() {
    Player p1("Nick");
    ASSERT(p1.getScore() == 0);
    p1.increaseScore();
    p1.increaseScore();
    p1.increaseScore();
    ASSERT(p1.getScore() == 3);
    p1.resetScore();
    ASSERT(p1.getScore() == 0);
    
    return TR_PASS;  //test4
}

/*
Test Player total wins is incremented correctly.
*/
TestResult test_PlayerWins() {
    Player p1("Nick");
    ASSERT(p1.getWins() == 0);
    p1.increaseWins();
    p1.increaseWins();
    ASSERT(p1.getWins() == 2);
    
    return TR_PASS;  //test5
}
#endif /*ENABLE_T1_TESTS*///////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_T2_TESTS
/*
Test Grid construction and destruction.
*/
TestResult test_GridConstruction() {
    // scope the instance to explicitly invoke the destructor before the return
    {
        Grid grid(6, 7);
        // prevent the compiler from optimizing out the instance
        (void)grid;
    }
    
    return TR_PASS;  //test6
}

/*
Test Grid size is correctly set on construction
*/
TestResult test_GridSizeValid() {
    // try and create the smallest possible grid
    {
        Grid grid(4, 4);
        ASSERT(grid.rowCount() == 4);
        ASSERT(grid.columnCount() == 4);
    }
    
    // try and create a 6x7 grid (the standard Connect 4 grid)
    {
        Grid grid(6, 7);
        ASSERT(grid.rowCount() == 6);
        ASSERT(grid.columnCount() == 7);
    }
    
    // try and create a 9x12 grid
    {
        Grid grid(9, 12);
        ASSERT(grid.rowCount() == 9);
        ASSERT(grid.columnCount() == 12);
    }
    
    return TR_PASS;  //test7
}

/*
Test Grid size is correctly set on construction for grid sizes less than 4x4. It shouldn't be possible to create a grid
any dimension smaller than 4.
*/
TestResult test_GridSizeInvalid() {
    // try and create a 3x1 grid
    {
        Grid grid(3, 1);
        ASSERT(grid.rowCount() == 4);
        ASSERT(grid.columnCount() == 4);
    }
    
    // try and create a 0x2 grid
    {
        Grid grid(0, 2);
        ASSERT(grid.rowCount() == 4);
        ASSERT(grid.columnCount() == 4);
    }
    
    // try and create a 2x9 grid
    {
        Grid grid(2, 9);
        ASSERT(grid.rowCount() == 4);
        ASSERT(grid.columnCount() == 9);
    }
    
    return TR_PASS;  //test8
}

/*
Try and access all elements in the default grid.
*/
TestResult test_GridAccessEmpty() {
    Grid grid(6, 7);
    
    // Check the grid is empty
    for (unsigned int r = 0; r < 6; ++r) {
        for (unsigned int c = 0; c < 7; ++c) {
            ASSERT(grid.cellAt(r, c) == Grid::GC_EMPTY);
        }
    }
    
    return TR_PASS;  //test9
}

/*
Test insertion of discs into valid columns in a grid.
*/
TestResult test_GridInsertDisc() {
    Grid grid(6, 7);
    
    ASSERT(grid.insertDisc(0, Grid::GC_PLAYER_ONE));
    // Check the correct cell in the target column was changed
    std::string gridState = "       "
                            "       "
                            "       "
                            "       "
                            "       "
                            "1      ";
    ASSERT(verifyGridState(grid, gridState));
    
    ASSERT(grid.insertDisc(0, Grid::GC_PLAYER_TWO));
    // Check the correct cell in the target column was changed
    gridState = "       "
                "       "
                "       "
                "       "
                "2      "
                "1      ";
    ASSERT(verifyGridState(grid, gridState));
    
    // check the rest of the grid is still blank
    for (unsigned int r = 0; r < 6; ++r) {
        for (unsigned int c = 1; c < 7; ++c) {
            ASSERT(grid.cellAt(r, c) == Grid::GC_EMPTY);
        }
    }
    
    return TR_PASS;  //test10
}

/*
Test insertion of discs into grid columns that are full.
*/
TestResult test_GridInsertFull() {
    Grid grid(6, 7);
    
    // fill a column with discs from one player
    ASSERT(grid.insertDisc(0, Grid::GC_PLAYER_ONE));
    ASSERT(grid.insertDisc(0, Grid::GC_PLAYER_ONE));
    ASSERT(grid.insertDisc(0, Grid::GC_PLAYER_ONE));
    ASSERT(grid.insertDisc(0, Grid::GC_PLAYER_ONE));
    ASSERT(grid.insertDisc(0, Grid::GC_PLAYER_ONE));
    ASSERT(grid.insertDisc(0, Grid::GC_PLAYER_ONE));
    // Check all cells in the target column were filled
    std::string gridState = "1      "
                            "1      "
                            "1      "
                            "1      "
                            "1      "
                            "1      ";
    ASSERT(verifyGridState(grid, gridState));
    // Check no more discs can be inserted
    ASSERT(!grid.insertDisc(0, Grid::GC_PLAYER_ONE));
    ASSERT(!grid.insertDisc(0, Grid::GC_PLAYER_TWO));
    ASSERT(verifyGridState(grid, gridState));
    
    return TR_PASS;  //test11
}

/*
Test that the grid state is reset when the reset method is called.
*/
TestResult test_GridReset() {
    Grid grid(6, 7);
    
    // insert a selection of discs
    ASSERT(grid.insertDisc(0, Grid::GC_PLAYER_ONE));
    ASSERT(grid.insertDisc(1, Grid::GC_PLAYER_TWO));
    ASSERT(grid.insertDisc(0, Grid::GC_PLAYER_TWO));
    ASSERT(grid.insertDisc(0, Grid::GC_PLAYER_TWO));
    // Check all cells were filled
    std::string gridState = "       "
                            "       "
                            "       "
                            "2      "
                            "2      "
                            "12     ";
    ASSERT(verifyGridState(grid, gridState));
    // perform reset
    grid.reset();
    // Check all cells were cleared
    for (unsigned int r = 0; r < 6; ++r) {
        for (unsigned int c = 0; c < 7; ++c) {
            ASSERT(grid.cellAt(r, c) == Grid::GC_EMPTY);
        }
    }
    
    return TR_PASS;  //test12
}
#endif /*ENABLE_T2_TESTS*/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_T3_TESTS
/*
Test construction and destruction of the Game class.
*/
TestResult test_GameConstruction() {
    // scope the instance to explicitly invoke the destructor before the return
    {
        Game game;
    }
    
    return TR_PASS;  //test13
}

/*
Test initial game state once the game is in progress.
*/
TestResult test_GameInitialState() {
    Game game;
    ASSERT(game.status() == Game::GS_INVALID);
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == 0);
    ASSERT(!game.playNextTurn(0));
    
    return TR_PASS;  //test14
}

/*
Test assigning of a Grid to an instance of the Game class.
*/
TestResult test_GameSetGrid() {
    Grid* grid = new Grid(6, 7);
    Game game;
    ASSERT(game.status() == Game::GS_INVALID);
    game.setGrid(grid);
    ASSERT(game.status() == Game::GS_INVALID);
    ASSERT(game.nextPlayer() == 0);
    ASSERT(game.winner() == 0)
    ASSERT(!game.playNextTurn(0));
    
    return TR_PASS;   //test15
}

/*
Test assigning of Players to the Game class.
*/
TestResult test_GameSetPlayers() {
    Grid* grid = new Grid(6, 7);
    Game game;
    
    // set up the grid and players to make the game valid
    ASSERT(game.status() == Game::GS_INVALID);
    game.setGrid(grid);
    ASSERT(game.status() == Game::GS_INVALID);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    ASSERT(game.status() == Game::GS_INVALID);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == &p1);
    
    // try and unset the players, should have no effect
    game.setPlayerOne(0);
    game.setPlayerTwo(0);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == &p1);
    
    return TR_PASS;   //test16
}

/*
Test a simple first move by player one.
*/
TestResult test_GameFirstMove() {
    Grid* grid = new Grid(6, 7);
    Game game;
    game.setGrid(grid);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    
    // Play first turn
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.nextPlayer() == &p1);
    ASSERT(game.playNextTurn(3));
    // Check grid state
    std::string gridState = "       "
                            "       "
                            "       "
                            "       "
                            "       "
                            "   1   ";
    ASSERT(verifyGridState(*grid, gridState));
    
    return TR_PASS;   //test17
}

/*
Test restarting a completed game.
*/
TestResult test_GameRestartCompleted() {
    Grid* grid = new Grid(6, 7);
    Game game;
    game.setGrid(grid);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    
    // play a quick player one win
    unsigned int t = 0;
    while (game.status() == Game::GS_IN_PROGRESS) {
        if (t % 2 == 0) {
            // P1 plays in column 0
            ASSERT(game.playNextTurn(0));
        } else {
            // P2 plays in column 6
            ASSERT(game.playNextTurn(6));
        }
        ++t;
    }
    // Check grid state
    std::string gridState = "       "
                            "       "
                            "1      "
                            "1     2"
                            "1     2"
                            "1     2";
    ASSERT(verifyGridState(*grid, gridState));
    ASSERT(game.status() == Game::GS_COMPLETE);
    ASSERT(game.nextPlayer() == 0);
    ASSERT(game.winner() == &p1);
    // check player scores and wins have been modified appropriately
    ASSERT(p2.getScore() == 0);
    ASSERT(p2.getWins() == 0);
    ASSERT(p1.getScore() == 1);
    ASSERT(p1.getWins() == 1);
    
    // restart the game
    game.restart();
    // verify the game state has been reset
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.nextPlayer() == &p1);
    ASSERT(game.winner() == 0);
    ASSERT(p1.getScore() == 0);
    ASSERT(p1.getWins() == 1);
    // verify grid has been reset
    for (unsigned int r = 0; r < 6; ++r) {
        for (unsigned int c = 0; c < 7; ++c) {
            ASSERT(grid->cellAt(r, c) == Grid::GC_EMPTY);
        }
    }
    
    return TR_PASS; //test18
}

/*
Test insertion of a disc in a full column is rejected.
*/
TestResult test_GameFullColumn() {
    Grid* grid = new Grid(6, 7);
    Game game;
    game.setGrid(grid);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    
    // Fill first column
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.nextPlayer() == &p1);
    ASSERT(game.playNextTurn(0));
    ASSERT(game.playNextTurn(0));
    ASSERT(game.playNextTurn(0));
    ASSERT(game.playNextTurn(0));
    ASSERT(game.playNextTurn(0));
    ASSERT(game.playNextTurn(0));
    
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.nextPlayer() == &p1);
    
    // Check grid state
    std::string gridState = "2      "
                            "1      "
                            "2      "
                            "1      "
                            "2      "
                            "1      ";
    ASSERT(verifyGridState(*grid, gridState));
    
    // Try to place an extra disc in the first column
    ASSERT(!game.playNextTurn(0));
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.nextPlayer() == &p1);
    
    return TR_PASS; //test19
}

/*
Test stalemate (tie) game where the grid is completely full but no rows of 4 have been made in any direction.
*/
TestResult test_GameTie() {
    // Use a minimal grid size to limit the number of moves that need to be played
    Grid* grid = new Grid(4, 4);
    Game game;
    game.setGrid(grid);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    
    // Play turns; 4 per row, alternating groups of two rows at a time to make sure we don't trigger a win
    for (int i = 0; i < 2; ++i) {
        ASSERT(game.playNextTurn(0));
        ASSERT(game.playNextTurn(2));
        ASSERT(game.playNextTurn(1));
        ASSERT(game.playNextTurn(3));
        ASSERT(game.playNextTurn(2));
        ASSERT(game.playNextTurn(0));
        ASSERT(game.playNextTurn(3));
        ASSERT(game.playNextTurn(1));
    }
    
    /*for(int i = 0; i < 4; i++){
    	for (int j = 0; j < 4; j++){
    		cout << grid->cellAt(i,j) << " | ";
    	}
    	cout << endl;
    }
	*/////////PRINT VECTOR


    // Check grid state
    std::string gridState = "2211"
                            "1122"
                            "2211"
                            "1122";
    ASSERT(verifyGridState(*grid, gridState));
    
    // Check game status is complete and no more turns can be played
    ASSERT(game.status() == Game::GS_COMPLETE);
    ASSERT(!game.playNextTurn(0));
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == 0);
    ASSERT(p2.getScore() == 0);
    ASSERT(p2.getWins() == 0);
    ASSERT(p1.getScore() == 0);
    ASSERT(p1.getWins() == 0);
    
    return TR_PASS; //test20
}

/*
Test player one winning via a horizontal connect four.
*/
TestResult test_GameWinP1H() {
    Grid* grid = new Grid(6, 7);
    Game game;
    game.setGrid(grid);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    
    // Play turns
    // p1 = x, p2 = o
    //
    //  0 1 2 3 4 5 6
    // | | | | | | | |
    // | | | | | | | |
    // | | | | | | | |
    // | | | | | | | |
    // | |x|x|x|x|o| |
    // |x|o|o|x|o|o| |
    ASSERT(game.playNextTurn(0)); // p1
    ASSERT(game.playNextTurn(1)); // p2
    ASSERT(game.playNextTurn(1)); // p1
    ASSERT(game.playNextTurn(2)); // p2
    ASSERT(game.playNextTurn(2)); // p1
    ASSERT(game.playNextTurn(5)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(5)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(4)); // p2
    ASSERT(game.playNextTurn(4)); // p1
    
    // Check grid state
    std::string gridState = "       "
                            "       "
                            "       "
                            "       "
                            " 11112 "
                            "122122 ";
    ASSERT(verifyGridState(*grid, gridState));
    
    // Check game status is complete and no more turns can be played
    ASSERT(game.status() == Game::GS_COMPLETE);
    ASSERT(!game.playNextTurn(0));
    ASSERT(game.winner() == &p1);
    ASSERT(game.nextPlayer() == 0);
    ASSERT(p2.getScore() == 0);
    ASSERT(p2.getWins() == 0);
    ASSERT(p1.getScore() == 1);
    ASSERT(p1.getWins() == 1);
    
    return TR_PASS; //test21
}

/*
Test player one winning via a vertical connect four.
*/
TestResult test_GameWinP1V() {
    Grid* grid = new Grid(6, 7);
    Game game;
    game.setGrid(grid);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    
    // Play turns
    // p1 = x, p2 = o
    //
    //  0 1 2 3 4 5 6
    // | | | | | | | |
    // | | | | | | | |
    // | | | |x| | | |
    // | | | |x| | | |
    // | |x|o|x|x|o| |
    // |x|o|o|x|o|o| |
    ASSERT(game.playNextTurn(0)); // p1
    ASSERT(game.playNextTurn(1)); // p2
    ASSERT(game.playNextTurn(1)); // p1
    ASSERT(game.playNextTurn(2)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(5)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(5)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(4)); // p2
    ASSERT(game.playNextTurn(4)); // p1
    ASSERT(game.playNextTurn(2)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    
    // Check grid state
    std::string gridState = "       "
                            "       "
                            "   1   "
                            "   1   "
                            " 12112 "
                            "122122 ";
    ASSERT(verifyGridState(*grid, gridState));
    
    // Check game status is complete and no more turns can be played
    ASSERT(game.status() == Game::GS_COMPLETE);
    ASSERT(!game.playNextTurn(0));
    ASSERT(game.winner() == &p1);
    ASSERT(game.nextPlayer() == 0);
    ASSERT(p2.getScore() == 0);
    ASSERT(p2.getWins() == 0);
    ASSERT(p1.getScore() == 1);
    ASSERT(p1.getWins() == 1);
    
    return TR_PASS; //test22
}

/*
Test player scoring: each player's score should increase when they win a game, and both scores should be reset when the
Game's restart method is called.
*/
TestResult test_GameScore() {
    Grid* grid = new Grid(6, 7);
    Game game;
    game.setGrid(grid);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    
    // Generate a quick player one win
    unsigned int t = 0;
    while (game.status() == Game::GS_IN_PROGRESS) {
        if (t % 2 == 0) {
            // P1 plays in column 0
            ASSERT(game.playNextTurn(0));
        } else {
            // P2 plays in column 6
            ASSERT(game.playNextTurn(6));
        }
        ++t;
    }
    // Check grid state
    std::string gridState = "       "
                            "       "
                            "1      "
                            "1     2"
                            "1     2"
                            "1     2";
    ASSERT(verifyGridState(*grid, gridState));
    
    ASSERT(game.status() == Game::GS_COMPLETE);
    ASSERT(game.winner() == &p1);
    ASSERT(p1.getScore() == 1);
    ASSERT(p2.getScore() == 0);
    game.restart();
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.winner() == 0);
    ASSERT(p1.getScore() == 0);
    ASSERT(p2.getScore() == 0);
    
    // Generate a quick win for P2
    t = 0;
    while (game.status() == Game::GS_IN_PROGRESS) {
        if (t % 4 == 0) {
            // P1 plays in column 0
            ASSERT(game.playNextTurn(0));
        } else if (t % 2 == 0) {
            // P1 plays in column 1
            ASSERT(game.playNextTurn(1));
        } else {
            // P2 plays in column 6
            ASSERT(game.playNextTurn(6));
        }
        ++t;
    }
    // Check grid state
    gridState = "       "
                "       "
                "      2"
                "      2"
                "11    2"
                "11    2";
    ASSERT(verifyGridState(*grid, gridState));
    
    ASSERT(game.status() == Game::GS_COMPLETE);
    ASSERT(game.winner() == &p2);
    ASSERT(p1.getScore() == 0);
    ASSERT(p2.getScore() == 1);
    game.restart();
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.winner() == 0);
    ASSERT(p1.getScore() == 0);
    ASSERT(p2.getScore() == 0);
    
    // Check wins have been accumulating
    ASSERT(p1.getWins() == 1)
    ASSERT(p2.getWins() == 1)
    
    return TR_PASS; //test23
}

// Test for Figure 1 example (by Hamish O'Neill)
TestResult test_fig1() {
    Grid* grid = new Grid(6, 7);
    Game game;
    game.setGrid(grid);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);

    // Play turns
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(2)); // p2
    ASSERT(game.playNextTurn(4)); // p1
    ASSERT(game.playNextTurn(3)); // p2
    ASSERT(game.playNextTurn(4)); // p1
    ASSERT(game.playNextTurn(3)); // p2
    ASSERT(game.playNextTurn(5)); // p1
    ASSERT(game.playNextTurn(4)); // p2
    ASSERT(game.playNextTurn(6)); // p1

    // Check grid state
    std::string gridState = "       "
            "       "
            "       "
            "   22  "
            "   21  "
            "  21111";
    ASSERT(verifyGridState(*grid, gridState));

    // Check game status is complete and no more turns can be played
    ASSERT(game.status() == Game::GS_COMPLETE);
    ASSERT(!game.playNextTurn(0));
    ASSERT(game.winner() == &p1);
    ASSERT(game.nextPlayer() == 0);
    ASSERT(p2.getScore() == 0);
    ASSERT(p2.getWins() == 0);
    ASSERT(p1.getScore() == 1);
    ASSERT(p1.getWins() == 1);

    return TR_PASS; //test24
}

//extra test by jgud007 testing a diagonal down connect 4
TestResult test_DiagDown() {
    Grid* grid = new Grid(4, 4);
    Game game;
    game.setGrid(grid);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);

    // Generate a quick player one win
    ASSERT(game.playNextTurn(0)); //p1 col 0
    ASSERT(game.playNextTurn(3)); //p2 col 3
    ASSERT(game.playNextTurn(1)); //p1 col 1
    ASSERT(game.playNextTurn(2)); //p2 col 2
    ASSERT(game.playNextTurn(0)); //p1 col 0
    ASSERT(game.playNextTurn(3)); //p2 col 3
    ASSERT(game.playNextTurn(1)); //p1 col 1
    ASSERT(game.playNextTurn(2)); //p2 col 2
    ASSERT(game.playNextTurn(2)); //p1 col 2
    ASSERT(game.playNextTurn(0)); //p2 col 0
    ASSERT(game.playNextTurn(3)); //p1 col 3
    ASSERT(game.playNextTurn(1)); //p2 col 1
    ASSERT(game.playNextTurn(2)); //p1 col 2
    ASSERT(game.playNextTurn(0)); //p2 col 0

    // Check grid state
    std::string gridState = "2 1 "
                            "2211"
                            "1122"
                            "1122";

    ASSERT(verifyGridState(*grid, gridState));

    ASSERT(game.status() == Game::GS_COMPLETE);
    ASSERT(game.winner() == &p2);
    ASSERT(p1.getScore() == 0);
    ASSERT(p2.getScore() == 1);

    return TR_PASS; //test25

}

//extra test by jgud007 testing a diagonal up connect 4
TestResult test_DiagUp() {
    Grid* grid = new Grid(4, 4);
    Game game;
    game.setGrid(grid);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);

    // Generate a quick player one win
    ASSERT(game.playNextTurn(0)); //p1 col 0
    ASSERT(game.playNextTurn(3)); //p2 col 3
    ASSERT(game.playNextTurn(1)); //p1 col 1
    ASSERT(game.playNextTurn(2)); //p2 col 2
    ASSERT(game.playNextTurn(0)); //p1 col 0
    ASSERT(game.playNextTurn(3)); //p2 col 3
    ASSERT(game.playNextTurn(1)); //p1 col 1
    ASSERT(game.playNextTurn(2)); //p2 col 2
    ASSERT(game.playNextTurn(2)); //p1 col 2
    ASSERT(game.playNextTurn(0)); //p2 col 0
    ASSERT(game.playNextTurn(3)); //p1 col 3
    ASSERT(game.playNextTurn(1)); //p2 col 1
    ASSERT(game.playNextTurn(3)); //p1 col 3


    // Check grid state
    std::string gridState = "   1"
                            "2211"
                            "1122"
                            "1122";

    ASSERT(verifyGridState(*grid, gridState));

    ASSERT(game.status() == Game::GS_COMPLETE);
    ASSERT(game.winner() == &p1);
    ASSERT(p1.getScore() == 1);
    ASSERT(p2.getScore() == 0);

    return TR_PASS; //test26

}

#endif /*ENABLE_T3_TESTS*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_T4_TESTS
/*
Test a simple single connect four for player1 in a SuperGame with no propogation of deleted cells required.
*/
TestResult test_SuperGameMoveSimple() {
    Grid* grid = new Grid(6, 7);
    SuperGame game;
    game.setGrid(grid);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    
    // Play turns
    // p1 = x, p2 = o
    //
    //  0 1 2 3 4 5 6
    // | | | | | | | |
    // | | | | | | | |
    // | | | | | | | |
    // | | | | | | | |
    // | |x|x|x|x|o| |
    // |x|o|o|x|o|o| |
    ASSERT(game.playNextTurn(0)); // p1
    ASSERT(game.playNextTurn(1)); // p2
    ASSERT(game.playNextTurn(1)); // p1
    ASSERT(game.playNextTurn(2)); // p2
    ASSERT(game.playNextTurn(2)); // p1
    ASSERT(game.playNextTurn(5)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(5)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(4)); // p2
    ASSERT(game.playNextTurn(4)); // p1
    
    // This example doesn't check the initial state so it's a little easier to pass
    std::string gridStateFinal = "       "
                                 "       "
                                 "       "
                                 "       "
                                 "     2 "
                                 "122122 ";
    ASSERT(verifyGridState(*grid, gridStateFinal))
    
    // Check game status is in progress and the player scores have been adjusted appropriately
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == &p2);
    ASSERT(p2.getScore() == 0);
    ASSERT(p1.getScore() == 1);
    
    return TR_PASS;//test27
}

/*
Test the first example from the assignment brief.
*/
TestResult test_SuperGameMoveExample1() {
    Grid* grid = new Grid(6, 7);
    SuperGame game;
    game.setGrid(grid);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.nextPlayer() == &p1);
    
    // Play turns
    // p1 = x, p2 = o
    //
    //  0 1 2 3 4 5 6
    // | | | | | | | |
    // | | | |x| | | |
    // | | |x|x|x| | |
    // | |o|o|o|o| | |
    // |o|o|o|x|o|x|o|
    // |x|x|o|x|o|x|x|
    ASSERT(game.playNextTurn(0)); // p1
    ASSERT(game.playNextTurn(2)); // p2
    ASSERT(game.playNextTurn(1)); // p1
    ASSERT(game.playNextTurn(4)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(2)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(0)); // p2
    ASSERT(game.playNextTurn(5)); // p1
    ASSERT(game.playNextTurn(1)); // p2
    ASSERT(game.playNextTurn(6)); // p1
    ASSERT(game.playNextTurn(3)); // p2
    ASSERT(game.playNextTurn(5)); // p1
    ASSERT(game.playNextTurn(4)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(2)); // p2
    ASSERT(game.playNextTurn(2)); // p1
    ASSERT(game.playNextTurn(4)); // p2
    ASSERT(game.playNextTurn(4)); // p1
    ASSERT(game.playNextTurn(6)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    
    std::string gridStateInitial = "       "
                                   "   1   "
                                   "  111  "
                                   "  222  "
                                   "2221212"
                                   "1121211";
    // Check initial grid state
    ASSERT(verifyGridState(*grid, gridStateInitial))
    
    // play winning move
    ASSERT(game.playNextTurn(1)); // p2
    
    // Check post-connection state
    //
    //  0 1 2 3 4 5 6
    // | | | | | | | |
    // | | | | | | | |
    // | | | | | | | |
    // | | |x| | | | |
    // |o|o|o| |o| | |
    // |x|x|o| |o|x|o|
    std::string gridStateFinal = "       "
                                 "       "
                                 "       "
                                 "  1    "
                                 "222 2  "
                                 "112 212";
    ASSERT(verifyGridState(*grid, gridStateFinal));
    
    // Check game status is in progress and the player scores have been adjusted appropriately
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == &p1);
    ASSERT(p2.getScore() == 1);
    ASSERT(p2.getWins() == 0);
    ASSERT(p1.getScore() == 1);
    ASSERT(p1.getWins() == 0);
    
    return TR_PASS; //test28
}

// Test for Figure 4 example (by Hamish O'Neill)
TestResult test_fig4() {

    Grid* grid = new Grid(6, 7);
    SuperGame game;
    game.setGrid(grid);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.nextPlayer() == &p1);

    // Play turns
    ASSERT(game.playNextTurn(1)); // p1
    ASSERT(game.playNextTurn(0)); // p2
    ASSERT(game.playNextTurn(2)); // p1
    ASSERT(game.playNextTurn(3)); // p2
    ASSERT(game.playNextTurn(1)); // p1
    ASSERT(game.playNextTurn(0)); // p2
    ASSERT(game.playNextTurn(2)); // p1
    ASSERT(game.playNextTurn(4)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(4)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(2)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(1)); // p2
    ASSERT(game.playNextTurn(1)); // p1
    ASSERT(game.playNextTurn(4)); // p2
    ASSERT(game.playNextTurn(2)); // p1
    ASSERT(game.playNextTurn(3)); // p2

    std::string gridStateInitial = "       "
            "   2   "
            " 111   "
            " 2212  "
            "21112  "
            "21122  ";
    // Check initial grid state
    ASSERT(verifyGridState(*grid, gridStateInitial))

    // play winning move
    ASSERT(game.playNextTurn(4)); // p1

    // Check post-connection state
    std::string gridStateFinal = "       "
            "       "
            "       "
            "   22  "
            "22212  "
            "21122  ";
    ASSERT(verifyGridState(*grid, gridStateFinal));

    // Check game status is in progress and the player scores have been adjusted appropriately
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == &p2);
    ASSERT(p2.getScore() == 0);
    ASSERT(p2.getWins() == 0);
    ASSERT(p1.getScore() == 1);
    ASSERT(p1.getWins() == 0);

    return TR_PASS; //29
}

// Test for Figure 5 example (by Hamish O'Neill)
TestResult test_fig5() {

    Grid* grid = new Grid(6, 7);
    SuperGame game;
    game.setGrid(grid);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.nextPlayer() == &p1);

    // Play turns
    ASSERT(game.playNextTurn(0)); // p1
    ASSERT(game.playNextTurn(4)); // p2
    ASSERT(game.playNextTurn(2)); // p1
    ASSERT(game.playNextTurn(2)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(6)); // p2
    ASSERT(game.playNextTurn(4)); // p1
    ASSERT(game.playNextTurn(5)); // p2
    ASSERT(game.playNextTurn(5)); // p1
    ASSERT(game.playNextTurn(6)); // p2
    ASSERT(game.playNextTurn(2)); // p1
    ASSERT(game.playNextTurn(3)); // p2
    ASSERT(game.playNextTurn(3)); // p1
    ASSERT(game.playNextTurn(6)); // p2

    std::string gridStateInitial = "       "
            "       "
            "       "
            "  11  2"
            "  22112"
            "1 11222";
    // Check initial grid state
    ASSERT(verifyGridState(*grid, gridStateInitial))

    // play winning move
    ASSERT(game.playNextTurn(1)); // p1

    // Check post-connection state
    std::string gridStateFinal = "       "
            "       "
            "       "
            "       "
            "      2"
            "      2";
    ASSERT(verifyGridState(*grid, gridStateFinal));

    // Check game status is in progress and the player scores have been adjusted appropriately
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == &p2);
    ASSERT(p2.getScore() == 1);
    ASSERT(p2.getWins() == 0);
    ASSERT(p1.getScore() == 2);
    ASSERT(p1.getWins() == 0);

    return TR_PASS; //30
}


// Test a game where the grid is full and player 2 wins because of a higher score (by Hamish O'Neill)
TestResult test_SuperGameWin() {
    // Use a minimal grid size to limit the number of moves that need to be played
    Grid* grid = new Grid(4, 4);
    SuperGame game;
    game.setGrid(grid);
    Player p1("Nick");
    game.setPlayerOne(&p1);
    Player p2("Nasser");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);

    // Give player 2 a higher score
    p1.increaseScore();
    p2.increaseScore();
    p2.increaseScore();

    // Play turns; 4 per row, alternating groups of two rows at a time to make sure we don't trigger a win
    for (int i = 0; i < 2; ++i) {
        ASSERT(game.playNextTurn(0));
        ASSERT(game.playNextTurn(2));
        ASSERT(game.playNextTurn(1));
        ASSERT(game.playNextTurn(3));
        ASSERT(game.playNextTurn(2));
        ASSERT(game.playNextTurn(0));
        ASSERT(game.playNextTurn(3));
        ASSERT(game.playNextTurn(1));
    }

    // Check grid state
    std::string gridState = "2211"
            "1122"
            "2211"
            "1122";
    ASSERT(verifyGridState(*grid, gridState));

    // Check game status is complete and no more turns can be played
    ASSERT(game.status() == Game::GS_COMPLETE);
    ASSERT(!game.playNextTurn(0));
    ASSERT(game.winner() == &p2);
    ASSERT(game.nextPlayer() == 0);
    ASSERT(p2.getScore() == 2);
    ASSERT(p2.getWins() == 1);
    ASSERT(p1.getScore() == 1);
    ASSERT(p1.getWins() == 0);

    return TR_PASS; //31
}

// Test a stalemate in a Super Game. (by Nathan McDougall, based on code by Hamish O'Neill)
TestResult test_SuperGameStalemate() {
    // 4x4 grid
    Grid* grid = new Grid(4, 4);
    SuperGame game;
    game.setGrid(grid);
    Player p1("Jeff");
    game.setPlayerOne(&p1);
    Player p2("Olivia");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);

    // Hamish's elegant way of filling the Grid
    for (int i = 0; i < 2; ++i) {
        ASSERT(game.playNextTurn(0));
        ASSERT(game.playNextTurn(2));
        ASSERT(game.playNextTurn(1));
        ASSERT(game.playNextTurn(3));
        ASSERT(game.playNextTurn(2));
        ASSERT(game.playNextTurn(0));
        ASSERT(game.playNextTurn(3));
        ASSERT(game.playNextTurn(1));
    }

    // Check grid state --- by Hamish
    std::string gridState = "2211"
            "1122"
            "2211"
            "1122";
    ASSERT(verifyGridState(*grid, gridState));

    // Check everything is as if nothing had happened.
    ASSERT(game.status() == Game::GS_COMPLETE);
    ASSERT(!game.playNextTurn(0));
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == 0);
    ASSERT(p2.getScore() == 0);
    ASSERT(p2.getWins() == 0);
    ASSERT(p1.getScore() == 0);
    ASSERT(p1.getWins() == 0);

    return TR_PASS; //32
}

// Test a larger game (by Nathan McDougall)
TestResult test_SuperGameMedium() {
    // 10x10 grid
    Grid* grid = new Grid(10, 10);
    SuperGame game;
    game.setGrid(grid);
    Player p1("Hillary");
    game.setPlayerOne(&p1);
    Player p2("Donald");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);

    // Just two stacks side by side, each should get a point out of it.
    for (unsigned int i = 1; i <= 4; ++i) {
        ASSERT(game.playNextTurn(0)); //p1
        ASSERT(game.playNextTurn(1)); //p2
    }

    // Check grid state is empty.
    std::string gridState = "          "
                            "          "
                            "          "
                            "          "
                            "          "
                            "          "
                            "          "
                            "          "
                            "          "
                            "          ";
    ASSERT(verifyGridState(*grid, gridState));

    // Check the applicable points have been awarded and that nothing funny is going on.
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == &p1);
    ASSERT(p1.getScore() == 1);
    ASSERT(p1.getWins() == 0);
    ASSERT(p2.getScore() == 1);
    ASSERT(p2.getWins() == 0);

    // Up-scaled version of the princple of figure 5.
    for (unsigned int i = 1; i <= 3 ; ++i) {
        ASSERT(game.playNextTurn(i)); //p1
        ASSERT(game.playNextTurn(9)); //p2
    }

    // Junk move to make it player 2's turn.
    ASSERT(game.playNextTurn(9)); //p1

    //Check that everything is okay in the intermediary.
    gridState = "          "
                "          "
                "          "
                "          "
                "          "
                "          "
                "         1"
                "         2"
                "         2"
                " 111     2";
    ASSERT(verifyGridState(*grid, gridState));

    // Continue stacking so that a collapse is set up like in figure 5.
    for (unsigned int i = 1; i <= 4 ; ++i) {
        for (unsigned int j = 2; j <= 5 ; ++j) {
            ASSERT(game.playNextTurn(j)); //p2
            ASSERT(game.playNextTurn(j)); //p1
        }
    }

    // Check the stack is okay.
    gridState = "          "
                "  11      "
                "  2211    "
                "  1122    "
                "  2211    "
                "  1122    "
                "  2211   1"
                "  1122   2"
                "  2211   2"
                " 11122   2";
    ASSERT(verifyGridState(*grid, gridState));

    // Get p2 to make a connect 4 by placing in columns 6 and 7.
    // Get p1 to meanwhile finish up in column 9 and prepare a win there.
    for (unsigned int i = 6; i <= 7 ; ++i) {
            ASSERT(game.playNextTurn(i)); //p2
            ASSERT(game.playNextTurn(9)); //p1
    }

    // Check the stack has collapsed successfully
    gridState = "          "
                "          "
                "          "
                "          "
                "         1"
                "         1"
                "         1"
                "         2"
                "  11     2"
                "  22     2";
    ASSERT(verifyGridState(*grid, gridState));

    // Check the applicable points have been awarded and that nothing funny is going on.
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == &p2);
    ASSERT(p1.getScore() == 2);
    ASSERT(p1.getWins() == 0);
    ASSERT(p2.getScore() == 3);
    ASSERT(p2.getWins() == 0);

    // Make a connect 7 for both players.
    // Also, finish up the two sets of 3 in column 9, emptying the grid.
    ASSERT(game.playNextTurn(1)); //p2
    ASSERT(game.playNextTurn(9)); //p1
    ASSERT(game.playNextTurn(5)); //p2
    ASSERT(game.playNextTurn(1)); //p1
    ASSERT(game.playNextTurn(6)); //p2
    ASSERT(game.playNextTurn(6)); //p1
    ASSERT(game.playNextTurn(7)); //p2
    ASSERT(game.playNextTurn(7)); //p1
    ASSERT(game.playNextTurn(4)); //p2
    ASSERT(game.playNextTurn(5)); //p1
    ASSERT(game.playNextTurn(9)); //p2
    ASSERT(game.playNextTurn(4)); //p1

    // Check the stack has collapsed successfully
    gridState = "          "
                "          "
                "          "
                "          "
                "          "
                "          "
                "          "
                "          "
                "          "
                "          ";
    ASSERT(verifyGridState(*grid, gridState));

    // Check the applicable points have been awarded and that nothing funny is going on.
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == &p2);
    ASSERT(p1.getScore() == 4);
    ASSERT(p1.getWins() == 0);
    ASSERT(p2.getScore() == 5);
    ASSERT(p2.getWins() == 0);

    // Fill up a column, and check the playNextTurn is false when we try further.
    for (unsigned int i = 1; i <= 5; ++i) {
        ASSERT(game.playNextTurn(1));
        ASSERT(game.playNextTurn(1));
    }
    ASSERT(!game.playNextTurn(1));
    ASSERT(!game.playNextTurn(1));

    // Junk move to make it player 1's turn.
    ASSERT(game.playNextTurn(9)); //p2

    // Fill up every other column.
    for (unsigned int i = 0; i <= 8; i += 2) {
        for (unsigned int j = 1; j <= 5; ++j) {
            ASSERT(game.playNextTurn(i));
            ASSERT(game.playNextTurn(i));
        }
    }

    // Junk move to make it player 2's turn.
    ASSERT(game.playNextTurn(9)); //p1

    // Fill up every other column.
    for (unsigned int i = 3; i <= 7; i += 2) {
        for (unsigned int j = 1; j <= 5; ++j) {
            ASSERT(game.playNextTurn(i));
            ASSERT(game.playNextTurn(i));
        }
    }

    // Get some collapsing from the diagonals going on: check all is well.
    gridState = "        2 "
                "       11 "
                "    21222 "
                "    12111 "
                "    21222 "
                "    12211 "
                "    22122 "
                "11  11211 "
                "2122211221"
                "1211121112";
    ASSERT(verifyGridState(*grid, gridState));

    // Check the applicable points have been awarded and that nothing funny is going on.
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == &p2);
    ASSERT(p1.getScore() == 8);
    ASSERT(p1.getWins() == 0);
    ASSERT(p2.getScore() == 10);
    ASSERT(p2.getWins() == 0);

    return TR_PASS; //33
}


// Test on a very large grid (by Nathan McDougall)
TestResult test_SuperGameBig() {
    // 4x4 grid
    Grid* grid = new Grid(1000, 1000);
    SuperGame game;
    game.setGrid(grid);
    Player p1("Jian");
    game.setPlayerOne(&p1);
    Player p2("Tess");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);

    // Play two competeing towers
    for (int i = 1; i <= 4; ++i) {
        ASSERT(game.playNextTurn(768)); //p1
        ASSERT(game.playNextTurn(244)); //p2
    }

    // Check the grid is empty.
    std::string gridState = " ";
    for (int i = 1; i < 1000000; ++i) {
        gridState += " ";
    }
    ASSERT(verifyGridState(*grid, gridState));

    // Check everything.
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == &p1);
    ASSERT(p2.getScore() == 1);
    ASSERT(p2.getWins() == 0);
    ASSERT(p1.getScore() == 1);
    ASSERT(p1.getWins() == 0);

    return TR_PASS; //34 time consuming
}

// Test a draw by equal points. (by Nathan McDougall, based on code by Hamish O'Neill)
TestResult test_SuperGamePointDraw() {
    // 4x4 grid
    Grid* grid = new Grid(4, 4);
    SuperGame game;
    game.setGrid(grid);
    Player p1("Eric");
    game.setPlayerOne(&p1);
    Player p2("Jean-Marie");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);

    // Play two competeing towers
    for (int i = 1; i <= 4; ++i) {
        ASSERT(game.playNextTurn(1)); //p1
        ASSERT(game.playNextTurn(2)); //p2
    }

    // Check the grid is empty.
    std::string gridState = " ";
    for (int i = 1; i < 16; ++i) {
        gridState += " ";
    }
    ASSERT(verifyGridState(*grid, gridState));

    // Check everything.
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == &p1);
    ASSERT(p2.getScore() == 1);
    ASSERT(p2.getWins() == 0);
    ASSERT(p1.getScore() == 1);
    ASSERT(p1.getWins() == 0);

    // Hamish's elegant way of filling the Grid
    for (int i = 0; i < 2; ++i) {
        ASSERT(game.playNextTurn(0));
        ASSERT(game.playNextTurn(2));
        ASSERT(game.playNextTurn(1));
        ASSERT(game.playNextTurn(3));
        ASSERT(game.playNextTurn(2));
        ASSERT(game.playNextTurn(0));
        ASSERT(game.playNextTurn(3));
        ASSERT(game.playNextTurn(1));
    }

    // Check grid state --- by Hamish
    gridState = "2211"
                "1122"
                "2211"
                "1122";
    ASSERT(verifyGridState(*grid, gridState));

    // Check that the game was drawn correctly.
    ASSERT(game.status() == Game::GS_COMPLETE);
    ASSERT(!game.playNextTurn(0));
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == 0);
    ASSERT(p2.getScore() == 1);
    ASSERT(p2.getWins() == 0);
    ASSERT(p1.getScore() == 1);
    ASSERT(p1.getWins() == 0);

    return TR_PASS;
}

// Test a game where players are swapped mid-game. (by Nathan McDougall, based on code by Hamish O'Neill)
TestResult test_SuperGamePlayerSwap() {
    // 4x64 grid
    Grid* grid = new Grid(4, 64);
    SuperGame game;
    game.setGrid(grid);
    Player p1("Abishek");
    Player p1new("Bob");
    game.setPlayerOne(&p1);
    Player p2("Elanor");
    Player p2new("Cynthia");
    game.setPlayerTwo(&p2);
    ASSERT(game.status() == Game::GS_IN_PROGRESS);

    // Play two competeing towers
    for (int i = 1; i <= 4; ++i) {
        ASSERT(game.playNextTurn(35)); //p1
        ASSERT(game.playNextTurn(20)); //p2
    }

    // Check the grid is empty.
    std::string gridState = " ";
    for (int i = 1; i < 256; ++i) {
        gridState += " ";
    }
    ASSERT(verifyGridState(*grid, gridState));

    // Swap around the players
    game.setPlayerOne(&p1new);
    game.setPlayerTwo(&p2new);

    // Play two competeing towers
    for (int i = 1; i <= 4; ++i) {
        ASSERT(game.playNextTurn(17)); //p1
        ASSERT(game.playNextTurn(59)); //p2
    }

    // Check the grid is empty.
    ASSERT(verifyGridState(*grid, gridState));

    // Check everything.
    ASSERT(game.status() == Game::GS_IN_PROGRESS);
    ASSERT(game.winner() == 0);
    ASSERT(game.nextPlayer() == &p1new);
    ASSERT(p2.getScore() == 1);
    ASSERT(p2new.getScore() == 1);
    ASSERT(p2.getWins() == 0);
    ASSERT(p2new.getWins() == 0);
    ASSERT(p1.getScore() == 1);
    ASSERT(p1new.getScore() == 1);
    ASSERT(p1.getWins() == 0);
    ASSERT(p1new.getWins() == 0);

    return TR_PASS;
}


#endif /*ENABLE_T4_TESTS*/

/*
This function collects up all the tests as a vector of function pointers. If you create your own
tests and want to be able to run them, make sure you add them to the `tests` vector here.
*/
vector<TestResult (*)()> generateTests() {
    vector<TestResult (*)()> tests;
#ifdef ENABLE_T1_TESTS
    tests.push_back(&test_PlayerConstruction);
    tests.push_back(&test_PlayerName);
    tests.push_back(&test_PlayerScore);
    tests.push_back(&test_PlayerIncreaseScore);
    tests.push_back(&test_PlayerResetScore);
    tests.push_back(&test_PlayerWins);
#endif /*ENABLE_T1_TESTS*/
#ifdef ENABLE_T2_TESTS
    tests.push_back(&test_GridConstruction);
    tests.push_back(&test_GridSizeValid);
    tests.push_back(&test_GridSizeInvalid);
    tests.push_back(&test_GridAccessEmpty);
    tests.push_back(&test_GridInsertDisc);
    tests.push_back(&test_GridInsertFull);
    tests.push_back(&test_GridReset);
#endif /*ENABLE_T2_TESTS*/
#ifdef ENABLE_T3_TESTS
    tests.push_back(&test_GameConstruction);
    tests.push_back(&test_GameInitialState);
    tests.push_back(&test_GameSetGrid);
    tests.push_back(&test_GameSetPlayers);
    tests.push_back(&test_GameFirstMove);
    tests.push_back(&test_GameRestartCompleted);
    tests.push_back(&test_GameFullColumn);
    tests.push_back(&test_GameTie);
    tests.push_back(&test_GameWinP1H);
    tests.push_back(&test_GameWinP1V);
    tests.push_back(&test_GameScore);
    tests.push_back(&test_fig1);
    tests.push_back(&test_DiagDown);
    tests.push_back(&test_DiagUp);
#endif /*ENABLE_T3_TESTS*/
#ifdef ENABLE_T4_TESTS
    tests.push_back(&test_SuperGameMoveSimple);
    tests.push_back(&test_SuperGameMoveExample1);
    tests.push_back(&test_fig4);
    tests.push_back(&test_fig5);
    tests.push_back(&test_SuperGameWin);
    tests.push_back(&test_SuperGameStalemate);
    tests.push_back(&test_SuperGameMedium);
   // tests.push_back(&test_SuperGameBig);
    tests.push_back(&test_SuperGamePointDraw);
    tests.push_back(&test_SuperGamePlayerSwap);
#endif /*ENABLE_T4_TESTS*/

    return tests;
}

int main(int argc, char const* argv[]) {
    // If one or more test numbers are passed as command-line parameters, execute them
    vector<TestResult (*)()> tests_to_run = generateTests();
    unsigned int pass_count = 0;
    unsigned int fail_count = 0;
    unsigned int run_count = 0;
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            unsigned int test_num = atoi(argv[i]);
            if (test_num > tests_to_run.size()) {
                cout << "ERROR: unknown test case " << test_num << endl;
                continue;
            }
            // run the test
            ++run_count;
            TestResult result = tests_to_run[test_num]();
            if (result == TR_FAIL) {
                cout << "FAIL: Test " << test_num << " failed." << endl;
                ++fail_count;
            } else {
                cout << "PASS: Test " << test_num << " passed." << endl;
                ++pass_count;
            }
        }
    } else {
        // otherwise, run all tests
        // Make sure to update this section as you add more tests
        // Add the tests for the first task
        for (unsigned int t = 0; t < tests_to_run.size(); ++t) {
            ++run_count;
            TestResult result = tests_to_run[t]();
            if (result == TR_FAIL) {
                cout << "FAIL: Test " << t << " failed." << endl;
                ++fail_count;
            } else {
                cout << "PASS: Test " << t << " passed." << endl;
                ++pass_count;
            }
        }
    }
    cout << "\n-------------------------------------------------\n"
         << "Total Run: " << run_count << "\n"
         << "Total PASSED: " << pass_count << "\n"
         << "Total FAILED: " << fail_count << "\n"
         << "-------------------------------------------------\n" << endl;
    
    // return 99 if any test cases failed
    if (fail_count > 0) {
        return 99;
    }

    // Return 0 if everything was ok
    return 0;
}
