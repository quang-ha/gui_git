#include "test.h"
using namespace std;
bool testa()
{
	bool check = true;
	names nmz("item");
	network* net = new network(&nmz);
	devices* devices_obj = new devices(&nmz, net);
	monitor* mon = new monitor(&nmz, net);
	vector<nyaya_exception> e; vector<nyaya_exception> w;
	interpret(devices_obj, net, mon, &e, &w, "test1.nya");
	if (devices_obj->executedevices() && e.size() == 0 && w.size() == 1 && w[0].file == "test1.nya" && w[0].line == 3 && w[0].pos == 16 && w[0].msg == "Warning: It is not recommended to use the name of a command as the name of a device in Nyaya.") cout << "Test UI1 successful" << endl; else { cout << "Test UI1 failed" << endl; check = false; }
	if (mon->moncount() == 2 && mon->getmonsignal(0) == low && mon->getmonsignal(1) == high) cout << "Test UI2 successful" << endl; else {
		cout << "Test UI2 failed" << endl; check = false;
	}
	if (devices_obj->setswitch("SW1", low) && devices_obj->setswitch("DEFINE", high) && devices_obj->executedevices() && mon->moncount() == 2 && mon->getmonsignal(0) == high && mon->getmonsignal(1) == low) cout << "Test UI3 successful" << endl; else {
		cout << "Test UI3 failed" << endl; check = false;
	}
	return check;
}
bool testb()
{
	bool check = true;
	names nmz("item");
	network* net = new network(&nmz);
	devices* devices_obj = new devices(&nmz, net);
	monitor* mon = new monitor(&nmz, net);
	vector<nyaya_exception> e; vector<nyaya_exception> w;
	interpret(devices_obj, net, mon, &e, &w, "test6.nya");
	if (devices_obj->executedevices() && e.size() == 0 && w.size() == 0) cout << "Test UI4 successful" << endl; else { cout << "Test UI4 failed" << endl; check = false; }
	if (mon->moncount() == 1 && mon->getmonsignal(0) == high) cout << "Test UI5 successful" << endl; else {
		cout << "Test UI5 failed" << endl; check = false;
	}
	if (devices_obj->setswitch("A", high) && devices_obj->executedevices() && mon->moncount() == 1 && mon->getmonsignal(0) == low) cout << "Test UI6 successful" << endl; else {
		cout << "Test UI6 failed" << endl; check = false;
	}
	return check;
}
bool testc()
{
	bool check = true;
	names nmz("item");
	network* net = new network(&nmz);
	devices* devices_obj = new devices(&nmz, net);
	monitor* mon = new monitor(&nmz, net);
	vector<nyaya_exception> e; vector<nyaya_exception> w;
	interpret(devices_obj, net, mon, &e, &w, "test7.nya");
	print_exceptions(e); print_exceptions(w);
	if (e.size() == 0 && w.size() == 0) cout << "Test UI7 successful" << endl; else { cout << "Test UI7 failed" << endl; check = false; }
	return check;
}
bool testd()
{
	bool check = true;
	names nmz("item");
	network* net = new network(&nmz);
	devices* devices_obj = new devices(&nmz, net);
	monitor* mon = new monitor(&nmz, net);
	vector<nyaya_exception> e; vector<nyaya_exception> w;
	interpret(devices_obj, net, mon, &e, &w, "test8.nya");
	print_exceptions(e); print_exceptions(w);
	if (e.size() == 0 && w.size() == 0) cout << "Test UI8 successful" << endl; else { cout << "Test UI8 failed" << endl; check = false; }
	return check;
}
bool test_interpreter()
{
	return testa() & testb() & testc() & testd();
}