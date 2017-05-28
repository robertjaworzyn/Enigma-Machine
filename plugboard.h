#ifndef PLUGBOARD_H
#define PLUGBOARD_H
#include <vector>

using namespace std;

class Plugboard
{
private:
  char *pboard_filename;
  vector <int> pboard;
  //int counter;


public:
  Plugboard(char *_pboard_filename) : pboard_filename(_pboard_filename) {
    //counter = 0;
  }
  int load_pboard();
  bool pboard_switch(char &character);
};

#endif
