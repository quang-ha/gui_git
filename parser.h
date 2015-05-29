#ifndef parser_h
#define parser_h

#include "scanner.h"
#include "monitor.h"
using namespace std;

const int primitives = 9;

// The pin data structure stores data relating to inputs and outputs of devices
struct pin
{
	name device;
	name device_global;
	int num;
};

// The lnk_class data structure stores information relating to links between devices
struct lnk_class
{
	pin* from;
	pin* to;
	lnk_class()
	{
		from = new pin();
		to = new pin();
	}
};

// The devs data structure contains operating information about a device
struct devs
{
	vector<name> kinds;
	vector<names*> inputs;
	vector<names*> outputs;
	names* devnames;
	vector<vector<int>*> params;
	vector<vector<int>*> refparams;
	devs();
	int size();
	namestring& operator[](std::size_t idx);
};

// The typeslist data structure containts global lists of inputs, outputs,
// devices, connections and names
struct typeslist
{
	vector<names*> inputs;
	vector<names*> outputs;
	vector<devs*> devices;
	vector<vector<pin*>*> outpins;
	vector<vector<pin*>*> inpins;
	vector<vector<lnk_class*>*> links;
	names* typenames;
	vector<int> paramnos;
	typeslist();
	void add(namestring nom, int noparams);
	void add(namestring nom, int noparams, names*, names*);
	void add(namestring nom, int noparams, devs* dvs, vector<lnk_class*>* lnks, names*, names*);
	int size();
	namestring& operator[](std::size_t idx);
};

// This function returns the kind of a device
devicekind getkind(name type_id);

// The parser_args structure provides all the information needed to create a parser
struct parser_args
{
	devs* device_list;
	typeslist* types;
	vector<pin*>* monitor_list;
	vector<lnk_class*>* links;
	vector<nyaya_exception>* exceptions;
	vector<nyaya_exception>* warnings;
	int l; int p; string file;
	parser_args(devs* de, typeslist* ty, vector<pin*>* mo, vector<lnk_class*>* lk, vector<nyaya_exception>* e, vector<nyaya_exception>* w, int line, int pos, string f)
	{
		device_list = de; types = ty; monitor_list = mo; exceptions = e; warnings = w; l = line; p = pos; file = f; links = lk;
	}
};

// This is the declaration of the parser class
class parser {

protected:

  devs* device_list;  
  typeslist* types;
  vector<pin*>* monitors;
  vector<lnk_class*>* links;
  
  scanner* smz;
  vector<nyaya_exception>* exceptions;
  vector<nyaya_exception>* warnings;

  /* put other stuff that the class uses internally here */
  /* also declare internal functions                     */


 public:
	bool readin ();
    /* Reads the definition of the logic system and builds the             */
    /* corresponding internal representation via calls to the 'Network'    */
    /* module and the 'Devices' module.                                    */
    
    /* Functions for use in readin */
          
    void parse_include();
    void parse_create();
    void parse_connect();
    void parse_define();
    void parse_monitor();
	void warn(string);
    punctuation_type type_create();
    void parse_init(name type_id);
    void parse_param(name dev_id);
	void parse_link();
    virtual bool command();
	virtual void ref_param(name dev_id);
    parser(parser_args args);
    /* the constructor takes pointers to various other classes as parameters */
};

// ylbparser inherits from parser and parses .nylb Nyaya library files
class ylbparser : public parser
{
	public:
	ylbparser (parser_args args) : parser (args) { }
	bool command();
};

// defineparser inherits from parser and parses DEFINE lines of Nyaya code
class defineparser : public parser
{
	names* output_names;
	vector<pin*>* output_pins;
	names* input_names;
	vector<pin*>* input_pins;
	names* param_names;
public:
	defineparser(parser_args args, names* params, names* i, names* o, scanner* sc, vector<pin*>* op, vector<pin*>* ip) : parser(args) { smz = sc; param_names = params; input_names = i; output_names = o; input_pins = ip; output_pins = op; }
    void parse_output();
    void parse_input();
	bool command();
	void ref_param(name dev_id);
};
#endif /* parser_h */
