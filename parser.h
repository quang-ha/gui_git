#ifndef parser_h
#define parser_h

#include "monitor.h"
#include "scanner.h"

using namespace std;

class parser {
  network* netz; // instantiations of various classes for parser to use.
  devices* dmz; 
  //monitor* mmz;
  scanner smz;
  vector<nyaya_exception> exceptions;
  vector<nyaya_exception> warnings;

  /* put other stuff that the class uses internally here */
  /* also declare internal functions                     */


 public:
  bool readin ();
    /* Reads the definition of the logic system and builds the             */
    /* corresponding internal representation via calls to the 'Network'    */
    /* module and the 'Devices' module.                                    */

  parser (network* network_mod, devices* devices_mod, monitor* monitor_mod, const char* file);
    /* the constructor takes pointers to various other classes as parameters */
};

#endif /* parser_h */
