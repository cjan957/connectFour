#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <iostream>

/*
 The Player class holds information about a specific player playing Connect Four, including their name, score and total
 number of wins.
 */
class Player {
public:
	/*
	 Create a new Player with the given name.
	 */
	Player(const std::string& name);

	/*
	 Get this Player's name.
	 */
	std::string getName() const;

	/*
	 Get this Player's current score.
	 */
	unsigned int getScore() const;

	/*
	 Reset this Player's score to 0.
	 */
	void resetScore();

	/*
	 Increase this Player's score by 1.
	 */
	void increaseScore();

	/*
	 Get this Player's win count.
	 */
	unsigned int getWins() const;
	/*
	 Increase this Player's win count by 1.
	 */
	void increaseWins();

private:
	std::string playerName;
	int winsCount;
	int scoreCount;

	/*
	 Private Player copy constructor - players cannot be copied, must be passed around via pointers and references.
	 */
	Player(const Player&) {
	}
	;

	/*
	 Private Player copy assignment operator - players cannot be copied, must be passed around via pointers and
	 references.
	 */
	Player& operator=(const Player&) {
		return *this;
	}
	;
};

#endif /* end of include guard: PLAYER_HPP */
