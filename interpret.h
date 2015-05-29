#ifndef interpret_h
#define interpret_h

#include "parser.h"

using namespace std;

void interpret(devices* dvs, network* net, monitor* mon, vector<nyaya_exception>* exceptions, vector<nyaya_exception>* warnings, string file);
void print_exception(nyaya_exception);
void print_exceptions(vector<nyaya_exception>);
void checknetwork(string file, network* net, devs &dvs);
#endif /* parser_h */
