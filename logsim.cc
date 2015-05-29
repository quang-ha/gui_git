#include "gui.h"
#include <GL/glut.h>
#include "interpret.h"
#include "logsim.h"
using namespace std;
#define USE_GUI

IMPLEMENT_APP(MyApp)
  
bool MyApp::OnInit()
  // This function is automatically called when the application starts
{
  if (argc != 2)
  {
    wcout << "Usage:    " << argv[0] << " [filename] " << endl;
    exit(1);
  }
  // Construct the six classes required by the innards of the logic simulator
	names nmz("item");
	network* net = new network(&nmz);
	devices* devices_obj = new devices(&nmz, net);
	monitor* mon = new monitor(&nmz, net);
	vector<nyaya_exception> e; vector<nyaya_exception> w;
	interpret(devices_obj,net,mon,&e,&w,argv[1].ToStdString());
  print_exceptions(w);
  if (e.size() != 0)
  {
    print_exceptions(e);
    exit(1);
  }
#ifdef USE_GUI
    // glutInit cannot cope with Unicode command line arguments, so we pass
    // it some fake ASCII ones instead
    char **tmp1; int tmp2 = 0; glutInit(&tmp2, tmp1);
    // Construct the GUI
    MyFrame *frame = new MyFrame(NULL, "Logic simulator ELEA", wxDefaultPosition,  wxSize(1000, 750), &nmz, devices_obj, mon, net);
    frame->Show(true);
    return(true); // enter the GUI event loop
#else
return false;
#endif /* USE_GUI */
}
