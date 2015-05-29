#include "interpret.h"
using namespace std;

string inpin(pin* l, devs* dvs)
{
	return (*(dvs->inputs.at(l->device)))[l->num];
}
string instr(pin* l, devs* dvs, names* dvz)
{
	return (*dvz)[l->device_global] + "." + inpin(l, dvs);
}
string outpin(pin* l, devs* dvs)
{
	return (*(dvs->outputs.at(l->device)))[l->num];
}
string outstr(pin* l, devs* dvs, names* dvz)
{
	return (*dvz)[l->device_global] + (l->num == 0 ? "" : ("." + outpin(l, dvs)));
}
void interpret_out(devices* devices_obj, devs* &dvs, typeslist* types, network* net, pin* &p)
{
	name type_id = dvs->kinds.at(p->device);
	if (getkind(type_id) == custom)
	{
		p = types->outpins.at(type_id)->at(p->num);
		dvs = types->devices.at(type_id);
		interpret_out(devices_obj, dvs, types, net, p);
	}
}
void interpret_in(devices* devices_obj, devs* &dvs, typeslist* types, network* net, pin* &p)
{
	name type_id = dvs->kinds.at(p->device);
	if (getkind(type_id) == custom)
	{
		p = types->inpins.at(type_id)->at(p->num);
		dvs = types->devices.at(type_id);
		interpret_in(devices_obj, dvs, types, net, p);
	}
}
void interpret_link(devices* devices_obj, devs* dvs, typeslist* types, network* net, pin* from, pin* to, vector<nyaya_exception>* exceptions, vector<nyaya_exception>* warnings, string file)
{
	devs* dvfin = dvs; devs* dvfout = dvs;
	interpret_out(devices_obj, dvfin, types, net, from); interpret_in(devices_obj, dvfout, types, net, to);
	if (!net->makeconnection(from->device_global, from->num, to->device_global, inpin(to,dvfout))) exceptions->push_back(connect_fail_exception(outstr(from, dvs, devices_obj->dvs), instr(to, dvs, devices_obj->dvs), file));
}
void interpret_links(devices* devices_obj, devs* dvs, typeslist* types,  network* net, vector<lnk_class*> links, vector<nyaya_exception>* exceptions, vector<nyaya_exception>* warnings, string file)
{
	for (unsigned int i = 0; i < links.size(); i++)
	{
		interpret_link(devices_obj, dvs, types, net, links.at(i)->from, links.at(i)->to, exceptions, warnings, file);
	}
}
void interpret_device(devices* devices_obj, network* net, vector<pin*>* monitors, namestring n, name type_id, vector<name>* params, typeslist* types, vector<nyaya_exception>* exceptions, vector<nyaya_exception>* warnings, string file)
{
	if (getkind(type_id) == custom)
	{
		devs* dvz = types->devices.at(type_id);
		for (int j = 0; j < dvz->size(); j++)
		{
			vector<name>* paramvalues = new vector<name>();
			for (unsigned int k = 0; k < dvz->params.at(j)->size(); k++)
			{
				paramvalues->push_back(dvz->params.at(j)->at(k) == -1 ? params->at(dvz->refparams.at(j)->at(k)) : dvz->params.at(j)->at(k));
			}
			namestring nn = n + "/" + (*dvz)[j];
			for (unsigned int i = 0; i < types->outpins.at(type_id)->size(); i++)
			{
				pin* a = types->outpins.at(type_id)->at(i);
				if (a->device == j)
				{
					a->device_global = devices_obj->dvs->lookup(nn,true);
				}
			}
			for (unsigned int i = 0; i < types->inpins.at(type_id)->size(); i++)
			{
				pin* a = types->inpins.at(type_id)->at(i);
				if (a->device == j)
				{
					a->device_global = devices_obj->dvs->lookup(nn,true);
				}
			}
			for (unsigned int i = 0; i < monitors->size(); i++)
			{
				pin* a = monitors->at(i);
				if (a->device == j)
				{
					a->device_global = devices_obj->dvs->lookup(nn,true);
				}
			}
			for (unsigned int i = 0; i < types->links.at(type_id)->size(); i++)
			{
				lnk_class* a = types->links.at(type_id)->at(i);
				if (a->from->device == j) a->from->device_global = devices_obj->dvs->lookup(nn, true);
				if (a->to->device == j) a->to->device_global = devices_obj->dvs->lookup(nn, true);
			}
			interpret_device(devices_obj, net, monitors, nn, dvz->kinds.at(j), paramvalues, types, exceptions, warnings, file);
		}
		interpret_links(devices_obj, dvz, types, net, *(types->links.at(type_id)), exceptions, warnings, file);
	}
	else if (!devices_obj->makedevice(getkind(type_id), devices_obj->dvs->retrieve(n), params->size() ? params->at(0) : 0)) exceptions->push_back(make_fail_exception(n, file));
}
void interpret(devices* devices_obj, network* net, monitor* mon, vector<nyaya_exception>* exceptions, vector<nyaya_exception>* warnings, string file)
{
	try {
		devs dvs;
		typeslist types;
		vector<pin*> monitors;
		vector<lnk_class*> links;
		parser_args args(&dvs, &types, &monitors, &links, exceptions, warnings, 0, 0, file);
		parser pa(args);
		pa.readin();
		int s = dvs.size();
		devices_obj->dvs = dvs.devnames;
		for (int i = 0; i < s; i++)
		{
			interpret_device(devices_obj, net, &monitors, dvs[i], dvs.kinds.at(i), dvs.params.at(i), &types, exceptions, warnings, file);
		}
		interpret_links(devices_obj, &dvs, &types, net, links, exceptions, warnings, file);
		try
		{
			checknetwork(file, net, dvs);
		}
		catch (bad_network_exception e)
		{
			exceptions->push_back(e);
		}
		for (unsigned int i = 0; i < monitors.size(); i++)
		{
			pin* p = monitors.at(i); devs* d = &dvs;
			interpret_out(devices_obj, d, &types, net, p);
			if (!mon->makemonitor(p->device_global, p->num)) exceptions->push_back(monitor_exception(file));
		}
	}
	catch (nyaya_exception e)
	{
		exceptions->push_back(e);
	}
/*	catch (exception e)
	{
		exceptions->push_back(other_exception(file, -1, -1));
	}*/
}

void checknetwork(string file, network* net, devs &dvs)
{
	devlink d;
	inplink i;
	for (d = net->device_list; d != NULL; d = d->next)
		for (i = d->ilist; i != NULL; i = i->next)
		if (i->connect == NULL) {
			throw bad_network_exception(dvs[d->id] + ((i->id == blankname) ? "" : ("." + (*(net->nmz))[i->id])), file);
		}
}

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