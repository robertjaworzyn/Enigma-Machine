#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdlib>
#include "errors.h"
#include "enigma.h"
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
  int error_check;
  string message;
  
  /* need at least 3 command line parameters (executable, plugboard, reflector), and if
     rotors are included, also need a position file. therefore can't have less than three or four */ 
  if (argc < 3 || argc == 4)
  {
    cerr << "Insufficient number of command line parameters." << endl;
    return(INSUFFICIENT_NUMBER_OF_PARAMETERS);
  }
 
  Enigma_machine e(argv, argc);

  //check files are all well formed and are loaded correctly
  error_check = e.load_files(argv);
  if (error_check != 0)
    return error_check;
  
  getline(cin, message);

  //test input for non-numeric and lower case characters
  error_check = e.test_input(message);
  if (error_check != 0)
    return error_check;
  //encrypt message
  for (unsigned int i = 0; i < message.length(); i++) 
    e.encrypt(message[i]);
  cout <<  message;
  
  return NO_ERROR;
}

