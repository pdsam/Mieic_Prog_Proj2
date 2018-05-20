#include "Player.h"

using namespace std;

Player::Player()
{
	string name;

	cout << "Name of the player:" << endl;
	cin >> name;

	namePlayer = name;
	clues = 0;
	t = time(unsigned(NULL));
	numberFile = 001;

}
//////////////////////////////////////////////////
void Player::addClues()
//updates clues each time the player asks for help
{
	clues = clues++;
}
//////////////////////////////////////////////////
void Player::saveGame(string boardName)
//saves the player information in a text file
{
	//file: text file to save the information to
	ofstream file;
	file.open((boardName + "_p.txt").c_str());

	file << "Time of game: " << t << "--" << "Player: " << namePlayer << "--" << "Number of clues given: " << clues << "--" << "File n." << numberFile << endl;

	file.close();//close the file
}
//////////////////////////////////////////////////
void Player::setTime(time_t time)
//update the time of game
{
	t = time;
}
//////////////////////////////////////////////////
time_t Player::timeGame(time_t t1, time_t t2)
//calculates duration of the game
{
	return t2 - t1;
}