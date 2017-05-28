#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include "errors.h"
#include "reflector.h"

int Reflector::load_reflector()
{
  int temp;
  ifstream in;
  in.open(reflector_filename);
  if (!in)
  {
    cerr << "FAILED TO LOAD REFLECTOR FILE" << endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }

  while (in >> temp)
  {
    //check for in stream failure
    if (in.rdstate() > 0) break;
    
    if (temp < 0 || temp > 25)
    {
      cerr << "Invalid Reflector Index (should be between 0 and 25)" << endl;
      return INVALID_INDEX;
    }

    reflector_board.push_back(temp);
    
  }

  //trying to read a non-numeric character into an int array means
  //in stream fails, and it won't reach end of file
  if (!in.eof())
  {
    cerr << "Non-numeric character in reflector file " << reflector_filename << endl;
    return NON_NUMERIC_CHARACTER;
  }

  //check through vector for repeat mappings (i.e. mapping to itself or
  //one input mapping to two outputs)
  for (unsigned int i = 0; i < reflector_board.size(); i++)
  {
    for (unsigned int j = 0; j < reflector_board.size(); j++)
    {
      if (reflector_board[i] == reflector_board[j] && i != j)
      {
	cerr << "Invalid mapping in reflector file " << reflector_filename << ", " << temp << " is mapped twice" << endl;
	return INVALID_REFLECTOR_MAPPING;

      }
    }
  }  

  //check file has 13 pairs of numbers
  if (reflector_board.size() != 26)
  {
    cerr << "Incorrect number of reflector parameters in reflector file " << reflector_filename << endl;
    return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
  }

  in.close();
  return NO_ERROR;
}

bool Reflector::reflector_switch(char &character)
{
  int letter;
  
  letter = (int)(character) - (int)('A'); 

  //map the input character to its corresponding partner
  for (int j = 0; j < 26;  j+=2)
  {
    if (letter == reflector_board[j])
      letter = reflector_board[j+1];
    else if (letter == reflector_board[j+1])
      letter = reflector_board[j];
  }
  
  character = (char)(letter + 'A');     
    
  return true;
}

