#include "scanner.h"
void scanner::skipcomment(bool comment)
{
	if (!comment && curch == '#' || (comment && curch != '#' && curch != '\0' && curch != '\n' && !eofile))
	{
		get();
		skipcomment(true);
	}
	else if (curch == '#' || curch == '\0' || curch == '\n')
	{
		get();
	}
}
void scanner::skipspaces()
{
	skipcomment(false);
	if (isspace(curch) && !eofile)
	{
		get();
		skipspaces();
	}
}
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
int scanner::getnumber()
{
	skipspaces();
	int number = -1;
	getno(number);
	if (number == -1) throw NaN_exception(curch,line,pos);
	return number;
}
void scanner::getstr(namestring &str, string &fullstr, wordstatus &status)
{
	if(eofile) throw end_of_file_exception(line,pos);
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
	if(status == EMPTY)	throw no_word_exception(line,pos);
}
wordstate scanner::getword(names &table, name &id)
{
	skipspaces();
	wordstate status;
	status.status = EMPTY;	
	string fullstr;
	string str;
	getstr(str,fullstr, status.status);
	status.fullstr = fullstr;
	id = table.retrieve(str,line,pos);
	status.command_match = commands.tryretrieve(str);
	return status;
}
wordstate scanner::addword(names &table)
{
	skipspaces();
	wordstate status;
	status.status = EMPTY;
	string fullstr;
	string str;
	getstr(str,fullstr, status.status);
	status.fullstr = fullstr;
	table.add(str,line,pos);
	return status;
}
punctuation_type scanner::getpunc()
{
	skipspaces();
	char c = curch;
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
	}
	if(!eofile) throw invalid_character_exception('-',line,pos);
	throw end_of_file_exception(line,pos);
}
command_type scanner::getcommand()
{
	int id;
	wordstate w = getword(commands,id);
	return (command_type)id;	
}
scanner::scanner(const char* file)
{
	line = 0; pos = 0; eofile = false;
	inf.open(file);
	if (!inf) throw cant_open_exception(line,pos);
	get();
}
void scanner::get()
{
	eofile = !inf.get(curch); pos++;
	if (curch == '\0' || curch == '\n') { line++; pos = 0; }
}
