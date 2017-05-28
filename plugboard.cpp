#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include "errors.h"
#include "plugboard.h"

int Plugboard::load_pboard()
{
  int temp;
  ifstream in;
  in.open(pboard_filename);
  if (!in)
  {
    cerr << "FAILED TO LOAD PLUGBOARD FILE" << endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  
  while (in >> temp)
  {
    //check for in stream failure
    if (in.rdstate() > 0) break;

    if (temp < 0 || temp > 25)
    {
      cerr << "Invalid Plugboard Index (should be between 0 and 25)" << endl;
      return INVALID_INDEX;
    }
    
    pboard.push_back(temp);    
  }

  //trying to read a non-numeric character into an int array means
  //in stream fails, and it won't reach end of file
  if (!in.eof())
  {
    cerr << "Non-numeric character in plugboard file " << pboard_filename << endl;
    return NON_NUMERIC_CHARACTER;
  }

  //check through vector for repeat mappings (i.e. mapping to itself or
  //one input mapping to two outputs)
  for (unsigned int i = 0; i < pboard.size(); i++)
  {
    for (unsigned int j = 0; j < pboard.size(); j++)
    {
      if (pboard[i] == pboard[j] && i != j)
      {
	cerr << "Invalid mapping in plugboard file " << pboard_filename << ", " << temp << " is mapped twice" << endl;
	return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;

      }
    }
  }

  //check to see if the file contains an even number of parameters
  if (pboard.size() % 2 != 0 || pboard.size() > 26)
  {
    cerr << "Incorrect number of parameters in plugboard file " << pboard_filename << endl;
    return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
  }
    
  in.close();
  return NO_ERROR;
  
}

bool Plugboard::pboard_switch(char &character)
{
  int letter;
  letter = (int)(character) - (int)('A'); 

  //maps input character to its corresponding partner
  for (unsigned int j = 0; j < pboard.size(); j+=2)
  {
    if (letter == pboard[j])
      letter = pboard[j+1];
    else if (letter == pboard[j+1])
      letter = pboard[j];
  }

  character = (char)(letter + 'A');     
    
  return true;
}
