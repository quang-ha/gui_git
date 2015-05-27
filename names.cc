#include "names.h"
#include <algorithm>
using namespace std;

/* Name storage and retrieval routines */

names::names(string name)  /* the constructor */
{
	tablename = name;
}
names::names(vector<string> namestr, string name)  /* the constructor */
{
	tablename = name;
	namevec = namestr;
}
void validate(string str, int line, int pos)
{
	if(str.length() > maxlength) throw new long_name_exception(str,line,pos);
	if(!isalpha(str[0])) throw new non_alpha_exception(str, str[0],line,pos);
	for(int i = 1; i < str.length(); i++)
	{
		if(!isalpha(str[i]) && !isdigit(str[i])) throw new non_alpha_exception(str, str[i],line,pos);
	}
}
name names::add (namestring str, int line, int pos)
{
	validate(str,line,pos);
    vector<namestring>::iterator it = find(namevec.begin(),namevec.end(),str);
	if(it < namevec.end()) 
		throw name_already_exists(str, tablename,line,pos);
	namevec.push_back(str);
	return namevec.size() - 1;
}

name names::add (namestring str)
{
	return add(str, 0, 0);
}

bool names::tryretrieve (namestring str)
{
	vector<namestring>::iterator it = find(namevec.begin(),namevec.end(),str);
	if(it == namevec.end()) return false;
	return true;
}

name names::retrieve (namestring str, int l, int p)
{
	vector<namestring>::iterator it = find(namevec.begin(),namevec.end(),str);
	if(it == namevec.end()) throw name_doesnt_exist(str, tablename,l,p);
	return it - namevec.begin();
}
name names::lookup(namestring str)
{
	return lookup(str, 0, 0);
}
name names::lookup (namestring str, int l, int p)
{
	try
	{
		return retrieve(str,l,p);
	}
	catch(name_doesnt_exist e)
	{
		return add(str,l,p);		
	}
}
void names::writename (name id)
{
    cout << namevec.at(id);
}

int names::namelength (name id)
{
	return namevec.at(id).length();
}

string names::translate (name id)
{
	return namevec.at(id);
}
