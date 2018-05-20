#include "Dictionary.h"
#include "stdafx.h"
#include "board.h"
#include "stdio.h"

using namespace std;

void Uppers(string &s) //converts all caracters to uppercases
{
	for (unsigned int i = 0; i < s.size(); i++)
	{
		if (islower(s.at(i)))
		{
			s.at(i) = toupper(s.at(i));

		}

	}

}
//
////////////////////////////////////////////////////////////////////
void ProgramBegin(int &a)
{
	cout << "CROSSWORDS PUZZLE CREATOR" << endl;
	cout << "=========================================" << endl << endl;
	cout << "INSTRUCTIONS:" << endl << endl;
	cout << "POSITION (LCD /CNTRL-Z = stop)" << endl << endl;
	cout << "WORD (- = remove / ? = help / += return)" <<endl << endl;
	cout << "? --> show words to fit the position" << endl << endl;
	cout << "=========================================" << endl << endl;
	cout << "1.Create puzzle" << endl;
	cout << "2.Resume puzzle" << endl;
	cout << "0.Exit" << endl << endl;

	cout << "Option: ";
	cin >> a;

	while (!(a == 1 || a == 0 || a == 2)) //check for erros input
	{
		cin.clear();
		cin.ignore(10, '\n');
		cout << "Invalid option. Try again" << endl;
		cin >> a;
	}
}
////////////////////////////////////////////////////////////////////
int FillBoard(Board &b, Dictionary &d)
{
	///////////////////////////////
	// Start to fill the board/////
	///////////////////////////////

	string position, word;
	int flag; //flag = 1 -> doesnt write on board (errors or help /remove) flag = 0 -> write o board
	flag = 0;

	cout << " Position ( LCD / CNTRL-Z = stop)  ? " << endl;
	cin >> position;

	while (!cin.eof()) //in case flag eof is activated cntrl-z was writen->exit of process
	{

		while (!b.verifyPosition(position))//verify erros in writting the position
		{
			cout << "Invalid position.Try again";
			cout << "Position ( LCD / CNTRL-Z = stop)  ? " << endl;
			cin >> position;
		}

		cout << "Word ( - = remove / ? = help / + = return ) .. ? " << endl;
		cin >> word;
		Uppers(word);//in case input isn't in uppers


		while (!d.validword(word)) //verify if word is valid
		{
			if (word == "-") //eliminate word
			{
				if (!b.deleteWord(position))
				{
					cout << "Error. Impossible to remove word from the postion choosen." << endl;
					cout << "Please try again." << endl;

				}
				b.displayBoard(); //shows updated word
				flag = 1;
				break;
			}
			else
				if (word == "?") //help function
				{
					d.wordsVector(); //allocates words in intern structure (if it's still empty)
									 //helpword: string with ? and letters
									 //size: size of line or collumn to be used for the help 
					string helpword;
					size_t size;
					size = b.auxHelp(position);
					for (size_t i = 1; i < size; i++)
						//goes through all the positions of line or collumn to help fill the helpword
					{
						helpword = b.helpWordAux(position, i);
						if (helpword.find("#") != -1) //in case it finds # we don't want more iterations in the line/column
						{
							if (helpword.size() > 2) //only looks for available words when the string is bigger than 2
							{
								if (d.helpWord(helpword) != " ") //if exists matching word shows on screen
									cout << d.helpWord(helpword.substr(0, helpword.find("#"))) << endl;
							}
							break;
						}
						else
							if (helpword.size() > 2)
							{
								if (d.helpWord(helpword) != " ")
									cout << d.helpWord(helpword) << endl;
							}
					}
					flag = 1;
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
						//in case of error input in word
						cin.ignore(1000, '\n');
						cout << "Invalid word." << endl;
						cout << "Try again. Word:" << endl;
						cin >> word;
						Uppers(word);
						flag = 0;
					}
		}

		if (flag != 1)
		{
			if (!b.RepeatWord(word))
				cout << "Word already used. Try another one" << endl;
			else
			{
				if (!b.writeBoard(position, word)) //if write is false invalid word and doesnt write 
				{
					cout << "Invalid word. Try again" << endl;
				}
				else
				{
					b.updateIntern(position, word); //updates intern structure of board with word written and the respective position
					b.displayBoard();
				}
			}
		}

		cout << " Position ( LCD / CNTRL-Z = stop)  ? " << endl;
		cin >> position;
		flag = 0;
	}

	cin.clear(); //clean the eof flag so it can read the next input
	int saveflag;

	cout << "Do you want to save for later or finalize the board (0 for first; 1 for second)" << endl;
	cin >> saveflag;

	return saveflag;
}
////////////////////////////////////////////////////////////////////
int main()
{
	//a: options for the game menu; 
	//saveflag: 0 if the player wants to save the board to resume; 1 if the player wants to finalize the board
	//resume: 1 -> dealing with a board that was resumed 
	int a, saveflag,resume;
	a = 1000;
	saveflag = 2;

	while (a != 0)
	{
		////////
		//Menu//
		////////
		ProgramBegin(a);

		switch (a)
		{
		case 1:
		{
			cout << endl << "---------------------------------------" << endl << endl;
			cout << "CREATE PUZZLE" << endl;
			cout << endl << "---------------------------------------" << endl << endl;

			string dictName;
			int x, y;
			resume = 0;
			cout << "Board size (lines columns)? ";
			cin >> x >> y;
			Board b(x, y); //creates board
			b.setX(x); //updates atributes
			b.setY(y);

			cout << "Enter the dictionary name:" << endl;
			cin >> dictName;

			Dictionary d(dictName, resume);
			cout << endl;
			b.displayBoard();


			/////////////////
			//Fill Board/////
			/////////////////

			//fillboard returns value of saveflag
			saveflag = FillBoard(b, d);

			string namefile;
			namefile = b.namefile(); //returns the name of the board (name of file)
			b.saveBoard(namefile, dictName, saveflag); //saves board


			break;
		}
		case 2:
		{
			string name, dictName;
			resume = 1;
			cout << "Choose name of the board to resume" << endl;
			cin >> name;


			Board b; //creates board
			b.resumeBoard(name, dictName); //updates the board
			Dictionary d(dictName, resume);
			if (resume == 2) //in case the dictionary is unavailable we return to the menu
				break;

			b.displayBoard();

			/////////////////
			//Fill Board/////
			/////////////////
			saveflag = FillBoard(b, d);

			//save board
			remove((name + ".txt").c_str());//deletes the old file
			b.saveBoard(name, dictName, saveflag); //saves in the new file with the same name

			break;

		}
		case 0:
		{
			return 0; //exit program
		}
		}

		//system("cls");

	}
}