#include "name_ex.h"
#include <stdio.h>
#include <string>
using namespace std;
const string concat(char const* str, int i)
{
	char buffer[50];
#ifdef _WIN32
	sprintf_s(buffer, str, i);
#else
	sprintf(buffer, str, i);
#endif
	return buffer;
}
const char* nyaya_exception::what() const throw()
{
	return msg.c_str();
}
long_name_exception::long_name_exception(string on_name, int l, int p)
{
	line = l; pos = p;
	oname = on_name;
	msg = concat(((string)("Invalid Name " + oname + ": More than %d characters.")).c_str(), maxlength);
}
non_alpha_exception::non_alpha_exception(string on_name, char offender, int l, int p)
{
	line = l; pos = p;
	oname = on_name;
	ch = offender;
	msg = (string)("Invalid Name " + oname + ": Contains illegal character '") + ch;
}

name_already_exists::name_already_exists(string on_name, string tablename, int l, int p)
{
	line = l; pos = p;
	oname = on_name;
	table_name = tablename;
	msg = (string)("Invalid Name " + oname + ": A " + table_name) + " with that name already exists. Name: " + oname;
}	
name_doesnt_exist::name_doesnt_exist(string on_name, string tablename, int l, int p)
{
	line = l; pos = p;
	oname = on_name;
	table_name = tablename;
	msg = (string)("Invalid Name " + oname + ": No " + table_name) + " with that name exists.";
}

