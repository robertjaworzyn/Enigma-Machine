#ifndef ROTOR_H
#define ROTOR_H
#include <vector>

using namespace std;

class Rotor
{
private:
  unsigned int rotor_number;
  vector <int> rotor_array;
  vector <int> notches;
  int position;
  int notch_number;
  vector <int> rotor_positions;
  Rotor *next_rotor;
  
public:
  Rotor(int _rotor_number) : rotor_number(_rotor_number) {
    next_rotor = NULL;
  }
  int load_rotor_map(const char *rotor_filename); /* load rotor mapping file, and check it's well formed */
  int load_rotor_pos(const char *rotor_filename); /* load initial position file, and check it's well formed */
  bool set_rotor_position(int rotor_no); /* set rotor initial position */
  bool set_next_rotor(Rotor *rot); /* link rotors */
  void print_rotor_board(); 
  bool rotor_switch(char &character); /* encrypt using rotor map on first pass (before reflector) */
  bool switch_back(char &character); /* encrypt using rotor map on way back (after reflector) */
  bool rotate(); /* increment position of rotor with respect to absolute position */

};

#endif
