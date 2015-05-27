#ifndef scanner_h
#define scanner_h
#include "names.h"
#include "scan_ex.h"
#include <fstream>
using namespace std;

static string command_str[] = { "INCLUDE", "CREATE", "CONNECT", "DEFINE", "MONITOR", "INPUT", "OUTPUT" };
static names commands = names(vector<string>(command_str, command_str + sizeof command_str / sizeof command_str[0]), "command");

enum wordstatus { EMPTY, VALID, TRUNCATED };
struct wordstate
{
	wordstatus status;
	string fullstr;
	bool command_match;
};
enum command_type { include, create, connect, define, monitor, input, output };
enum punctuation_type { comma, ampersand, semicolon, arrow, dot, equals, openpar, closepar, opencurl, closecurl, percent };
class scanner
{
	ifstream inf;
	char curch;
	bool eofile;
	int line;
	int pos;
public:
	scanner(){}
	scanner(const char* file);
	void init(const char* file);
	void get();
	void skipcomment(bool comment);
	void skipspaces();
	void getno(int &number);
	void getstr(namestring &str, string &fullstr, wordstatus &status);
	wordstate addword(names &table);
	wordstate getword(names &table, name &id);
	int getnumber();
	punctuation_type getpunc();
	command_type getcommand();
	~scanner()
	{
		inf.close();
	}
};
#endif
