#include "parse_ex.h"
using namespace std;
	
bad_device_exception::bad_device_exception(string file, int l, int p) : nyaya_exception(l,p,file)
{
	msg = "Bad Device Exception: Attempt was made to use an invalid device. Check Nyaya code.";
}
	
other_exception::other_exception(string file, int l, int p) : nyaya_exception(l,p,file)
{
	msg = file == "none" ? "Unhandled Elea Exception: We're not sure what you just did, but it was not a good idea. We'll just pretend this never happened and let you continue from where you were before." : "Unhandled Nyaya Exception: There are one or more mistakes in this part of your Nyaya code that are so twisted and bizarre that no-one in our crack team of developers and testers could anticipate the posssibility of anyone making them. Please use your common sense to correct them.";
}

invalid_filename_exception::invalid_filename_exception(string file, int l, int p, string calling_file) : nyaya_exception(l,p,calling_file)
{
	filename = file;
	msg = "Parser Error: Invalid filename " + filename + ". Nyaya library files should be named xxx.nylb.";
}

invalid_command_exception::invalid_command_exception(int l, int p, string calling_file) : nyaya_exception(l,p,calling_file)
{
	msg = "Parser Error: Line does not begin with a valid Nyaya command.";
}


too_many_outputs::too_many_outputs(string file, int l, int p) : nyaya_exception(l, p, file)
{
	msg = "Too many outputs. Maximum allowable number is 8.";
}

too_many_inputs::too_many_inputs(string file, int l, int p) : nyaya_exception(l, p, file)
{
	msg = "Too many inputs. Maximum allowable number is 16.";
}

syntax_exception::syntax_exception(string expected, int l, int p, string calling_file) : nyaya_exception(l,p,calling_file)
{
	msg = "Invalid Nyaya syntax. Expected " + expected + " .";
}

invalid_refparam_exception::invalid_refparam_exception(string file, int l, int p, string n) : nyaya_exception(l, p, file)
{
	name = n;
	msg = "Invalid parameter: Reference " + n + " does not define a valid parameter in this context.";
}

wrong_param_number::wrong_param_number(string file, int l, int p, int n) : nyaya_exception(l, p, file)
{
	msg = concat("Wrong number of parameters: Devices of this type require %d parameters.",n);
}

make_fail_exception::make_fail_exception(string device, string file) : nyaya_exception(-1, -1, file)
{
	msg = "Couldn't create device " + device + ": Please check that its declaration is valid Nyaya syntax and that all parameters are correctly specified.";
}

connect_fail_exception::connect_fail_exception(string from, string to, string file) : nyaya_exception(-1, -1, file)
{
	msg = "Couldn't connect " + from + " to " + to + " : Please check that this CONNECT command is valid Nyaya syntax.";
}

bad_network_exception::bad_network_exception(string input, string file) : nyaya_exception(-1, -1, file)
{
	msg = "Invalid circuit: Unconnected input " + input + ".";
}

monitor_exception::monitor_exception(string file) : nyaya_exception(-1, -1, file)
{
	msg = "Error reading monitor command: Please ensure command is valid Nyaya syntax.";
}
