#include "screen.h"

Screen::Screen(int sda, int scl): display(128, 64, sda, scl, -1){
  this->scl = scl;
  this->sda = sda;
  return;
}

Screen::print(String str){
  //ToDo ...
  return;
}