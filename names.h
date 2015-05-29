#ifndef names_h
#define names_h

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include "parse_ex.h"

using namespace std;

const int blankname = 0;   /* special name                 */

typedef int name;
typedef string namestring;

class names{

  /* put stuff that the class uses internally here */
vector<namestring> namevec;
string tablename;
public:
	namestring& operator[](std::size_t idx)  { return namevec.at(idx); };
	name add (const namestring &str);
	name add (const namestring &str,int,int,const string &);
	name add(const namestring &str, int line, int pos, const string & file_name, bool);
	name retrieve (const namestring &str);
	name retrieve (const namestring &str,int,int,const string &);
	name lookup (const namestring &str,int,int,const string &);
	name lookup(const namestring &str, int l, int p, const string &file_name, bool allowslash);
	name lookup(const namestring &str,const string &);
	name lookup(const namestring &str);
	name lookup(const namestring &str, bool);
	bool tryretrieve (const namestring &str);
	void writename (name id);
    /* Prints out the given name on the console                            */

	int namelength (name id);
    /* Returns length ie number of characters in given name                */
	int size() { return namevec.size(); }

	names (const string&);
	names(const vector<string>&, const string&);
    /* names initialises the name table.  This procedure is called at      */
    /* system initialisation before any of the above procedures/functions  */
    /* are used.                                                           */
};

#endif /* names_h */
