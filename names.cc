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
void validate(string str, int line, int pos, string file_name, bool allowslash)
{
	if(str.length() > maxlength) throw long_name_exception(str,line,pos,file_name);
	if (!isalpha(str[0]))
	{
		throw non_alpha_exception(str, str[0], line, pos, file_name);
	}
	for(unsigned int i = 1; i < str.length(); i++)
	{
		if(!isalpha(str[i]) && !isdigit(str[i]) && (str[i] != '/' || !allowslash))  throw non_alpha_exception(str, str[i],line,pos,file_name);
	}
}

void validate(string str, int line, int pos, string file_name)
{
	validate(str, line, pos, file_name, false);
}
name names::add (namestring str)
{
	return add(str,0,0,"none");
}

name names::retrieve (namestring str)
{
	return retrieve(str,0,0,"none");
}

name names::add (namestring str, int line, int pos, string file_name, bool allowslash)
{
	validate(str, line, pos, file_name,allowslash);
    vector<namestring>::iterator it = find(namevec.begin(),namevec.end(),str);
	if(it < namevec.end()) throw name_already_exists(str, tablename,line,pos,file_name);
	namevec.push_back(str);
	return namevec.size() - 1;
}

name names::add(namestring str, int line, int pos, string file_name)
{
	return add(str, line, pos, file_name, false);
}
bool names::tryretrieve (namestring str)
{
	vector<namestring>::iterator it = find(namevec.begin(),namevec.end(),str);
	if(it == namevec.end()) return false;
	return true;
}

name names::retrieve (namestring str, int l, int p, string file_name)
{
	vector<namestring>::iterator it = find(namevec.begin(),namevec.end(),str);
	if(it == namevec.end()){
		 throw name_doesnt_exist(str,tablename,l,p,file_name);
	 }
	return it - namevec.begin();
}
name names::lookup(namestring str)
{
	return lookup(str, 0, 0, "none");
}
name names::lookup(namestring str, bool allowslash)
{
	return lookup(str, 0, 0, "none", allowslash);
}
name names::lookup(namestring str, string file_name)
{
	return lookup(str, 0, 0, file_name);
}
name names::lookup(namestring str, int l, int p, string file_name)
{
	return lookup(str, l, p, file_name, false);
}
name names::lookup (namestring str, int l, int p, string file_name, bool allowslash)
{
	try
	{		
		return retrieve(str,l,p,file_name);
	}
	catch(name_doesnt_exist e)
	{
		return add(str,l,p,file_name,allowslash);		
	}
}
void names::writename (name id)
{
    cout << namevec.at(id) << endl;
}

int names::namelength (name id)
{
	return namevec.at(id).length();
}
