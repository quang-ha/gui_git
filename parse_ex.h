#ifndef parseex_h
#define parseex_h
#include <algorithm>
#include <string>
#include "name_ex.h"
using namespace std;

class other_exception : public nyaya_exception
{
	public:
	other_exception(string file, int l, int p);
};

class bad_device_exception : public nyaya_exception
{
	public:
	bad_device_exception(string file, int l, int p);
};

class invalid_filename_exception : public nyaya_exception
{
public:
	string filename;
	invalid_filename_exception(string file, int l, int p, string containing_file);
	virtual ~invalid_filename_exception() throw() {}
};

class invalid_refparam_exception : public nyaya_exception
{
public:
	string name;
	invalid_refparam_exception(string file, int l, int p, string name);
	virtual ~invalid_refparam_exception() throw() {}
};


class too_many_outputs : public nyaya_exception
{
public:
	too_many_outputs(string file, int l, int p);
};

class too_many_inputs : public nyaya_exception
{
public:
	too_many_inputs(string file, int l, int p);
};
class wrong_param_number : public nyaya_exception
{
public:
	wrong_param_number(string file, int l, int p, int num);
};

class invalid_command_exception : public nyaya_exception
{
public:
	invalid_command_exception(int l, int p, string containing_file);
};

class syntax_exception : public nyaya_exception
{
public:
	syntax_exception(string expected, int l, int p, string containing_file);
};

class make_fail_exception : public nyaya_exception
{
public:
	make_fail_exception(string device, string filename);
};

class connect_fail_exception : public nyaya_exception
{
public:
	connect_fail_exception(string from, string to, string filename);
};

class bad_network_exception : public nyaya_exception
{
public:
	bad_network_exception(string input, string filename);
};

class monitor_exception : public nyaya_exception
{
public:
	monitor_exception(string filename);
};
#endif

