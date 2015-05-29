#include "scanner.h"

/***********************************************************
*
*    This file contains methods to manipulate .nya and .nylb
*    files as directed by the parser, including some error
*    handling.
* 
*/

// Constructor
scanner::scanner(const char* file, int l, int p)
{
	file_name = file;
	line = l; pos = p; eofile = false;
#ifdef _WIN32
	inf.open(((string)"C:/Users/Konstantinos/SkyDrive/MEng/IIA/Projects/Software/Code/GF2/parser/") + file);
#else
	inf.open(file);
#endif // _WIN32	
	if (!inf) throw cant_open_exception(line, pos, file_name);
	get();
}

// Function to skip comments in Nyaya code
void scanner::skipcomment(bool comment)
{
	if ((!comment && curch == '#') || (comment && curch != '#' && curch != '\0' && curch != '\n' && !eofile))
	{
		get();
		skipcomment(true);
	}
	else if (curch == '#' || curch == '\0' || curch == '\n')
	{
		get();
	}
}

// Function to skip spaces in Nyaya code
void scanner::skipspaces()
{
	skipcomment(false);
	if (isspace(curch) && !eofile)
	{
		get();
		skipspaces();
	}
}

// Function to convert strings into numbers for use in getnumber()
void scanner::getno(int &number)
{
	if (isdigit(curch) && !eofile)
	{
		if(number == -1) number = 0;
		number = number*10 + (curch - '0');
		get();
		getno(number);
	}
}

// Function to return an integer including error handling
int scanner::getnumber()
{
	skipspaces();
	int number = -1;
	getno(number);
	if (number == -1) throw NaN_exception(curch,line,pos, file_name);
	return number;
}

// Function containing validation, error handling and truncation of strings
void scanner::getstr(namestring &str, string &fullstr, wordstatus &status)
{
	if(eofile) { throw end_of_file_exception(line,pos, file_name); }
	if (isdigit(curch) || isalpha(curch))
	{
		if(status == EMPTY)
		{
			str = "";
			status = VALID;
		}
		fullstr += curch;
		if(str.length() < 8)
		{
			str += curch;
		}
		else
		{
			status = TRUNCATED;
		}
		get();
		getstr(str,fullstr,status);
	}
	if(status == EMPTY)	throw no_word_exception(line,pos,file_name);
}

// Function to return a string
string scanner::getstring()
{
	skipspaces();	
	namestring shortstr; wordstatus ws;
	string str;
	getstr(shortstr,str,ws);
	return str;
}

// Function using the other string handling functions providing extra useful features
wordstate scanner::getword(names &table, name &id)
{
	skipspaces();
	wordstate status;
	status.status = EMPTY;	
	string fullstr;
	string str;
	getstr(str,fullstr, status.status);
	status.fullstr = fullstr;
	id = table.retrieve(str,line,pos,file_name);
	status.command_match = commands.tryretrieve(str);
	return status;
}

// Function to add the current word to its corresponding table of names
wordstate scanner::addword(names &table)
{
	skipspaces();
	wordstate status;
	status.status = EMPTY;
	string fullstr;
	string str;
	getstr(str,fullstr, status.status);
	status.fullstr = fullstr;
	table.add(str,line,pos,file_name);
	status.command_match = commands.tryretrieve(str);
	return status;
}

// Function to take the scanner back one character
void scanner::backtrack(char c, int p)
{
	curch = c; pos--; inf.seekg(-1, ios_base::cur); if (pos == -1) { pos = p; line--; }
	throw invalid_character_exception(c, line, pos, file_name);
}

// Function to detect a desired piece of punctuation
punctuation_type scanner::getpunc(punctuation_type desired)
{
	vector<punctuation_type> v;
	v.push_back(desired);
	return getpunc(v); 
}

//  Function to enable search for several potential pieces of punctuation
punctuation_type scanner::getpunc(vector<punctuation_type> desired)
{
	skipspaces();
	char c = curch; int prev_pos = pos;
	punctuation_type p = getpunc();
	for (unsigned int i = 0; i < desired.size(); i++)
	{
		if (p == desired.at(i)) return p;
	}
	backtrack(c, prev_pos);
	return p;
}

// Function to detect and return a piece of punctuation, including error handling
punctuation_type scanner::getpunc()
{
	skipspaces();
	char c = curch; int prev_pos = pos;
	get();
	switch(c)
	{
		case ';':
			return semicolon;
		case '.':
			return dot;
		case ',':
			return comma;
		case '&':
			return ampersand;
		case '=':
			return equals;
		case '(':
			return openpar;
		case ')':
			return closepar;
		case '{':
			return opencurl;
		case '}':
			return closecurl;
		case '-':
			if (curch == '>') { get(); return arrow; }
			else throw invalid_character_exception('-',line,pos,file_name);
	}
	if(!eofile) backtrack(c,prev_pos);
	throw end_of_file_exception(line,pos,file_name);
}

// Function to return a Nyaya command
command_type scanner::getcommand()
{
	int id;
	wordstate w = getword(commands,id);
	return (command_type)id;	
}

// Function to retrieve the next character
void scanner::get()
{
	if(file_name != "none")
	//cout << curch << " line " << (line+1) << " pos " << pos << endl;
	eofile = !inf.get(curch); pos++;
	if (curch == '\0' || curch == '\n') { line++; pos = 0; }
}


// Function to find the next semicolon and therefore the next line of Nyaya code, for use in error handling
void scanner::nextline()
{
	nextpunc(';');
}

// Function to find the next instance of a given type of punctuation
void scanner::nextpunc(char c)
{
	while(curch != c){
		 if(eofile) throw end_of_file_exception(line,pos,file_name);
		 get();
	 }
	get();	
}
