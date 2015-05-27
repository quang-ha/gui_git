#include "guitest.h"
#include "gui.h"
#include <GL/glut.h>

IMPLEMENT_APP(MyApp)
  
bool MyApp::OnInit()
  // This function is automatically called when the application starts
{
  // ** Attempt at building the fake citcuite for the GUI ** 
  typedef string namestring;
  typedef int name;  

  // Construct the four classes required by the innards of the logic simulator
  nmz = new names("DEVICE1");
  // Construct the network
  // Add a new device
    // devicerec *and1 = new devicerec();
    // and1->id = nmz->lookup("ANDGATE");
    // and1->ilist = new inputrec[2]();
    // and1->ilist[0].id = nmz->lookup("I1");
    // and1->ilist[1].id = nmz->lookup("I2");
    // // Definte the input of the device
    // and1->olist = new outputrec();
    // and1->olist = nmz->lookup("O1");
    // and1->kind = andgate;

  netz = new network(nmz);
  // netz->adddevice(andgate, and1->id, *and1);
    // Definte the output of the device

  dmz = new devices(nmz, netz);
  bool OK;
  dmz->makedevice(andgate, nmz->add("ANDGATE"), 2, OK);
  dmz->makedevice(aswitch, nmz->add("S1"), 1, OK);
  dmz->makedevice(aswitch, nmz->add("S2"), 0, OK);
  netz->makeconnection(nmz->lookup("ANDGATE"), nmz->lookup("I1"), nmz->lookup("S1"), blankname, OK);
  cout << OK << "S1" << endl;
  netz->makeconnection(nmz->lookup("ANDGATE"), nmz->lookup("I2"), nmz->lookup("S2"), blankname, OK);
  cout << OK << "S2" << endl;

  mmz = new monitor(nmz, netz);
  //mmz->makemonitor(nmz->lookup("ANDGATE"), blankname, OK);

  dmz->executedevices(OK);
  cout << "Execute devices: " << OK << endl;
  netz->checknetwork(OK);
  cout << "Check network: " << OK << endl;
  cout << "Check monitor: " << mmz->getmonsignal(0) << endl;
  // glutInit cannot cope with Unicode command line arguments, so we pass
  // it some fake ASCII ones instead
  char **tmp1; int tmp2 = 0; glutInit(&tmp2, tmp1);
  // Construct the GUI
  //MyFrame *frame = new MyFrame(NULL, "Logic simulator ELEA", wxDefaultPosition,  wxSize(800, 600), nmz, dmz, mmz);
  MyFrame *frame = new MyFrame(NULL, "Logic Simulator ELEA", wxDefaultPosition, wxSize(800, 600), nmz, dmz, mmz, netz);
  frame->Show(true);
  frame->canvas->Update();
  return(true); // enter the GUI event loop
}
