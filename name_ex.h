#ifndef name_ex_h
#define name_ex_h

#include <algorithm>
#include <iostream>

using namespace std;

const unsigned int maxlength = 20;    /* max chars in a name string   */

const string concat(char const* str, int i);

class nyaya_exception : public exception
{
public:
	string msg;
	string file;
	int line; int pos;
	nyaya_exception (int l, int p, string filename) { line = l+1; pos = p; file = filename; }
	virtual const char* what() const throw();
	virtual ~nyaya_exception() throw() {}
};
class invalid_name_exception: public nyaya_exception
{
	public:
		string oname;
		invalid_name_exception(string on_name, int l, int p, string filename) : nyaya_exception(l,p,filename) { oname = on_name; }
		virtual ~invalid_name_exception() throw(){}
};
class long_name_exception : public invalid_name_exception
{
	public:
	long_name_exception(string on_name, int line, int pos, string file);
};
class non_alpha_exception : public invalid_name_exception
{  
  char ch;
  public:
	non_alpha_exception(string on_name, char offender, int line, int pos, string file);
};
class name_already_exists : public invalid_name_exception
{
	string table_name;
	public:
		name_already_exists(string on_name, string tablename, int line, int pos, string file);
		virtual ~name_already_exists() throw(){}
};
class name_doesnt_exist : public invalid_name_exception
{
	string table_name;
	public:
		name_doesnt_exist(string on_name, string tablename, int l, int p, string file);
		virtual ~name_doesnt_exist() throw(){};
};

#endif /* name_ex_h */
