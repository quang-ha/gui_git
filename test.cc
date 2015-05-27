#include "test.h"
using namespace std;
int main()
{
	test_names();
	test_scanner();
#ifdef _WIN32
	while(true){}
#endif
}
