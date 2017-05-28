#ifndef REFLECTOR_H
#define REFLECTOR_H
#include <vector>

using namespace std;

class Reflector
{
private:
  char *reflector_filename;
  vector <int> reflector_board;

public:
  Reflector(char *_reflector_filename) : reflector_filename(_reflector_filename) {
  }
  int load_reflector();
  bool reflector_switch(char &character);
};

#endif
