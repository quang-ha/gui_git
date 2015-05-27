#include "test.h"
using namespace std;
void test_empty()
{
	scanner sc("empty.nya");
	try
	{
		command_type ct = sc.getcommand();
	}
	catch (end_of_file_exception e)
	{
		cout << "Test US1 successful" << endl;
	}
}
void test_bistable()
{
	scanner sc("test1.nya");
	names devices("devices");
	static string types[] = { "NAND", "SWITCH"};
	names type(vector<string>(types, types + sizeof types / sizeof types[0]), "type");
	try
	{
		if (sc.getcommand() == create)
		{
			cout << "Test US2 successful" << endl;
			int i;
			sc.getword(type, i);
			if (i == 0)
			{
				cout << "Test US3 successful" << endl;
				sc.addword(devices);
				if (sc.getpunc() == openpar && sc.getnumber() == 2)
				{
					cout << "Test US4 successful" << endl;
					if (sc.getpunc() == closepar && sc.getpunc() == comma)
					{
						cout << "Test US5 successful" << endl;
						sc.addword(devices);
						if (sc.getpunc() == openpar && sc.getnumber() == 2)
						{
							cout << "Test US6 successful" << endl;
							if (sc.getpunc() == closepar && sc.getpunc() == ampersand)
							{
								sc.getword(type, i);
								if (i == 1)
								{
									cout << "Test US7 successful" << endl;
									sc.addword(devices);
									if (sc.getpunc() == openpar && sc.getnumber() == 0 && sc.getpunc() == closepar && sc.getpunc() == comma)
									{
										cout << "Test US8 successful" << endl;
										if (sc.addword(devices).command_match && sc.getpunc() == openpar && sc.getnumber() == 0 && sc.getpunc() == closepar)
										{
											if (sc.getpunc() == semicolon && sc.getcommand() == connect)
											{
												cout << "Test US9 successful" << endl;
												sc.getword(devices, i);
												if (devices[i] == "SW1")
												{
													cout << "Test US10 successful" << endl;
													if (devices[0] == "G1" && devices[1] == "G2" && devices[2] == "SW1" && devices[3] == "DEFINE" && devices.size() == 4)
													{
														cout << "Test US11 successful" << endl;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
				
			}
		}

	}
	catch (invalid_name_exception e)
	{
		cout << e.what() << endl;
		cout << "At line " << e.line << " position " << e.pos << endl;
		cout << "Name: " << e.oname << endl;
	}
	catch (nyaya_exception e)
	{
		cout << e.what() << endl;
		cout << "At line " << e.line << " position " << e.pos << endl;
	}
}
void test_scanner()
{
	test_empty();
	test_bistable();
}
