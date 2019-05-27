/*
*  This is the live stats tool.  It can be run alone with -s or --standalone.
*
*/

#include <signal.h>
#include "libs/argparser.h"

bool running = TRUE;
int checktime;
char* dev = NULL;


int main(int argc, char* argv[]) {
  if(argc > 1) {
    parseFlags(*argv);
  }
  while(running) {
    break;
  }
}
