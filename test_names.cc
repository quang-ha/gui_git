#include "test.h"
using namespace std;
bool test_names_errors(names &n, string* namestr, bool dir, int no)
{
	bool check = true;
	for (int i = dir ? 0 : 5; i != (dir ? 6 : -1); dir ? i++ : i--)
	{
		try
		{
			n.lookup(namestr[i],0,0,"test");
		}
		catch (non_alpha_exception e)
		{
			if (i == 3 && e.file == "test" && string(e.what()).compare("Invalid Name 1234abcdefghik: Contains illegal character '1', which is not allowed in Nyaya.") == 0)
				cout << "Test UN" << ((dir ? 2 : 6) + no) << " successful" << endl;
			else if (i == 4 && e.file == "test" && string(e.what()).compare("Invalid Name Bob!: Contains illegal character '!', which is not allowed in Nyaya.") == 0)
				cout << "Test UN" << ((dir ? 3 : 5) + no) << " successful" << endl;
			else {
				cout << "Test failed." << endl; check = false;
			}
		}
		catch (long_name_exception e)
		{
			if (i == 2 && e.file == "test" && e.file == "test" &&string(e.what()).compare(string("Invalid String d429090ygikygiiuyg1234456678erty: Strings longer than 20 characters are not allowed in Nyaya.").c_str()) == 0)
				cout << "Test UN" << ((dir ? 1 : 7) + no) << " successful" << endl;
			else {
				cout << "Test failed." << endl; check = false;
			}
		}
	}
	return check;
}
bool test_names_values(names &n, string* namestr)
{
	bool check = true;
	if (n[0] == namestr[0] && n[1] == namestr[1] && n[2] == namestr[5] && n.size() == 3)
		cout << "Test UN4 successful" << endl;
	else {
		cout << "Test UN4 failed" << endl; check = false;
	}

	if(!test_names_errors(n, namestr, false, 0) | !test_names_errors(n, namestr, true, 7)) check = false;

	if (n[0] == namestr[0] && n[1] == namestr[1] && n[2] == namestr[5] && n.size() == 3)
		cout << "Test UN11 successful" << endl;
	else {
		cout << "Test UN11 failed" << endl; check = false;
	}
	return check;

}
bool test_names()
{
	names n("bob");
	string namestr[] = { "asdasd", "kl123", "d429090ygikygiiuyg1234456678erty", "1234abcdefghik", "Bob!", "Alice" };

	return test_names_errors(n, namestr, true, 0) & test_names_values(n, namestr);
}
