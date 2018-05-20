
#include "stdafx.h"
#include "Board.h"
#include "Dictionary.h"
#include "Player.h"

using namespace std;


void Uppers(string &s)
//converts all caracters to uppercases
{
	for (unsigned int i = 0; i < s.size(); i++)
	{
		if (islower(s.at(i)))
		{
			s.at(i) = toupper(s.at(i));

		}

	}

}

void Begining(int &a)
{
	cout << "CROSSWORDS PLAYER" << endl;
	cout << "=========================================" << endl << endl;
	cout << "INSTRUCTIONS:" << endl << endl;
	cout << "POSITION (LCD /CNTRL-Z = stop)" << endl << endl;
	cout << "WORD (- = remove / ? = help / + = return)" << endl << endl;
	cout << "? --> show synonym" << endl << endl;
	cout << "=========================================" << endl << endl;
	cout << "1.Start game" << endl;
	cout << "0.Exit" << endl << endl;

	cout << "Option: ";
	cin >> a;

	while (!(a == 1 || a == 0)) //checks for input mistakes
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid option. Try again" << endl;
		cin >> a;
	}
}

void Clues(Board &c, Dictionary &d)
{
	size_t size;
	size = c.sizeIntern();

	vector<string> horizontal;
	vector<string> vertical;

	for (size_t i = 0; i < size; i++)
		//goes through all the elements in the struct and finds clues
	{
		string word, position;
		position = c.FindPositioninStruct(i); //return position of the given element of vector
		word = c.FindWordinStruct(i); //return word of the given element of vector
		if (position.at(2) == 'H')
			//saves the synonymous for display in 2 vectors: horizontal or vertical clues
			horizontal.push_back(position + "-->" + d.lookForSyn(word));
		else
			vertical.push_back(position + "-->" + d.lookForSyn(word));

	}

	//write on screen clues
	cout << "-------VERTICAL-------" << endl;
	for (size_t i = 0; i < vertical.size(); i++)
	{
		cout << vertical.at(i) << endl;
	}
	cout << "-----HORIZONTAL-----" << endl;
	for (size_t i = 0; i < horizontal.size(); i++)
	{
		cout << horizontal.at(i) << endl;
	}
}

int Game(Board &c, Dictionary &d, Player &h, string boardName)
{

	///////////////////////////////////
	// Start to fill the board/////////
	///////////////////////////////////

	string position, word;
	int flag, flagw; //flag = 1 -> doesnt write on board (errors or help /remove) flag = 0 -> write o board
	flag = 0;
	flagw = 0;

	cout << " Position ( LCD / CNTRL-Z = stop)  ? " << endl;
	cin >> position;

	int repeat, saveflag;
	repeat = 0;
	saveflag = 1;


	while (repeat != 1) //in case flag eof is activated cntrl-z was writen->exit of the option of game
	{
		while (!cin.eof())
		{
	
			while (!(c.verifyPosition(position) && c.RightPosition(position)))//verify erros in writting the position
			{
				cout << "Invalid position.Try again";
				cout << "Position ( LCD / CNTRL-Z = stop)  ? " << endl;
				cin >> position;

			}
			

			cout << "Word ( - = remove / ? = help / + = return ) .. ? " << endl;
			cin >> word;
			Uppers(word); //in case the input is not in upper letters

			while (!d.validword(word)) //verify if word is valid
			{
				if (word == "-") //eliminate word
				{
					if (!c.deletePlayerWord(position))
					{
						cout << "Error. Impossible to remove word from the postion choosen." << endl;
						cout << "Please try again." << endl;

					}
					c.displayPlayBoard(); //shows updated word
					flag = 1;
					break;
				}
				else
					if (word == "?")
					{
						string word;
						word = c.FindWordPosition(position); //return word in the position choosen
						cout << d.lookForSyn(word) << endl; //show a synonymous of the word
						flag = 1;
						h.addClues(); //updates the player atribute clues
						break;
					}
					else
						if (word == "+")
						{
							flag = 1;
							break;
						}
						else
						{
							cin.ignore(1000, '\n');
							cout << "Error typing the word.Try again. Word:" << endl;
							cin >> word;
							Uppers(word);
							flag = 0;
						}
			}

			if (flag != 1)
			{
				if (!c.RepeatPlayerWord(word))
				cout << "Word already used. Try another one" << endl;
				else
				{
					if (!c.writePlayerBoard(position, word)) 
					//if write is false invalid word and doesnt write
					{
						cout << "Invalid word. Try again" << endl;
					}
					else
					{
						c.updateIntern_new(position, word);
						c.displayPlayBoard();
					}
				}
			}

			cout << " Position ( LCD / CNTRL-Z = stop)  ? " << endl;
			cin >> position;
			flag = 0;
		}


		cin.clear(); //clean the eof flag so it can read the next input

		if (c.FinalCheckBoard())
		{
			cout << "Correct!! Want to save game? (0 for yes; 1 for no)" << endl;
			cin >> saveflag;
			repeat = 1;
		}
		else
		{
			cout << "wrong Solution! Return the game? (0 to return; 1 to exit and show solution) " << endl;
			cin >> repeat;
			if (repeat == 1)
				saveflag = 2;

			if (repeat == 0)
			{
				cout << "Position ( LCD / CNTRL-Z = stop)  ? " << endl;
				cin >> position;
			}
		}
	}

	return saveflag;

}

int main()
{
	//a: menu game option; t1: beggining time; t2: ending time; save: true ->save the player information
	int a,resume;
	resume = 1;
	a = 1000;
	time_t t1, t2;
	bool save;
	string boardName, dictiName;

	while (a != 0)
	{
		Begining(a); //Display menu and choose option 

		switch (a)
		{
		case 1:
			//start the game
		{
		Player h;

		cout << "Choose the board to play:" << endl;
		cin >> boardName;

		//load the beggining information:
		//load the board and process it fo the game, and load dictionary
		Board c(boardName, dictiName);
		Dictionary d(dictiName, resume);
		if (resume == 2) //in case the dictionary is unavailable we return to the menu
		{
			resume = 1;
			break;
		}
		c.convert_to_playboard();
		c.displayPlayBoard();


		Clues(c, d); //show clues for game
		t1 = time(NULL); //beggining time

		////////////
		////Game///
		///////////
		int save;

		save = Game(c, d, h, boardName);
		t2 = time(NULL); //ending time

		//updates time 
		time_t timef;
		timef = h.timeGame(t1, t2); //calculates the time of game
		h.setTime(timef); //updates the time of game
		cout << "Time passed: " << timef << endl; //displays

		if (save == 0) //saves the game information
		{
			h.saveGame(boardName);
		}
		else
			if (save == 2)
			{
				Board b;
				b.resumeBoard(boardName, dictiName);
				b.displayBoard();
			}
		
		break;
		}
		case 0:
			//exit the game
		{
			return 0;
		}
		}

	}

}