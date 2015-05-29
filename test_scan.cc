#include "test.h"
using namespace std;
bool test_empty()
{
	scanner sc("empty.nya",0,0);
	try
	{
		sc.getcommand();
	}
	catch (end_of_file_exception e)
	{
		cout << "Test US1 successful" << endl;
		return true;
	}
	return false;
}
bool test_include()
{
	scanner sc("test2.nya",0,0);
	if(sc.getcommand() == include && sc.getstring() == "invalid" && sc.getpunc() == dot && sc.getstring() == "nylb" && sc.getpunc() == semicolon)
	{
		cout << "Test US13 successful" << endl;
		return true;
	}
	else
	{
		cout << "Test failed" << endl;
		return false;
	}
}
bool test_bistable()
{
	bool check = true;
	scanner sc("test1.nya",0,0);
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
									if (sc.getpunc() == openpar && sc.getnumber() == 1 && sc.getpunc() == closepar && sc.getpunc() == comma)
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
														sc.nextline();
														if(sc.getcommand() == monitor_command)
														{
															cout << "Test US12 successful" << endl;
														}
														else { check = false;  }
													}
													else { check = false; }
												}
												else { check = false; }
											}
											else { check = false; }
										}
										else { check = false; }
									}
									else { check = false; }
								}
								else { check = false; }
							}
							else { check = false; }
						}
						else { check = false; }
					}
					else { check = false; }
				}
				else { check = false; }
				
			}
			else { check = false; }
		}
		else { check = false; }
		return check;
	}
	catch (invalid_name_exception e)
	{
		cout << e.what() << endl;
		cout << "At line " << e.line << " position " << e.pos << endl;
		cout << "Name: " << e.oname << endl;
		return false;
	}
	catch (nyaya_exception e)
	{
		cout << e.what() << endl;
		cout << "At line " << e.line << " position " << e.pos << endl;
		return false;
	}
}
bool test_scanner()
{
	return test_empty() & test_bistable() &	test_include();
}
