#include "Dictionary.h"

using namespace std;

void convertToUppers(string &s)
//converts all caracters of words or synonyms to uppercases
{
	for (unsigned int i = 0; i < s.size(); i++)
	{
		if (islower(s.at(i)))
		{
			s.at(i) = toupper(s.at(i));

		}

	}

}
////////////////////////////////////////////////////////////////////
bool ValidSynonyms(string s)//validates synonyms
{
	for (size_t i = 0; i < s.size(); i++)
		//searches for every char different to the valid list of char  
	{
		unsigned char letter;
		letter = s[i];

		if (!(isupper(letter)) || s[i] == ' ')
			//conditions for valid synonyms: all letters(uppers) or spaces (in case its more than one word)
		{
			return false;
		}
	}

	return true;
}
////////////////////////////////////////////////////////////////////
Dictionary::Dictionary(string &dictName, int &resume)
//resume: 1-> in case we are resuming a board we can't give the user the option to change the dictionary file in case of error
{
	//f: dictionary file
	ifstream f;
	f.open((dictName + ".txt").c_str()); //opens the text file

	while (!f.is_open()) //checks if dictionary is valid 
	{
		if (resume == 1)
		{
			cout << "Dictionary unavailable!" << endl;
			resume = 2;
			return;
		}
		cout << "Dictionary not found! Try another" << endl;
		cin >> dictName;
		f.open((dictName + ".txt").c_str());
	}

	//line: string with line content
	//word: first word of line 
	//s: synonyms
	string line, word, s;
	//vector that helps to fill the map structure
	vector<string> synonyms;

	while (getline(f, line, '\n')) //stops when there is nothing more to extract
	{
		if (line.empty()) //in case of empty lines we don't need to process it
			break;

		word = line.substr(0, line.find(":"));
		convertToUppers(word);

		line = line.substr(line.find(":") + 1); //updates line to synonyms only 

		while (!line.empty()) //loads synonyms of the word to vector
		{
			if (line.find(';') == line.size()) //if the ; is in the end of the line there arent words to process after 
				break;

			s = line.substr(1, line.find(",") - 1);
			convertToUppers(s);

			if (ValidSynonyms(s)) //tests for valid synonyms
			{
				synonyms.push_back(s); //updates vector of synonyms
			}

			//writes last word of line in the map because we look for the , first 
			//and update the line for next iteration only after doing so 

			if (line.find(",") == -1) //if we don't find more --> no more words
				break;
			else
				line = line.substr(line.find(",") + 1); //update line

		}

		//loads word and synounymous to map
		dictionary_map.insert(pair<string, vector<string>>(word, synonyms));

		synonyms.clear();//cleans vector for next word's synonyms
	}

	f.close();

}
////////////////////////////////////////////////////////////////////
bool Dictionary::validword(string word)
//checks if the input word exists on the dictionary
{
	if (dictionary_map.find(word) != dictionary_map.end())
		return true;
	else
		return false;

}
////////////////////////////////////////////////////////////////////
bool wildcardMatch(const char *str, const char *strWild)
{
	// We have a special case where string is empty ("") and the mask is "*".
	// We need to handle this too. So we can't test on !*str here.
	// The loop breaks when the match string is exhausted.
	while (*strWild)
	{
		// Single wildcard character
		if (*strWild == '?')
		{
			// Matches any character except empty string
			if (!*str)
				return false;
			// OK next
			++str;
			++strWild;
		}
		else
		{
			// Standard compare of 2 chars. Note that *str might be 0 here,
			// but then we never get a match on *strWild
			// that has always a value while inside this loop.
			if (toupper(*str++) != toupper(*strWild++))
				return false;
		}
	}
	// Have a match? Only if both are at the end...
	return !*str && !*strWild;
}

/////////////Program1 functions////////////////////////////////////

void Dictionary::wordsVector()
//updates atribute words to help in the helpWord function
{
	if (words.empty())
		//only writes words the first time the ? is called (when the vector is still empty)
	{
		for (auto i : dictionary_map) //range based loop
		{
			words.push_back(i.first);
		}
	}
}
////////////////////////////////////////////////////////////////////
string Dictionary::helpWord(string word)
{
	//i: index ; n: counter to stop the while cicle after x repetitions
	size_t i;
	int n;
	n = 0;

	srand((unsigned)time(NULL));

	i = rand() % (word.size() - 1); //generate random index 

	while (!wildcardMatch(words[i].c_str(), word.c_str()) && n < 5801)
		//in case the randomly generated words doesnt match repeats until finding correspondent word or repeting 5800 times 
	{
		i = rand() % (words.size() - 1);
		n++;
	}

	if (n < 5801)
		return words[i];
	else
		return " "; //in case there is no word that matches

}

/////////////Program2 functions////////////////////////////////////

string Dictionary::lookForSyn(string word)
{

	if (usedsyn.find(word) != usedsyn.end()) //if already exists the key in the map
	{
		if (usedsyn.at(word).size() == dictionary_map.at(word).size()) //in case all the synonymous were already choosen 
			return "No more synonyms to show";
	}

	srand((unsigned)time(NULL));

	size_t i;
	i = rand() % (dictionary_map.at(word).size() - 1); //random position of the syn

													   //check if the chosen synonyms for the help was already shown 
													   //(word of the synonyms exists in usedsyn) 
	if (usedsyn.find(word) != usedsyn.end())
	{
		while (find(usedsyn.at(word).begin(), usedsyn.at(word).end(), dictionary_map.at(word).at(i)) != usedsyn.at(word).end())//look for another synonyms if it is equal
		{
			i = rand() % dictionary_map.at(word).size();
		}
		this->usedsyn.at(word).push_back(dictionary_map.at(word).at(i)); ////insert in the usedsyn the respective syn used in the word key
	}
	else //in case the word didn't had synonyms used yet (usedsyn didn't have the word)
	{
		vector<string>newsyn; //create vector to allocate new syn 
		newsyn.push_back(dictionary_map.at(word).at(i));

		//insert in the usedsyn the word with the respective syn used
		this->usedsyn.insert(pair<string, vector<string>>(word, newsyn));

	}

	return dictionary_map.at(word).at(i);
}

///////////////////////////////////////////////////////////////////
Dictionary::~Dictionary()
{
}
