#ifndef test_h
#define test_h

#include "interpret.h"
using namespace std;
void print_exception(nyaya_exception);
void print_exceptions(vector<nyaya_exception>);
bool test_names_errors(names &n, string* namestr, bool dir, int no);
bool test_names_values(names &n, string* namestr);
bool test_names();
bool test_scanner();
bool test_parser();
bool test_interpreter();

#endif /* test_h */
