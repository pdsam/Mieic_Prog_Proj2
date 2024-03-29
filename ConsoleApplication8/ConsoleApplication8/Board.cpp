#include "Board.h" 

using namespace std;

/*functions made for modifying class attributes, particulary the hight and length of
board(x/y)*/
void Board::setX(size_t newX) {
	x = newX;
}
void Board::setY(size_t newY) {
	y = newY;
}

/////////////////////////
//Program1 Constructors//
/////////////////////////

//makes a board with height x and length y
Board::Board(int x, int y)
{
	this->matrix.resize(x);

	for (unsigned int i = 0; i <= matrix.size() - 1; i++)
	{
		matrix.at(i).resize(y, '.');// the caracter '.' means space avaible for writing

	}

}
//default constructor in case the x and y levels aren't avaible
Board::Board()
{
	this->matrix.resize(2);
	this->matrix.at(0).resize(2, '.');
	this->matrix.at(1).resize(2, '.');

}

////////////////////////
//Program2 Constructor//
///////////////////////

// Constructor that uses the board file
Board::Board(string &Boardname, string &DictName) {
	//file opening part:
	ifstream f;
	f.open((Boardname + ".txt").c_str());
	while (!f.is_open()) //checks if dictionary is valid 
	{
		cout << "file not found! Try again" << endl;
		cin >> Boardname;
		f.open((Boardname + ".txt").c_str());
	}

	string line;
	//dictionary name
	getline(f, line, '\n');

	DictName = line;
	DictName = line.substr(0, line.find('.'));
	//removing the blank line
	getline(f, line, '\n');
	//handling just the first line of the matrix to find the length of the board
	getline(f, line, '\n');
	line.erase(remove_if(line.begin(), line.end(), isspace), line.end()); //existe funçao escrita em baixo
	this->y = (int)line.length();

	vector <char> temp;
	//push back the first line
	for (size_t i = 0; i < line.length(); i++) {
		temp.push_back(line.at(i));
	}
	matrix.push_back(temp);
	//pushes back the rest of the lines for the board
	while (getline(f, line, '\n'))
	{
		temp.erase(temp.begin(), temp.end());
		if (!line.empty()) {
			line.erase(remove_if(line.begin(), line.end(), isspace), line.end()); //existe funçao escrita em baixo
			for (unsigned int i = 0; i < line.length(); i++) {
				(this->x)++;
				temp.push_back(line.at(i));
			}
			matrix.push_back(temp);
		}

		else break;
	}
	this->x = (int)matrix.size();//gets the height of the board
								 
	//updates the vector that contais the words and their positions
	while (getline(f, line, '\n'))
	{
		string position, word;
		position = line.substr(0, line.find(" "));
		word = line.substr(line.find(" ") + 2);
		internStruct.push_back(pair<string, string>(position, word));
	}
	f.close();

}

/////////////////////////////////////////////////////////////////
bool Board::verifyPosition(string position)
//verify is the entered position is corrected
{
	if (position.size() != 3) //checks if size of input is 3
		return false;

	char vertical, horizontal, p;

	vertical = position.at(0);
	horizontal = position.at(1);
	p = position.at(2);

	unsigned int nvertical, nhorizontal;

	nhorizontal = position.at(1) - 97;
	nvertical = position.at(0) - 65;

	if (isupper(vertical) && islower(horizontal) && (p == (char)72 || p == (char)86))
		//checks if: first letter is upper; second is lower; third is H or V
	{
		//checks if the first and second letters are valid considering the size of the board
		return nhorizontal < y && 0 <= nhorizontal && nvertical < x && 0 <= nvertical;
	}
	else
		return false;

} //check for position input errors
void eliminSpace(string &line)
//auxiliary function to remove the spaces from line that comes from saved file
{
	line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
}
void Board::updateIntern(string position, string word)
//updates intern structure of the original words of the board
{
	internStruct.push_back(pair<string, string>(position, word));
}
///////////////////////////////////////////////////////////////


//--------------------------------------------------------------
////////////////////////////////////////////////////////////////
//////////////////////Program1 functions////////////////////////
////////////////////////////////////////////////////////////////
//--------------------------------------------------------------

/////////////////
//Display Board//
/////////////////

void Board::displayBoard()
{
	//primeira fila de letras minusculas a vermelhos
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << "  ";
	SetConsoleTextAttribute(hConsole, 12);//12 = light red
	//display the letters from a to the length of letters
	for (unsigned int i = 0; i < y; i++)
	{
		char temp = (char)97 + i;
		cout << temp << " ";
	}

	cout << "\n";
	//displays the rest of the board
	for (size_t n = 0; n < x; n++)
	{
		//displays the bertical letters
		SetConsoleTextAttribute(hConsole, 12);
		char temp = (char)(65 + n);
		cout << temp << " ";

		size_t size;
		size = matrix.at(0).size();
		SetConsoleTextAttribute(hConsole, 15);
		SetConsoleTextAttribute(hConsole, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		//displays the rest of the board
		for (size_t i = 0; i < size; i++)
		{
			if (matrix.at(n).at(i) == '#') {//if the character is a # the word is writtin in white and the background in black
				SetConsoleTextAttribute(hConsole, 15);
				cout << matrix.at(n).at(i) << " ";
			}
			else {//if not the character must be displyed in black with a white background
				SetConsoleTextAttribute(hConsole, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
				cout << matrix.at(n).at(i) << " ";
			}
		}
		//normalizes the console colours
		SetConsoleTextAttribute(hConsole, 15);
		cout << endl;
	}
}

//////////////////////
//// Write  Board////
/////////////////////

bool Board::writeBoard(string orientation, const string input) //writes on the board(matrix in the object); return true if the write was sucessful or false if not
{
	int cX, cY, p;

	cY = orientation.at(1) - 97;
	cX = orientation.at(0) - 65;
	p = orientation.at(2);


	string temp;
	temp = input;
	/////////////////////////////vertical//////////////////////////////
	if (p == 86)//if vertical
	{
		if (cX + input.size() <= this->x)//checks if it fits in the space
		{
			if (cX + input.size() < this->x) {//checks if there is space for the '#' after the word
				if (matrix.at(cX + input.size()).at(cY) == '.') {//check if nothing occupying the space after the word
					matrix.at(cX + input.size()).at(cY) = '#';//writes a # after the word
				}
				else if (matrix.at(cX + input.size()).at(cY) != '#') {
					return false;
				}
				
			}
			if (cX != 0) {//if the word isn't at the top, checks if it can place a # before the word
				if (matrix.at(cX - 1).at(cY) == '.') {
					matrix.at(cX - 1).at(cY) = '#';
				}
				
			}
			//writes the word on the board and checks if it is incorrectly overlaping with other words
			for (unsigned int i = 0; i < input.size(); i++) {
				if (matrix.at(cX + i).at(cY) == '.') {
					matrix.at(cX + i).at(cY) = input.at(i);
				}
				else if (matrix.at(cX + i).at(cY) != input.at(i)) {//checking if it is the same caracter in case the space is not avaible
					return false;
					break;
				}
			}

		}
		else return false;


	}

	///////////////////////////////horizontal/////////////////////////////
	else if (p == 72) {//if horizontal
		if (cY + input.size() <= this->y) {//checks if it fits in the space

			if (cY + input.size() < this->y) {//checks if there is space for the '#' after the word
				if (matrix.at(cX).at(cY + input.size()) == '.') {//check if nothing occupying the space after the word
					matrix.at(cX).at(cY + input.size()) = '#';//writes a # after the word
				}
				else if (matrix.at(cX).at(cY + input.size()) != '#') {
					return false;
				}
				
			}

			if (cY != 0) {//if the word isn't at the top, checks if it can place a # before the word
				if (matrix.at(cX).at(cY - 1) == '.') {
					matrix.at(cX).at(cY - 1) = '#';
				}
				
			}

			//writes the word on the board and checks if it is incorrectly overlaping with other words
			for (unsigned int i = 0; i < input.size(); i++) {
				if (matrix.at(cX).at(cY + i) == '.' || matrix.at(cX).at(cY + i) == ' ') {
					matrix.at(cX).at(cY + i) = input.at(i);
				}
				else if (matrix.at(cX).at(cY + i) != input.at(i)) {//checking if it is the same caracter in case the space is not avaible
					return false;
					break;
				}
			}
		}

	}


	
	else {
		return false;
	}
	return true;
}

///////////////////////////
////Eliminate from Board///
///////////////////////////

bool Board::deleteWord(string position)
//reprints the board but without the selected word, deleting it from the internal structure
{
	bool flag = false;
	//finds and deletes it from the internal structure
	for (size_t i = 0; i < internStruct.size(); i++) {
		if (internStruct.at(i).first == position) {
			internStruct.erase(internStruct.begin() + i);
			flag = true;
		}


	}
	// overwrites the matrix with the caracter representation of a avaible space
	if (flag) {
		for (size_t i = 0; i < matrix.size(); i++) {
			for (size_t j = 0; j < matrix.at(i).size(); j++) {
				matrix.at(i).at(j) = '.';
			}

		}
		//reprints the board now without the pretended word
		for (size_t i = 0; i < internStruct.size(); i++) {
			writeBoard(internStruct.at(i).first, internStruct.at(i).second);

		}
	}
	return flag;
}


///////////////////
//Help functions///
///////////////////

string Board::helpWordAux(string position, size_t size)
//auxiliar function to helpWord (dictionary.cpp) 
//return word with letters and ?
{
	int horizontal, vertical, p;
	vertical = position.at(0) - 65;
	horizontal = position.at(1) - 97;
	p = position.at(2);

	string word;

	if (p == 72) //horizontal
	{
		for (size_t i = horizontal; i <= size; i++)
		{
			string a;
			a = matrix.at(vertical).at(i);
			if (a == "#") //if finds # ends and returns word with # 
				return word.append("#");
			else
				if (a == ".") //if the position is empty adds ? to word 
					word.append("?");
				else
					word.append(a); //if the position isn't empty adds the letter to word 
		}

		return word;
	}
	else //vertical
	{
		for (unsigned int i = vertical; i <= size; i++)
		{
			string a;
			a = matrix.at(i).at(horizontal);
			if (a == "#")//if finds # ends and returns word 
				return word;
			else
				if (a == ".")//if the position is empty adds ? to word 
					word.append("?");
				else
					word.append(a); //if the position isn't empty adds the letter to word 
		}
	}

	return word;
}

size_t Board::auxHelp(string position)
//return size of line or column to be used in main
{
	int p = position.at(2);

	if (p == 72)
	{
		return matrix[0].size();//return size of line
	}
	else
		return matrix.size();//return size of column
}

/////////////////////
/////Resume board ///
/////////////////////

void Board::resumeBoard(string &Boardname, string &DictName)
{
	ifstream f;
	f.open((Boardname + ".txt").c_str());
	string line;//opens the text file

	while (!f.is_open()) //checks if boardname is valid
	{
		cerr << "file not found!" << endl;
		cin.clear();
		cin.ignore(1000, '/n');
		cin >> Boardname;
		f.open((Boardname + ".txt").c_str());
	}

	size_t x, y, n;
	x = 0;
	y = 0;
	n = 0;

	getline(f, line, '\n');
	//dictionary name
	DictName = line;
	DictName = line.substr(0, line.find('.'));
	//
	getline(f, line, '\n');
	getline(f, line, '\n'); //first line
	eliminSpace(line);
	y = line.size();
	x++;

	while (getline(f, line, '\n')) //updates counter for the x size
	{
		if (!line.empty())
			x++;
		else break;
	}

	//updates matrix 
	this->matrix.resize(x);

	for (size_t i = 0; i <= matrix.size() - 1; i++)
	{
		matrix.at(i).resize(y, '.');

	}

	setX(x);
	setY(y);
	//

	while (getline(f, line, '\n')) //write words on board and update internstruct
	{
		string position, word;
		position = line.substr(0, line.find(" "));
		word = line.substr(line.find(" ") + 2);
		//writes the words on board
		writeBoard(position, word);
		//updates intern structure with words
		internStruct.push_back(pair<string, string>(position, word));
	}

	f.close();//closes file

}

///////////////////////////
////save board functions///
///////////////////////////

void Board::saveBoard(string filename, string dictName, int flag)
//save the board on file (flag indicate wether we want to save for later or finalize the board)
{
	//file: file to save the board to
	ofstream file;
	file.open((filename + ".txt").c_str());

	size_t vertical;
	vertical = matrix.size();

	file << dictName + ".txt" << endl;
	file << endl;

	for (size_t n = 0; n < x; n++)
	{
		size_t size;
		size = matrix[0].size(); //horizontal size

		for (size_t i = 0; i < size; i++)
		{
			if (matrix[n][i] == '.')
				if (flag == 0) //save to return 
					file << matrix[n][i] << "   ";
				else //save to finalize board
					file << "#" << "   ";

			else
				file << matrix[n][i] << "  ";
		}

		file << endl;
	}

	file << endl;

	for (size_t i = 0; i < internStruct.size(); i++)
		//show the begining position of each word
	{
		file << internStruct.at(i).first << "  " << internStruct.at(i).second << endl;
	}

	file.close(); //closes file

}

string numberdigits(int number)
//returns string with the correct amount of digits --> auxiliary to namefile
{
	string finalnumber;

	//counts digits and writes the amount of '0' before the number based on the result
	if (ceil(log10(fabs(number) + 1)) > 2) //3 digits
	{
		return finalnumber = to_string(number);
	}
	else
		if (ceil(log10(fabs(number) + 1)) > 1) //2 digits
		{
			return finalnumber = "0" + to_string(number);
		}
		else //1 digit
		{
			return finalnumber = "00" + to_string(number);
		}
}

string Board::namefile()
//returns name of the file to save
{
	//name: file to open ; numberString: component of numbers in the name of the file to save
	ifstream name;
	string numberStr;
	int number;
	number = 1;
	numberStr = numberdigits(number); //initialize string

	name.open(("b" + numberStr + ".txt").c_str());

	//looking for when the file doesnt open (doesnt exist);
	//until then we increment an unity to the name of file
	while (name.is_open())
	{
		name.close();
		number++;
		numberStr = numberdigits(number);
		name.open(("b" + numberStr + ".txt").c_str());
	}

	return "b" + numberStr;

}

/////////////////////////////////
/////Check if word is repeated//
////////////////////////////////

bool Board::RepeatWord(string word)
//if the input word exists on the intern structure return false else true
{
	for (size_t i = 0; i < internStruct.size(); i++)
	{
		vector<pair<string, string>>::iterator it;
		pair<string, string> a;
		it = internStruct.begin() + i;
		a = *it;

		if (a.second == word)
			return false;
	}
	return true;

}

//--------------------------------------------------------------
////////////////////////////////////////////////////////////////
//////////////////////Program2 functions////////////////////////
////////////////////////////////////////////////////////////////
//--------------------------------------------------------------

void Board::updateIntern_new(string position, string word)
//updates intern structure of the words written on the board from the player
{
	internStruct_new.push_back(pair<string, string>(position, word));
}

size_t Board::sizeIntern()
//returns size of internStruct 
//auxiliary in main.cpp - clues function
{
	return internStruct.size();
}

///////////////////////////
//// Write Player Board////
///////////////////////////

/*writes on the internal structure keeping the inputs from the player.
Recieves as argument the orientation string (ex: AbV), where the first is the line,
the second is the column and the last is the orientation (vertical or horizontal).
The other argument is the word to write on the board*/
// returns false if it failed to write or true if it was sucessful
bool Board::writePlayerBoard(string orientation, const string input) {
	//processing the orietation string
	int cX, cY, p, AvaibleSpace;
	unsigned int aux = 0;
	cY = orientation.at(1) - 97;
	cX = orientation.at(0) - 65;
	p = orientation.at(2);


	string temp;
	temp = input;
	/////////////////////////////vertical//////////////////////////////
	if (p == 86)
	{

		//the while loop checks how many blank spaces there are to fill
		while (x > (aux + cX))
		{
			if (matrix.at(cX + aux).at(cY) == '#') {
				AvaibleSpace = aux;
				break;
			}
			else aux++;
		}

		if (input.size() == aux) {//checks if the word fits in the white spaces
								  //in the for loop it is checked if the word doesn't mess up others
			for (unsigned int i = 0; i < input.size(); i++) {
				if (matrix.at(cX + i).at(cY) == '1') {//1= avaible space for writing
					matrix.at(cX + i).at(cY) = input.at(i);
				}
				else if (matrix.at(cX + i).at(cY) != input.at(i)) {
					return false;
					break;
				}
			}

		}
		else return false;

	}
	///////////////////////////////horizontal/////////////////////////////////
	if (p == 72) {
		//the while loop checks how many blank spaces there are to fill
		while (y > (aux +cY))
		{
			if (matrix.at(cX).at(cY + aux) == '#') {
				//AvaibleSpace = aux;
				break;
			}
			else aux++;
		}

		//checks if the word fits in the white spaces
		if (input.size() == aux) {
			//in the for loop it is checked if the word doesn't mess up others
			for (unsigned int i = 0; i < input.size(); i++) {
				if (matrix.at(cX).at(cY + i) == '1') {//1= avaible space for writing
					matrix.at(cX).at(cY + i) = input.at(i);
				}
				else if (matrix.at(cX).at(cY + i) != input.at(i)) {
					return false;
					break;
				}
			}

		}
		else return false;
	}
	return true;
}

////////////////////////
//Display Player Board//
////////////////////////

/*converts the board from the save file into something the displayPlayerBoard understands*/
void Board::convert_to_playboard() {
	playerMatrix = matrix;//keeps the original matrix for the delete function
	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t l = 0; l < matrix.at(i).size(); l++) {
			if (matrix.at(i).at(l) == '.') {
				matrix.at(i).at(l) = '0';//not avaible for writing
			}
			else if (matrix.at(i).at(l) != '.' && matrix.at(i).at(l) != '#') {
				matrix.at(i).at(l) = '1';//1 = avaible for writing 

			}

		}


	}

}

//display's the board that the player is suposed to fill
//12= red; 0 = black; 15 = white;
void Board::displayPlayBoard() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << "  ";
	SetConsoleTextAttribute(hConsole, 12);//12 = light red
										  //first line of letters
	for (size_t i = 0; i < y; i++)
	{
		char temp = (char)(97 + i);
		cout << temp << " ";
	}

	cout << "\n";


	for (size_t i = 0; i < matrix.size(); i++) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
		//the first letter of the line
		char holder = (char)(65 + i);
		cout << holder << " ";

		for (size_t l = 0; l < matrix.at(i).size(); l++) {
			if (matrix.at(i).at(l) == '1') {//if 1 the space is avaible for writing
				SetConsoleTextAttribute(hConsole, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
				cout << "." << " ";
			}
			else if (matrix.at(i).at(l) == '0') {//if 0 the space is not avaible for wrtiing
				SetConsoleTextAttribute(hConsole, 15 | BACKGROUND_BLUE | BACKGROUND_GREEN);
				cout << " " << " ";
			}
			else if (matrix.at(i).at(l) == '#'){//if # than it's itself			
				SetConsoleTextAttribute(hConsole, 15);
			cout << "#" << " ";
		}
			else//if it is none of the above it's a letter from the words
			{
				SetConsoleTextAttribute(hConsole, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
				cout << matrix.at(i).at(l) << " "
					;
			}

	}
	cout << "\n";

}
//makes the writing colour normal again
SetConsoleTextAttribute(hConsole, 15);

}

/////////////////////////////
////Eliminate Player Board///
/////////////////////////////

//reprints the board but without the selected word, deleting it from the internal structure
bool Board::deletePlayerWord(string position)
{
	bool flag = false;
	//finds and deletes it from the internal structure
	for (size_t i = 0; i < internStruct_new.size(); i++) {
		if (internStruct_new.at(i).first == position) {
			internStruct_new.erase(internStruct_new.begin() + i);
			flag = true;
		}
	}
	// overwrites the matrix with the caracter representation of a avaible space
	if (flag) {
		matrix = playerMatrix;
		convert_to_playboard();



		//reprints the board now without the pretended word
		for (size_t i = 0; i < internStruct_new.size(); i++) {
			writePlayerBoard(internStruct_new.at(i).first, internStruct_new.at(i).second);

		}
	}
	return flag;
}

///////////////////
//Clues Functions//
///////////////////

string Board::FindWordinStruct(size_t i)
//return word of the given element (i) of vector
{
	vector<pair<string, string>>::iterator it;
	pair<string, string> a;
	it = internStruct.begin() + i;
	a = *it;

	return a.second;
}

string Board::FindPositioninStruct(size_t i)
//return position of the given element(i) of vector
{
	vector<pair<string, string>>::iterator it;
	pair<string, string> a;
	it = internStruct.begin() + i;
	a = *it;

	return a.first;
}

bool Board::RepeatPlayerWord(string word)
//if the input word exists on the intern structure return false else true
{
	for (size_t i = 0; i < internStruct_new.size(); i++)
	{
		vector<pair<string, string>>::iterator it;
		pair<string, string> a;
		it = internStruct_new.begin() + i;
		a = *it;

		if (a.second == word)
			return false;
	}
	return true;

}

///////////////////////////
/////Help functions////////
///////////////////////////

//verify if in fact the element of the internStruct is the right one for the input position
//in case there is more than one word in the line or column
//auxiliary to FindWordPosition
bool VerifyWord(string word, string internPosition, string position)
{
	unsigned int pH, pV, itpH, itpV;
	pV = position.at(0);
	pH = position.at(1);
	itpV = internPosition.at(0);
	itpH = internPosition.at(1);


	if (position.at(2) == 72)
		//horizontal
		//if we move horizontally in the board the number of spaces equal to the size of the word
		//we need to pass the position choosen from the player to be the correspondent word
	{
		if (itpH <= pH + word.size())
			return true;
		else
			return false;
	}
	else //vertically
	{
		//if we move vertically in the board the number of spaces equal to the size of the word
		//we need to pass the position choosen from the player to be the correspondent word
		if (itpV <= pV + word.size())
			return true;
		else
			return false;
	}
}

//auxiliary to lookforSyn (dictionary.cpp)
//returns the word correspondent to the position on the board
string Board::FindWordPosition(string position)
{
	//p:third letter of the position - H or V
	int p;
	p = position.at(2);

	vector < pair < string, string >>::iterator it;
	pair<string, string> a;

	//in case the positions the person chose is the beggining position for the word
	//look in the internStruct 
	for (it = internStruct.begin(); it != internStruct.end(); it++)
	{
		a = *it;
		if (a.first == position)
		{
			return a.second;
		}
		
	}

	//in case is not the beggining position of the word
	if (p == 72) //horizontal: need to work with the first letter of the position
	{
		for (it = internStruct.begin(); it != internStruct.end(); it++)
		{
			//looks for the right position in the intern structure
			a = *it;
			if (a.first.at(0) == position.at(0) && a.first.at(2) == position.at(2))
				//first and third letter of position are the same
			{
				//in case there is more than one word in the line
				//(there is more than one position with the same first and the third letter)
				if (VerifyWord(a.second, a.first, position)) //function to verify if the position is the correct 
					return a.second;
			}
		}
		
	}
	else //vertical: need to work with the second letter of the position
	{
		for (it = internStruct.begin(); it != internStruct.end(); it++)
		{
			//looks for the right position in the intern structure
			a = *it;
			if (a.first.at(1) == position.at(1) && a.first.at(2) == position.at(2))
			{
				//in case there is more than one word in the column
				//(there is more than one position with the same second and the third letter)
				if (VerifyWord(a.second, a.first, position)) //function to verify if the position is the correct 
					return a.second;
			}
		}
		
	}

	return "a";
}

bool Board::RightPosition(string position)
//if the input position exists on the intern structure return false else true
{
	for (size_t i = 0; i < internStruct.size(); i++)
	{
		vector<pair<string, string>>::iterator it;
		pair<string, string> a;
		it = internStruct.begin() + i;
		a = *it;

		if (a.first == position)
			return true;
	}
	return false;

}

/////////////////////
////Final Check//////
/////////////////////

bool Board::FinalCheckBoard()
//checks if the board made by the player correponds to the solution
{
	vector<pair<string, string>>::iterator p; //to search in the original intern structure
	int flag = 1;

	for (p = internStruct.begin(); p != internStruct.end(); p++)
	{
		for (size_t t = 0; t < internStruct_new.size(); t++)
		{
			if (*p == internStruct_new[t])
				//verify if the vector in the original structure exists in the player structure
			{
				flag = 0; //in case it matches a vector of the player struct
				break;
			}
			else
				flag = 1;//in case it doesn't matches a vector of the player struct
		}

		if (flag == 1) //one element doesn't match -> different structures
			return false;

	}
	return true;
}

Board::~Board()
{}