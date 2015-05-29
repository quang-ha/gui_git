#include "name_ex.h"
#include <stdio.h>
#include <string>
using namespace std;
const string concat(char const* str, int i)
{
	char buffer[200];
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
long_name_exception::long_name_exception(const string &on_name, int l, int p, const string &file) : invalid_name_exception(on_name, l, p, file)
{	
	msg = concat(((string)("Invalid String " + oname + ": Strings longer than %d characters are not allowed in Nyaya.")).c_str(), maxlength);
}
non_alpha_exception::non_alpha_exception(const string &on_name, char offender, int l, int p, const string &file) : invalid_name_exception(on_name, l, p, file)
{
	ch = offender;
	msg = (string)("Invalid Name " + oname + ": Contains illegal character '") + ch + "', which is not allowed in Nyaya.";
}

name_already_exists::name_already_exists(const string &on_name, const string &tablename, int l, int p, const string &file) : invalid_name_exception(on_name, l, p, file)
{
	table_name = tablename;
	msg = (string)("Invalid Name " + oname + ": A " + table_name) + " with that name already exists.";
}

name_doesnt_exist::name_doesnt_exist(const string &on_name, const string &tablename, int l, int p, const string &file) : invalid_name_exception(on_name, l, p, file)
{
	table_name = tablename;
	msg = (string)("Invalid Name " + oname + ": No " + table_name) + " with that name exists.";
}
