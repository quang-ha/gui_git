#include "test.h"
using namespace std;
int main()
{
	if (test_names() & test_scanner() & test_parser() & test_interpreter()) cout << "All tests successful" << endl;
#ifdef _WIN32
	while(true){}
#endif
}
