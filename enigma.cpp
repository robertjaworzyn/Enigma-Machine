#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdlib>
#include "errors.h"
#include "plugboard.h"
#include "rotor.h"
#include "reflector.h"
#include "enigma.h"

using namespace std;

Enigma_machine::Enigma_machine(char **argv, const int _argc) : argc(_argc)
{
  //calculate number of rotors input (includes position file)
  rotor_number = argc - 3;
  p = new Plugboard(argv[1]);
  r = new Reflector(argv[2]);

  for (int i = 0; i < rotor_number; i++)
    rotors.push_back(new Rotor(rotor_number));
}

Enigma_machine::~Enigma_machine()
{
  delete p;
  delete r;
  for (int i = 0; i < rotor_number; i++)
  {  
    delete rotors[i];
  }
}

int Enigma_machine::test_input(string &message)
{
  for (unsigned int i = 0; i < message.length(); i++)
  {
    //input should not be non-alphabetic or lower case
    if (!isalpha(message[i]) || !isupper(message[i]))
    {
      cerr << message[i] <<  " is not a valid input character (input characters must be upper case letters A-Z)!";
      return INVALID_INPUT_CHARACTER;

    }

  }
  return NO_ERROR;
}

int Enigma_machine::load_files(char **argv)
{
  int error_check; 

  //load plugboard and return any errors  
  error_check = p->load_pboard();
  if (error_check != 0)
    return error_check;

  //load rotors
  if (rotor_number > 0)
  {
    for (int i = 0; i < rotor_number - 1; i++)
    {
      error_check = rotors[i]->load_rotor_map(argv[1+rotor_number-i]);
      if (error_check != 0)
	return error_check;
      //link rotors (so that the next one will rotate when a notch is reached)
      rotors[i]->set_next_rotor(rotors[i+1]);
      //load and set initial rotor positions
      error_check = rotors[i]->load_rotor_pos(argv[2+rotor_number]);  
      if (error_check != 0)
	return error_check;
      rotors[i]->set_rotor_position(rotor_number-2-i);
    }
    //last rotor should not have a next_rotor pointer
    rotors[rotor_number-2]->set_next_rotor(NULL);
  }
  //load reflector
  error_check = r->load_reflector();
  if (error_check != 0)
    return error_check;

  return NO_ERROR;
}

void Enigma_machine::encrypt(char &message)
{
  //send input through plugboard
  p->pboard_switch(message);

  //rotate first rotor for every character input
  if (rotor_number != 0)
    rotors[0]->rotate();

  //send output of plugboard through any rotors
  if (rotor_number > 0)
  {
    for (int j = 0; j < rotor_number - 1; j++)
      rotors[j]->rotor_switch(message);
  } 

  //send output of rotors through reflector
  r->reflector_switch(message);

  //back through rotors
  if (rotor_number > 0)  
  {
    for (int j = rotor_number - 2; j >= 0; j--)
      rotors[j]->switch_back(message);
  }

  //back through plugboard
  p->pboard_switch(message);
}
