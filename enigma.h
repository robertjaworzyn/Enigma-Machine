#ifndef ENIGMA_H
#define ENIGMA_H
#include <string>
#include <vector>
#include "reflector.h"
#include "plugboard.h"
#include "rotor.h"

using namespace std;

class Enigma_machine
{
private:
  Plugboard *p;
  Reflector *r;
  int argc;
  int rotor_number;
  vector <Rotor *> rotors;
  char *pboard_filename;
  char *reflector_filename;
  
public:
  Enigma_machine(char **argv, const int _argc);
  ~Enigma_machine();
  int test_input(string &message); /* check string (before encryption) for any invalid characters */
  int load_files(char **argv); /* load all plugboard, rotor, and reflector files */
  void encrypt(char &message); /* encrypt character by character by sending through
				  plugboard, rotors, reflector, back through rotors and plugboard */

};

#endif
