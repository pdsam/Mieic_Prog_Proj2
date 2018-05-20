#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <iostream>
#include<fstream> //open the dictionary file
#include "time.h" //generate random numbers


class Dictionary
{
public:
	Dictionary(std::string &dictName, int &resume);
	bool validword(std::string word);// checks if the input word exists in the dictionary
	////Program1 functions
	std::string helpWord(std::string word); //returns word for ? 
	void wordsVector(); //updates the words(atribute)
	bool EmptyDicti();
	////Program2 function
	std::string lookForSyn(std::string word); //returns synonyms to the word given
	~Dictionary();
private:
	//intern structure to save the words and synonyms
	//map key -> word ; key content-> synonyms
	std::map<std::string, std::vector<std::string> > dictionary_map;
	//Program1
	//intern structure to help in the ? 
	std::vector<std::string> words;
	//Program2
	//structure to save all the synonimous already showen for a given word
	std::map<std::string, std::vector<std::string> > usedsyn;
};

#endif 
