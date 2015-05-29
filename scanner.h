#ifndef scanner_h
#define scanner_h
#include "names.h"
#include "scan_ex.h"
#include <string>
#include <fstream>
using namespace std;

// The following structs and enums denote the various possible states of various elements of Nyaya
static string command_str[] = { "INCLUDE", "CREATE", "CONNECT", "DEFINE", "MONITOR", "INPUT", "OUTPUT" };
static names commands = names(vector<string>(command_str, command_str + sizeof command_str / sizeof command_str[0]), "command");
enum wordstatus { EMPTY, VALID, TRUNCATED };
struct wordstate
{
	wordstatus status;
	string fullstr;
	bool command_match;
};
enum command_type { include, create, connect, define, monitor_command, input, output };
enum punctuation_type { comma, ampersand, semicolon, arrow, dot, equals, openpar, closepar, opencurl, closecurl, percent };

// Scanner class declaration
class scanner
{
	ifstream inf;
	void getno(int &number);
	void skipcomment(bool comment);
	void getstr(namestring &str, string &fullstr, wordstatus &status);
	void backtrack(char c, int prev_pos);
public:
	char curch;
	string file_name;
	bool eofile;
	int line;
	int pos;
	scanner(const char* file, int line, int pos);
	void get();
	void skipspaces();
	void nextline();
	void nextpunc(char p);
	wordstate addword(names &table);
	wordstate getword(names &table, name &id);
	string getstring();
	int getnumber();
	punctuation_type getpunc(vector<punctuation_type>);
	punctuation_type getpunc(punctuation_type);
	punctuation_type getpunc();
	command_type getcommand();
	~scanner()
	{
		inf.close();
	}
};
#endif
