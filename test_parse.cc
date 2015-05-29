#include "test.h"
using namespace std;
void print_exception(nyaya_exception e)
{
	cout << e.what() << endl << "at " << e.file << " line " << e.line << " position " << e.pos << endl << endl;	
}
void print_exceptions(vector<nyaya_exception> e)
{
	for (unsigned int i = 0; i < e.size(); i++)
	{
		print_exception(e.at(i));
	}	
}
bool test1()
{
	bool check = true;
	vector<nyaya_exception> e;
	vector<nyaya_exception> w;
	devs dvs;
	typeslist types;
	vector<pin*> monitors;
	vector<link*> links;
	types.add("ALICE",4);
	types.add("BOB",4);
	parser_args args(&dvs, &types, &monitors, &links, &e, &w, 0, 0, (string)"test2.nya");
	parser pa(args);
	pa.readin();
	if (e.size() == 10 && e[0].file == "invalid.nylb" && e[0].line == 1 && e[0].pos == 0 && e[0].msg == "File Error: Couldn't find a valid Nyaya Library file called invalid.nylb.") cout << "Test UP1 successful" << endl; else { cout << "Test UP1 failed" << endl; check = false; }
	if (e[1].file == "test2.nya" && e[1].line == 2 && e[1].pos == 18 && e[1].msg == "Parser Error: Invalid filename test1.nya. Nyaya library files should be named xxx.nylb.") cout << "Test UP2 successful" << endl; else { cout << "Test UP2 failed" << endl; check = false; }
	if (e[2].file == "test2.nya" && e[2].line == 3 && e[2].pos == 14 && e[2].msg == "Scanner Error: Character ; is not valid punctuation.") cout << "Test UP3 successful" << endl; else { cout << "Test UP3 failed" << endl; check = false; }
	if (e[3].file == "test2.nya" && e[3].line == 4 && e[3].pos == 13 && e[3].msg == "Expected word not found." && e[4].file == "test2.nya" && e[4].line == 5 && e[4].pos == 15 && e[4].msg == "Invalid Name 1: Contains illegal character '1', which is not allowed in Nyaya.") cout << "Test UP4 successful" << endl; else { cout << "Test UP4 failed" << endl; check = false; }
	if (e[5].file == "test2.nya" && e[5].line == 6 && e[5].pos == 31 && e[5].msg == "Invalid parameter: Reference e does not define a valid parameter in this context." && e[6].file == "test2.nya" && e[6].line == 7 && e[6].pos == 34 && e[6].msg == "Invalid Name BOB: A device with that name already exists.") cout << "Test UP5 successful" << endl; else { cout << "Test UP5 failed" << endl; check = false; }
	if (e[7].file == "test2.nya" && e[7].line == 8 && e[7].pos == 15 && e[7].msg == "Invalid Name l: A device with that name already exists." && e[8].file == "test2.nya" && e[8].line == 9 && e[8].pos == 15 && e[8].msg == "Invalid Name l: A device with that name already exists.") cout << "Test UP6 successful" << endl; else { cout << "Test UP6 failed" << endl; check = false; }
	if (e[9].file == "test2.nya" && e[9].line == 10 && e[9].pos == 22 && e[9].msg == "Wrong number of parameters: Devices of this type require 4 parameters.") cout << "Test UP7 successful" << endl; else { cout << "Test UP7 failed" << endl; check = false; }
	if (w.size() == 2 && w[0].file == "test2.nya" && w[0].line == 6 && w[0].pos == 29 && w[0].msg == "Warning: It is not recommended to use the name of a type as the name of a device in Nyaya." && w[1].file == "test2.nya" && w[1].line == 7 && w[1].pos == 20 && w[1].msg == "Warning: It is not recommended to use the name of a command as the name of a device in Nyaya.")
		cout << "Test UP8 successful" << endl; else { cout << "Test UP8 failed" << endl; check = false; }
	if (dvs.size() == 4 && dvs[0] == "l" && dvs[1] == "BOB" && dvs[2] == "DEFINE" && dvs[3] == "w") cout << "Test UP9 successful" << endl; else { cout << "Test UP9 failed" << endl; check = false; }
	if(dvs.params.at(0)->at(0) == 1 && dvs.params.at(0)->at(1) == 2 && dvs.params.at(0)->at(2) == 3 && dvs.params.at(0)->at(3) == 4 && dvs.params.at(0)->size() == 4 && dvs.params.at(1)->size() == 0) cout << "Test UP10 successful" << endl; else { cout << "Test UP10 failed" << endl; check = false; }
	if (dvs.params.at(2)->at(0) == 1 && dvs.params.at(2)->at(1) == 2 && dvs.params.at(2)->at(2) == 3 && dvs.params.at(2)->at(3) == 4 && dvs.params.at(2)->size() == 4) cout << "Test UP11 successful" << endl; else { cout << "Test UP11 failed" << endl; check = false; }
	return check;
}
bool test2()
{
	bool check = true;
	vector<nyaya_exception> e;
	vector<nyaya_exception> w;
	devs dvs;
	typeslist types;
	vector<pin*> monitors;
	vector<link*> links;
	parser_args args(&dvs, &types, &monitors, &links, &e, &w, 0, 0, (string)"test1.nya");
	
	parser pa(args);
	pa.readin();
	if (e.size() == 0 && w.size() == 1 && w[0].file == "test1.nya" && w[0].line == 3 && w[0].pos == 16 && w[0].msg == "Warning: It is not recommended to use the name of a command as the name of a device in Nyaya.") cout << "Test UP12 successful" << endl; else { cout << "Test UP12 failed" << endl; check = false; }
	if (dvs.size() == 4 && dvs[0] == "G1" && dvs[1] == "G2" && dvs[2] == "SW1" && dvs[3] == "DEFINE") cout << "Test UP13 successful" << endl; else { cout << "Test UP13 failed" << endl; check = false; }
	if (monitors.size() == 2 && dvs[monitors.at(0)->device] == "G1" && monitors.at(0)->num == 0 && dvs[monitors.at(1)->device] == "G2" && monitors.at(1)->num == 0) cout << "Test UP14 successful" << endl; else { cout << "Test UP14 failed" << endl; check = false; }
	if(links.size() == 4 && dvs[links.at(0)->from->device] == "SW1" && links.at(0)->from->num == 0 && dvs[links.at(0)->to->device] == "G1" && (*(dvs.inputs.at(links.at(0)->to->device)))[links.at(0)->to->num] == "I1") cout << "Test UP15 successful" << endl; else { cout << "Test UP15 failed" << endl; check = false; }
	if (dvs[links.at(1)->from->device] == "G1" && links.at(1)->from->num == 0 && dvs[links.at(1)->to->device] == "G2" && (*(dvs.inputs.at(links.at(1)->to->device)))[links.at(1)->to->num] == "I1") cout << "Test UP16 successful" << endl; else { cout << "Test UP16 failed" << endl; check = false; }
	if (dvs[links.at(2)->from->device] == "G2" && links.at(2)->from->num == 0 && dvs[links.at(2)->to->device] == "G1" && (*(dvs.inputs.at(links.at(2)->to->device)))[links.at(2)->to->num] == "I2") cout << "Test UP17 successful" << endl; else { cout << "Test UP17 failed" << endl; check = false; }
	if (dvs[links.at(3)->from->device] == "DEFINE" && links.at(3)->from->num == 0 && dvs[links.at(3)->to->device] == "G2" && (*(dvs.inputs.at(links.at(3)->to->device)))[links.at(3)->to->num] == "I2") cout << "Test UP18 successful" << endl; else { cout << "Test UP18 failed" << endl; check = false; }
	return check;
}
bool test3()
{
	bool check = true;
	vector<nyaya_exception> e;
	vector<nyaya_exception> w;
	devs dvs;
	typeslist types;
	vector<pin*> monitors;
	vector<link*> links;
	network n(types.typenames);
	devices d(types.typenames, &n);
	monitor m(types.typenames, &n);
	parser_args args(&dvs, &types, &monitors, &links, &e, &w, 0, 0, (string)"test5.nya");

	parser pa(args);
	pa.readin();
	if (e.size() == 1 && e[0].file == "test5.nya" && e[0].line == 7 && e[0].pos == 20 && e[0].msg == "Invalid Name I3: No input with that name exists.") cout << "Test UP19 successful" << endl; else { cout << "Test UP19 failed" << endl; check = false; }
	if(w.size() == 1 && w[0].file == "test5.nya" && w[0].line == 3 && w[0].pos == 16 && w[0].msg == "Warning: It is not recommended to use the name of a command as the name of a device in Nyaya.") cout << "Test UP20 successful" << endl; else { cout << "Test UP20 failed" << endl; check = false; }
	if (monitors.size() == 2 && dvs[monitors.at(0)->device] == "G1" && monitors.at(0)->num == 0 && dvs[monitors.at(1)->device] == "G2" && monitors.at(1)->num == 0) cout << "Test UP21 successful" << endl; else { cout << "Test UP21 failed" << endl; check = false; }
	if (links.size() == 3 && dvs[links.at(0)->from->device] == "SW1" && links.at(0)->from->num == 0 && dvs[links.at(0)->to->device] == "G1" && (*(dvs.inputs.at(links.at(0)->to->device)))[links.at(0)->to->num] == "I1") cout << "Test UP22 successful" << endl; else { cout << "Test UP22 failed" << endl; check = false; }
	if (dvs[links.at(1)->from->device] == "G1" && links.at(1)->from->num == 0 && dvs[links.at(1)->to->device] == "G2" && (*(dvs.inputs.at(links.at(1)->to->device)))[links.at(1)->to->num] == "I1") cout << "Test UP23 successful" << endl; else { cout << "Test UP23 failed" << endl; check = false; }
	if (dvs[links.at(2)->from->device] == "G2" && links.at(2)->from->num == 0 && dvs[links.at(2)->to->device] == "G1" && (*(dvs.inputs.at(links.at(2)->to->device)))[links.at(2)->to->num] == "I2") cout << "Test UP24 successful" << endl; else { cout << "Test UP24 failed" << endl; check = false; }
	return check;
}
bool test4()
{
	bool check = true;
	vector<nyaya_exception> e;
	vector<nyaya_exception> w;
	devs dvs;
	typeslist types;
	vector<pin*> monitors;
	vector<link*> links;
	parser_args args(&dvs, &types, &monitors, &links, &e, &w, 0, 0, (string)"test7.nya");

	parser pa(args);
	pa.readin();
	print_exceptions(e);
	print_exceptions(w);
	if (e.size() == 0 && w.size() == 0) cout << "Test UP25 successful" << endl; else { cout << "Test UP25 failed" << endl; check = false; }
	return check;
}
bool test5()
{
	bool check = true;
	vector<nyaya_exception> e;
	vector<nyaya_exception> w;
	devs dvs;
	typeslist types;
	vector<pin*> monitors;
	vector<link*> links;
	parser_args args(&dvs, &types, &monitors, &links, &e, &w, 0, 0, (string)"test8.nya");

	parser pa(args);
	pa.readin();
	if (e.size() == 0 && w.size() == 0) cout << "Test UP26 successful" << endl; else { cout << "Test UP26 failed" << endl; check = false; }
	return check;
}
bool test_parser()
{
	return test1() & test2() & test3() & test4() & test5();
}
