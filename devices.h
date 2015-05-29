#ifndef devices_h
#define devices_h

#include "names.h"
#include "network.h"

class devices{
  network* netz;   // the version of the network module we use.  
  typedef name devicetable[baddevice + 1]; 
  devicetable dtab;
  bool        steadystate;
  name        clkpin, datapin, setpin;
  name        clrpin, qpin, qbarpin;     /* Input and Output Pin names */
  bool        debugging;

  void showdevice (devlink d);
  void makeswitch (name id, int setting, bool& ok);
  void makeclock (name id, int frequency);
  void makegate (devicekind dkind, name did, int ninputs, bool& ok);
  void makedtype (name id);
  void signalupdate (asignal target, asignal& sig);
  asignal inv (asignal s);
  void execswitch (devlink d);
  void execgate (devlink d, asignal x, asignal y);
  void execxorgate(devlink d);
  void execdtype (devlink d);
  void execclock(devlink d);
  void updateclocks (void);
  void outsig (asignal s);

public:
  names* dvs;
  names* nmz;      // the version of the names module we use.
  bool makedevice (devicekind dkind, name did, int variant);
    /* Adds a device to the network of the specified kind and name.  The   */
    /* variant is used with such things as gates where it specifies the    */
    /* number of inputs. 'ok' returns true if operation succeeds.          */
 
  bool setswitch(namestring sid, asignal level);
    /* Sets the state of the named switch. 'ok' returns false if switch    */
    /* not found.                                                          */
 
  bool executedevices ();
    /* Executes all devices in the network to simulate one complete clock  */
    /* cycle. 'ok' is returned false if network fails to stabilise (i.e.   */
    /* it is oscillating). */                                             
 
  devicekind devkind (name id);
    /* Returns the kind of device corresponding to the given name.         */
    /* 'baddevice' is returned if the name is not a legal device.          */
 
  void writedevice (devicekind k);
    /* Prints out the given device kind.                                   */
 
  void debug (bool on);
    /* Used to set debugging switch.                                       */
 
  devices (names* names_mod, network* net_mod);
    /* Called to initialise module.                                        */
};

#endif /* devices_h */
