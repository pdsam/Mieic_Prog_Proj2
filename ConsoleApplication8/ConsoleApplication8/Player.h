#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include <string>
#include <time.h>
#include <iostream>
#include <fstream>

class Player
{
public:
	Player();
	void setTime(time_t time); //update the time
	void saveGame(std::string boardName); //save the player information at the end of game
	time_t timeGame(time_t t1, time_t t2); //calculates duration of the game
	void addClues(); //updates the clues of the player

private:
	time_t t;
	std::string namePlayer;
	int clues;
	int numberFile;

};


#endif