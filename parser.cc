#include <iostream>
#include "parser.h"

using namespace std;

/* The parser for the circuit definition files */


bool parser::readin (void)
{
	switch(smz.getcommand())
	{
		case include:
		default:
		break;
	}
}

parser::parser (network* network_mod, devices* devices_mod
//monitor* monitor_mod
, const char* file)
{
	smz.init(file);
	netz = network_mod;
	dmz = devices_mod;
	//mmz = monitor_mod;
}
