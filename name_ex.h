#ifndef name_ex_h
#define name_ex_h

#include <algorithm>
#include <iostream>

using namespace std;

const int maxlength = 20;    /* max chars in a name string   */
class nyaya_exception : public exception
{
public:
	string msg;
	int line; int pos;
	virtual const char* what() const throw();
	virtual ~nyaya_exception() throw() {}
};
class invalid_name_exception: public nyaya_exception
{
	public:
		string oname;
		virtual ~invalid_name_exception() throw(){}
};
class long_name_exception : public invalid_name_exception
{
	public:
	long_name_exception(string on_name, int line, int pos);
};
class non_alpha_exception : public invalid_name_exception
{  
  char ch;
  public:
	non_alpha_exception(string on_name, char offender, int line, int pos);
};
class name_already_exists : public invalid_name_exception
{
	string table_name;
	public:
		name_already_exists(string on_name, string tablename, int line, int pos);
		virtual ~name_already_exists() throw(){}
};
class name_doesnt_exist : public invalid_name_exception
{
	string table_name;
	public:
		name_doesnt_exist(string on_name, string tablename, int line, int pos);
		virtual ~name_doesnt_exist() throw(){};
};

#endif /* name_ex_h */
