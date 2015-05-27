#ifndef names_h
#define names_h

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include "name_ex.h"

using namespace std;

const int blankname = -1;   /* special name                 */

typedef int name;
typedef string namestring;

class names{

  /* put stuff that the class uses internally here */
vector<namestring> namevec;
string tablename;
public:
	namestring& operator[](std::size_t idx)  { return namevec.at(idx); };
	name add (namestring str,int,int);
	name add (namestring str);
	name retrieve (namestring str,int,int);
	name lookup (namestring str,int,int);
	name lookup(namestring str);
	bool tryretrieve (namestring str);
	void writename (name id);
    /* Prints out the given name on the console                            */

	int namelength (name id);
    /* Returns length ie number of characters in given name                */
	int size() { return namevec.size(); }

	names (string);
	names(vector<string>, string);
	string translate(name id);
    /* names initialises the name table.  This procedure is called at      */
    /* system initialisation before any of the above procedures/functions  */
    /* are used.                                                           */
};

#endif /* names_h */
