#ifndef test_h
#define test_h

#include "scanner.h"
using namespace std;

void test_names_errors(names &n, string* namestr, bool dir, int no);
void test_names_values(names &n, string* namestr);
void test_names();
void test_scanner();

#endif /* test_h */