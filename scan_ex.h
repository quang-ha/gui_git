#include <algorithm>
#include "name_ex.h"
using namespace std;

class invalid_character_exception : public nyaya_exception
{
	public:
	char sym;
	invalid_character_exception(char symbol, int l, int p, string file);
};

class cant_open_exception : public nyaya_exception
{
public:
	cant_open_exception(int l, int p, string file);
};

class end_of_file_exception : public nyaya_exception
{
public:
	end_of_file_exception(int l, int p, string file);
};

class NaN_exception : public nyaya_exception
{
	public:
	char sym;
	NaN_exception(char symbol, int l, int p, string file);
};

class no_word_exception : public nyaya_exception
{
	public:	
		no_word_exception(int l, int p, string file);
};
