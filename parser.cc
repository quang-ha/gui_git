#include "parser.h"
#include "parse_ex.h"
using namespace std;

/******************************************************************** 
*
*    The parser for the circuit definition files
*
*/

// Constructor for the typeslist data structure
typeslist::typeslist()
{
	typenames = new names("type");
	add("SWITCH", 1);
	add("CLOCK", 1);
	add("AND", 1);
	add("NAND", 1); 
	add("OR", 1);
	add("NOR", 1);
	names* xin = new names("input"); xin->add("I1"); xin->add("I2");
	add("XOR", 0, xin, new names("output"));
	names* din = new names("input"); names* dout = new names("output");
	add("DTYPE", 0,din,dout);
	din->add("DATA"); din->add("CLK"); din->add("SET"); din->add("CLEAR");
	dout->add("Q"); dout->add("QBAR");
}

// Function to add a new type to typeslist
void typeslist::add(namestring nom, int noparams)
{
	add(nom, noparams, NULL, new vector<lnk_class*>(), new names("input"), new names("output"));
}

// Function to add a new tpe to typeslist including names of inputs and outputs
void typeslist::add(namestring nom, int noparams, names* i, names* o)
{
	add(nom, noparams, NULL, new vector<lnk_class*>(), i, o);
}

// Function to add a full set of data to typeslist
void typeslist::add(namestring nom, int noparams, devs* dvs, vector<lnk_class*>* lnks, names* input, names* output)
{
	typenames->add(nom); paramnos.push_back(noparams); devices.push_back(dvs); links.push_back(lnks); inputs.push_back(input); outputs.push_back(output);
}

// Function to return the size of the typeslist
int typeslist::size() { return typenames->size(); }
namestring& typeslist::operator[](std::size_t idx) { return (*typenames)[idx]; };

// Function to return the kind of a device
devicekind getkind(name type_id)
{
	return type_id < primitives ? (devicekind)type_id : custom;
}

// Constructor for the devs data type
devs::devs()
{
	devnames = new names("device");
}

// Function to return the size of the devs
int devs::size() { return devnames->size(); }
namestring& devs::operator[](std::size_t idx) { return (*devnames)[idx]; };

// Constructor for the parser class
parser::parser (parser_args args)
{
	smz = new scanner(args.file.c_str(), args.l, args.p);
	device_list = args.device_list;
	types = args.types;
	monitors = args.monitor_list;
	links = args.links;
	exceptions = args.exceptions;
	warnings = args.warnings;
}

// Function to parse through a .nya Nyaya file
bool parser::readin (void)
{
	try
	{
		while (!smz->eofile)
		{
			try
			{	
				if (!command()) break;
			}
			catch(end_of_file_exception e)
			{				
				throw e;
			}
			catch (nyaya_exception e)
			{
				exceptions->push_back(e);
				smz->nextline();
			}
			smz->skipspaces();
		}	
		
	}
	catch (end_of_file_exception e2)
	{		
		exceptions->push_back(e2);
	}
	return true;
}

// Function to parse a command and switch to the relevant function
bool parser::command()
{		
	switch(smz->getcommand())
	{
		case include:
					parse_include(); break;
		case create:
					parse_create(); break;
		case connect:
					parse_connect(); break;
		case define:
					parse_define(); break;
		case monitor_command:
					parse_monitor(); break;
		default: throw invalid_command_exception(smz->line,smz->pos, smz->file_name);
	}
	return true;
}

// Function to parse through a .nylb Nyaya library file
bool ylbparser::command (void)
{
	switch(smz->getcommand())
	{
		case define:
			parse_define(); return true;
		default: throw invalid_command_exception(smz->line,smz->pos,smz->file_name);
	}
	
}

// Function to parse an INCLUDE line of Nyaya code
void parser::parse_include()
{
	string file = smz->getstring();
	if(smz->getpunc(dot) == dot)
	{
		string ext = smz->getstring();
		if(ext == "nylb")
		{				
			ylbparser newparser(parser_args(device_list, types, monitors, links, exceptions, warnings, 0, 0, (file + ".nylb").c_str()));
			newparser.readin();
			if(smz->getpunc() != semicolon) throw syntax_exception("semicolon", smz->line, smz->pos, smz->file_name);
		}
		else throw invalid_filename_exception(file + "." + ext,smz->line,smz->pos, smz->file_name);	
	}
	else throw invalid_filename_exception(file,smz->line,smz->pos, smz->file_name);
}


// Function to parse a CREATE line of Nyaya code
void parser::parse_create()
{
	punctuation_type p;
	do
	{
		p = type_create();
	}
	while(p == ampersand);
	if(p != semicolon) throw syntax_exception("semicolon, ampersand or comma", smz->line, smz->pos, smz->file_name);
}

// Function to parse a typecreate section of a CREATE line of Nyaya code
punctuation_type parser::type_create()
{
	name type_id;
	smz->getword(*(types->typenames),type_id);		
	punctuation_type p;
	do
	{
		parse_init(type_id);
		p = smz->getpunc();
	}
	while(p == comma);
	return p;
}

// Function to provide a warning message when a command or type name is
// used as the name of a variable
void parser::warn(string message)
{
	nyaya_exception warning(smz->line, smz->pos, smz->file_name);
	warning.msg = message;
	warnings->push_back(warning);
}

// Function to parse an init section of a CREATE line of Nyaya code
void parser::parse_init(name type_id)
{
	if (smz->addword(*(device_list->devnames)).command_match)
	{
		warn("Warning: It is not recommended to use the name of a command as the name of a device in Nyaya.");
	}
	else if (types->typenames->tryretrieve((*(device_list->devnames))[device_list->size() - 1]))
	{
		warn("Warning: It is not recommended to use the name of a type as the name of a device in Nyaya.");
	}
	device_list->kinds.push_back(type_id);
	
	// This section deals with several possibilities, that the next symbol could be ), an integer or a name,
	// and also handles potenial errors this may cause
	if (smz->getpunc() != openpar) throw syntax_exception("(", smz->line, smz->pos, smz->file_name);
	int count = 0;
	try
	{
		if (smz->getpunc() != closepar) throw syntax_exception(")", smz->line, smz->pos, smz->file_name);
		while (static_cast<int>(device_list->params.size()) < device_list->size()) device_list->params.push_back(new vector<int>());
	}
	catch (invalid_character_exception e)
	{
		if (isdigit(e.sym) || isalpha(e.sym))
		{
			punctuation_type p;
			do
			{
				parse_param(device_list->size() - 1);
				count++;
				p = smz->getpunc();
			} while (p == comma);
			if (p != closepar) throw syntax_exception(")", smz->line, smz->pos, smz->file_name);
		}
		else
		{
			throw e;
		}
	}
	
	// This section of code validates the type and number of parameters in the devide bein created, and also
	// creates and populates the pins on a given device
	if (count != types->paramnos.at(type_id)) throw wrong_param_number(smz->file_name, smz->line, smz->pos, types->paramnos.at(type_id));
	while (static_cast<int>(types->inputs.size()) <= type_id) types->inputs.push_back(new names("input"));
	while (static_cast<int>(device_list->inputs.size()) < device_list->size()) device_list->inputs.push_back(new names("input"));
	while (static_cast<int>(types->outputs.size()) <= type_id) types->outputs.push_back(new names("output"));
	while (static_cast<int>(device_list->outputs.size()) < device_list->size()) device_list->outputs.push_back(new names("output"));

	if (types->inputs.at(type_id)->size() == 0 && types->paramnos.at(type_id) > 0)
	{
		for (int i = 1; i <= device_list->params.at(device_list->size() - 1)->at(0); i++)
		{
			device_list->inputs.at(device_list->size() - 1)->add(concat("I%d", i));
		}
	}
	else
	{
		for (int i = 0; i < static_cast<int>(types->inputs.at(type_id)->size()); i++)
		{
			device_list->inputs.at(device_list->size() - 1)->add((*(types->inputs.at(type_id)))[i]);
		}
	}
	if (types->outputs.at(type_id)->size() != 0)
	{
		for (int i = 0; i < static_cast<int>(types->outputs.at(type_id)->size()); i++)
		{
			device_list->outputs.at(device_list->size() - 1)->add((*(types->outputs.at(type_id)))[i]);
		}
	}
}

// Function to parse a parameter from a CREATE line of Nyaya code
void parser::parse_param(name dev_id)
{
	try
	{
		while (static_cast<int>(device_list->params.size()) <= dev_id) device_list->params.push_back(new vector<int>());
		device_list->params.at(dev_id)->push_back(smz->getnumber());
	}
	catch (NaN_exception e)
	{
		ref_param(dev_id);
	}
}

// Function to throw an exception for an invalid parameter
void parser::ref_param(name dev_id)
{
	throw invalid_refparam_exception(smz->file_name, smz->line, smz->pos, smz->getstring());
}

// Function to parse a reference parameter in a DEFINE line of Nyaya code
void defineparser::ref_param(name dev_id)
{
	name param;
	smz->getword(*param_names, param);
	while (static_cast<int>(device_list->params.size()) <= dev_id) device_list->params.push_back(new vector<int>());
	while (static_cast<int>(device_list->refparams.size()) <= dev_id) device_list->refparams.push_back(new vector<int>());
	device_list->params.at(dev_id)->push_back(-1);
	device_list->refparams.at(dev_id)->push_back(param);

}

// Function to parse a CONNECT line of Nyaya code
void parser::parse_connect()
{
	punctuation_type p;
	do
	{
		parse_link();
		p = smz->getpunc();
	} 
	while (p == comma);
	if (p != semicolon) throw syntax_exception("semicolon", smz->line, smz->pos, smz->file_name);
}

// Function to parse a link section of a CONNECT line of Nyaya code
void parser::parse_link() {
	lnk_class* lnk = new lnk_class();
	lnk->from->num = 0;
	smz->getword(*(device_list->devnames), lnk->from->device);
	punctuation_type p = smz->getpunc();
	if (p == dot)
	{
		smz->getword(*(device_list->outputs[lnk->from->device]), lnk->from->num);
		p = smz->getpunc();
	}
	if(p != arrow) throw syntax_exception("arrow", smz->line, smz->pos, smz->file_name);
	smz->getword(*(device_list->devnames), lnk->to->device);
	if (smz->getpunc() != dot) throw syntax_exception("dot", smz->line, smz->pos, smz->file_name);
	smz->getword(*(device_list->inputs[lnk->to->device]), lnk->to->num);
	lnk->to->device_global = lnk->to->device;
	lnk->from->device_global = lnk->from->device;
	links->push_back(lnk);
}


// Function to parse a MONITOR line of Nyaya code
void parser::parse_monitor()
{
	punctuation_type p = openpar;
	do
	{
		pin* o = new pin(); o->num = 0;
		smz->getword(*(device_list->devnames), o->device);
		p = smz->getpunc();
		if (p == dot)
		{
			smz->getword(*(device_list->outputs[o->device]), o->num);
			p = smz->getpunc();
		}
		o->device_global = o->device;
		monitors->push_back(o);
	} while (p == comma);
	if (p != semicolon) throw syntax_exception("semicolon", smz->line, smz->pos, smz->file_name);

}

// Function to parse an INPUT section of a DEFINE line of Nyaya code
void defineparser::parse_input()
{
	punctuation_type p = openpar;
	int count = 0;
	do
	{
		pin* in = new pin();
		if (smz->addword(*(input_names)).command_match)
		{
			warn("Warning: It is not recommended to use the name of a command as the name of an input in Nyaya.");
		}
		else if (types->typenames->tryretrieve((*(input_names))[input_names->size() - 1]))
		{
			warn("Warning: It is not recommended to use the name of a type as the name of an input in Nyaya.");
		}
		if (smz->getpunc() != equals) throw syntax_exception("equals", smz->line, smz->pos, smz->file_name);
		smz->getword(*(device_list->devnames), in->device);
		if (smz->getpunc() != dot) throw syntax_exception("dot", smz->line, smz->pos, smz->file_name);
		smz->getword(*(device_list->inputs.at(in->device)), in->num);
		p = smz->getpunc();
		count++;
		in->device_global = in->device;
		input_pins->push_back(in);
	} while (p == comma);
	if (count > 16) throw too_many_inputs(smz->file_name, smz->line, smz->pos);
	if (p != semicolon) throw syntax_exception("semicolon", smz->line, smz->pos, smz->file_name);
	
}

// Function to parse an OUTPUT section of a DEFINE line of Nyaya code
void defineparser::parse_output()
{
	punctuation_type p = openpar;
	int count = 0;
	do
	{
		pin* out = new pin();
		out->num = 0;
		if (smz->addword(*(output_names)).command_match)
		{
			warn("Warning: It is not recommended to use the name of a command as the name of an output in Nyaya.");
		}
		else if (types->typenames->tryretrieve((*(output_names))[output_names->size() - 1]))
		{
			warn("Warning: It is not recommended to use the name of a type as the name of an output in Nyaya.");
		}
		if(smz->getpunc() != equals) throw syntax_exception("equals", smz->line, smz->pos, smz->file_name);
		smz->getword(*(device_list->devnames), out->device);
		p = smz->getpunc();
		if (p == dot)
		{
			smz->getword(*(device_list->outputs.at(out->device)), out->num);
			p = smz->getpunc();
		}
		out->device_global = out->device;
		output_pins->push_back(out);
	} while (p == comma);
	if (count > 8) throw too_many_outputs(smz->file_name, smz->line, smz->pos);
	if (p != semicolon) throw syntax_exception("semicolon", smz->line, smz->pos, smz->file_name);
	
}

// Function to detect CREATE and CONNECT sections of a DEFINE line of Nyaya code
bool defineparser::command()
{
	switch (smz->getcommand())
	{
	case create:
		parse_create(); return true;
	case connect:
		parse_connect(); return true;
	case input:
		return false;
	default: throw invalid_command_exception(smz->line, smz->pos, smz->file_name);
	}
}

// Function to parse a DEFINE line of Nyaya code
// Begins by initialising variables and fetching the name and reference parameters
void parser::parse_define()
{
	names paramnames("parameter");
	int count = 0;
	if (smz->addword(*(types->typenames)).command_match)
	{
		warn("Warning: It is not recommended to use the name of a command as the name of a type in Nyaya.");
	}
	if (smz->getpunc() != openpar) throw syntax_exception("(", smz->line, smz->pos, smz->file_name);
	try
	{
		if (smz->getpunc() != closepar) throw syntax_exception(")", smz->line, smz->pos, smz->file_name);
	}
	catch (invalid_character_exception e)
	{
		if (isdigit(e.sym) || isalpha(e.sym))
		{
			punctuation_type p;
			do
			{
				if (smz->addword(paramnames).command_match)
				{
					warn("Warning: It is not recommended to use the name of a command as the name of a parameter in Nyaya.");
				}
				else if (types->typenames->tryretrieve(paramnames[paramnames.size() - 1]))
				{
					warn("Warning: It is not recommended to use the name of a type as the name of a parameter in Nyaya.");
				}
				count++;
				p = smz->getpunc();
			} while (p == comma);
			if (p != closepar) throw syntax_exception(")", smz->line, smz->pos, smz->file_name);
		}
		else
		{
			throw e;
		}
	}
	types->paramnos.push_back(count);
	
	// This section of code peforms some validation and creates and initialises variables to be used to describe the device being defined
	if (smz->getpunc() != opencurl) throw syntax_exception( "{" , smz->line , smz->pos , smz->file_name );
	devs* d = new devs(); vector<lnk_class*>* lnk = new vector<lnk_class*>();
	types->devices.push_back(d); types->links.push_back(lnk);
	while (static_cast<int>(types->inputs.size()) <= types->size()) types->inputs.push_back(new names("input"));
	while (static_cast<int>(types->inpins.size()) <= types->size()) types->inpins.push_back(new vector<pin*>());
	while (static_cast<int>(types->outputs.size()) <= types->size()) types->outputs.push_back(new names("output"));
	while (static_cast<int>(types->outpins.size()) <= types->size()) types->outpins.push_back(new vector<pin*>());
	defineparser dpz(parser_args(d, types, monitors, lnk, exceptions, warnings, smz->line, smz->pos, smz->file_name) , &paramnames , types->inputs.at(types->size() - 1), types->outputs.at(types->size() - 1), smz, types->outpins.at(types->size() - 1), types->inpins.at(types->size() - 1));
	
	// The main body of the define is then parsed, with error handling included
	if (dpz.smz->getcommand() != create) throw invalid_command_exception(dpz.smz->line, dpz.smz->pos, dpz.smz->file_name);
	dpz.parse_create();
	dpz.readin();
	smz->line = dpz.smz->line; smz->pos = dpz.smz->pos;
	dpz.parse_input();
	if (dpz.smz->getcommand() != output) throw invalid_command_exception(dpz.smz->line, dpz.smz->pos, dpz.smz->file_name);
	dpz.parse_output();
	if (smz->getpunc() != closecurl) throw syntax_exception("}", smz->line, smz->pos, smz->file_name);
	if (smz->getpunc() != semicolon) throw syntax_exception("semicolon", smz->line, smz->pos, smz->file_name);
}

