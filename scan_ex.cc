#include "scan_ex.h"
using namespace std;

invalid_character_exception::invalid_character_exception(char symbol, int l, int p, string file) : nyaya_exception (l, p, file)
{
	sym = symbol;
	string s = "Scanner Error: Character  ";
	s[s.length()-1]=symbol;
	msg = s + " is not valid punctuation.";
}

NaN_exception::NaN_exception(char symbol, int l, int p, string file) : nyaya_exception (l, p, file)
{
	sym = symbol;
	string s = "Scanner Error: Character  ";
	s[s.length()-1]=symbol;
	msg = s + " is not a number.";
}

end_of_file_exception::end_of_file_exception(int l, int p, string file) : nyaya_exception(l, p, file)
{
	msg = "Unexpected end of file.";
}

no_word_exception::no_word_exception(int l, int p, string file) : nyaya_exception(l, p, file)
{
	msg = "Expected word not found.";
}

cant_open_exception::cant_open_exception(int l, int p, string file) : nyaya_exception(l, p, file)
{
	msg = "File Error: Couldn't find a valid Nyaya Library file called " + file + ".";
}