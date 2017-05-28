#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdlib>
#include "errors.h"
#include "rotor.h"

int Rotor::load_rotor_map(const char *rotor_filename)
{
  int counter(0), temp;
  ifstream in;
  in.open(rotor_filename);
  if (!in)
  {
    cerr << "FAILED TO LOAD ROTOR FILE" << endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  
  while (counter < 26 && in >> temp)
  {
    //check for in stream failure
    if (in.rdstate() > 0) break;
    
    if (temp < 0 || temp > 25)
    {
      cerr << "Invalid rotor index (should be between 0 and 25)" << endl;
      return INVALID_INDEX;
    }

    rotor_array.push_back(temp);
    counter++;
  }

  //read in notch positions
  while (in >> temp)
  {
    if (in.rdstate() > 0) break;
    notches.push_back(temp);
  }

  //trying to read a non-numeric character into an int array means
  //in stream fails, and it won't reach end of file
  if (!in.eof())
  {
    cerr << "Non-numeric character for mapping in rotor file " << rotor_filename;
    return NON_NUMERIC_CHARACTER;
  }

  //check through vector for repeat mappings (i.e. mapping to itself or
  //one input mapping to two outputs)
  for (unsigned int i = 0; i < rotor_array.size(); i++)
  {
    for (unsigned int j = 0; j < rotor_array.size(); j++)
    {
      if (rotor_array[i] == rotor_array[j] && i != j)
      {
	cerr << "Invalid mapping in rotor file " << rotor_filename << ", " << temp << " is mapped twice" << endl;
	return INVALID_ROTOR_MAPPING;

      }
    }
  }

  //check to see if all inputs are correctly mapped
  if (counter < 26)
  {
    cerr << "Not all inputs mapped in rotor file: " << rotor_filename << endl;
    return INVALID_ROTOR_MAPPING;
  }

  notch_number = notches.size();
    
  return NO_ERROR;
}

int Rotor::load_rotor_pos(const char *rotor_filename)
{
  ifstream in;
  int temp;
  in.open(rotor_filename);

  
  while (in >> temp)
  {
    if (in.rdstate() > 0) break;
    if (temp < 0 || temp > 25)
    {
      cerr << "Invalid rotor index (should be between 0 and 25)" << endl;
      return INVALID_INDEX;
    }
    rotor_positions.push_back(temp);
  }

  if (!in.eof())
  {
    cerr << "Non-numeric character in rotor positions file " << rotor_filename << endl;
    return NON_NUMERIC_CHARACTER;
  }

  //check that there enough starting positions in the position file for the existing rotors
  if (rotor_positions.size() < rotor_number - 1)
  {
    cerr << "Different number of rotor and rotor starting positions in pos file" << endl;
    return NO_ROTOR_STARTING_POSITION;
  }
  
  
  in.close();
  return NO_ERROR;
}

bool Rotor::set_rotor_position(int rotor_no)
{
  for (int i = 0; i < rotor_positions[rotor_no]; i++)
    this->rotate();
 
  return true;
}

void Rotor::print_rotor_board()
{
  for (int i = 0; i < 30; i++)
    cout << rotor_array[i] << " ";
  cout << endl;
}

bool Rotor::set_next_rotor(Rotor *rot)
{
  next_rotor = rot;
  return true;
}

bool Rotor::rotor_switch(char &character)
{
  int counter, letter, temp;

  //convert input character into an integer, taking into account the
  //relative position of the rotor
  letter = (int)(character) - (int)('A') + position ; 
  if (letter > 25)
    letter = letter -26;
 
  //checks whether this rotor has rotated past notch and rotates the next rotor if so
  for (int i = 0; i < notch_number; i++)
  {
    if (position == notches[i] && next_rotor != NULL)
      next_rotor->rotate();
  }

  //swaps letter with it's partner, outputs absolute position (not relative)
  for (counter = 0; counter < 26; counter++)
  {
    if (letter == counter)
    {
      temp = (rotor_array[counter] + 'A' - position);
      break;
    }
      
  }
  
  if (temp < 'A')
    temp += 26;
  
  character = (char)temp;
  
  return true;
}

bool Rotor::switch_back(char &character)
{
  int counter, letter, temp;

  //calculate input of letter depending on relative position of the rotor
  letter = (int)(character) - 'A' + position;
  if (letter > 25)
    letter = letter - 26;
  
  //swaps the letter with it's partner, outputs absolute position
  for (counter = 0; counter < 26; counter++)
  {
    if (letter == rotor_array[counter])
    {
      temp = (counter + 'A' - position);
      break;
    }
  }

  if (temp <'A')
    temp += 26;
  
  character = (char)temp;
  
  return true;
}

bool Rotor::rotate()
{
  //a rotation causes relative position of the rotor to increment
  position++;
  position = position % 26;
  return true;
}
