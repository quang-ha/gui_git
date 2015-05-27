#include <algorithm>

using namespace std;

class scanner_exception: public nyaya_exception
{
};

class invalid_character_exception : public scanner_exception
{
	public:
	char sym;
	invalid_character_exception(char symbol, int l, int p)
	{
		sym = symbol;
		line = l; pos = p;
		msg = (string)("Scanner Error: Character " + sym) + " is not valid punctuation.";
	}
};

class cant_open_exception : public scanner_exception
{
public:
	cant_open_exception(int l, int p)
	{
		line = l; pos = p;
		msg = "Can't open file.";
	}
};
class end_of_file_exception : public scanner_exception
{
public:
	end_of_file_exception(int l, int p)
	{
		line = l; pos = p;
		msg = "Unexpected end of file.";
	}
};

class NaN_exception : public scanner_exception
{
	public:
	char sym;
	NaN_exception(char symbol, int l, int p)
	{
		line = l; pos = p;
		sym = symbol;
		msg = (string)("Scanner Error: Character " + sym) + " is not a number.";
	}
};

class no_word_exception : public scanner_exception
{
	public:	
	no_word_exception(int l, int p)
	{
		line = l; pos = p;
		msg = "Expected word not found.";
	}
};
