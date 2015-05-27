#include "test.h"
using namespace std;
void test_names_errors(names &n, string* namestr, bool dir, int no)
{

	for (int i = dir ? 0 : 5; i != (dir ? 6 : -1); dir ? i++ : i--)
	{
		try
		{
			n.lookup(namestr[i],0,0);
		}
		catch (const non_alpha_exception* e)
		{
			if (i == 3 && string(e->what()).compare("Invalid Name 1234abcdefghik: Contains illegal character '1'.") == 0)
				cout << "Test UN" << ((dir ? 2 : 6) + no) << " successful" << endl;
			else if (i == 4 && string(e->what()).compare("Invalid Name Bob!: Contains illegal character '!'.") == 0)
				cout << "Test UN" << ((dir ? 3 : 5) + no) << " successful" << endl;
			else cout << "Test failed." << endl;
		}
		catch (const long_name_exception* e)
		{
			if (i == 2 && string(e->what()).compare(string("Invalid Name d429090ygikygiiuyg1234456678erty: More than 20 characters.").c_str()) == 0)
				cout << "Test UN" << ((dir ? 1 : 7) + no) << " successful" << endl;
			else cout << "Test failed." << endl;
		}
	}
}
void test_names_values(names &n, string* namestr)
{
	if (n[0] == namestr[0] && n[1] == namestr[1] && n[2] == namestr[5] && n.size() == 3)
		cout << "Test UN4 successful" << endl;
	else cout << "Test UN4 failed" << endl;

	test_names_errors(n, namestr, false, 0); test_names_errors(n, namestr, true, 7);

	if (n[0] == namestr[0] && n[1] == namestr[1] && n[2] == namestr[5] && n.size() == 3)
		cout << "Test UN11 successful" << endl;
	else cout << "Test UN11 failed" << endl;

}
void test_names()
{
	names n("bob");
	string namestr[] = { "asdasd", "kl123", "d429090ygikygiiuyg1234456678erty", "1234abcdefghik", "Bob!", "Alice" };

	test_names_errors(n, namestr, true, 0);
	test_names_values(n, namestr);
}
