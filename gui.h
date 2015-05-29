// define this to use XPMs everywhere (by default, BMPs are used under Win)
// BMPs use less space, but aren't compiled into the executable on other platforms
#ifdef __WINDOWS__
    #define USE_XPM_BITMAPS 0
#else
    #define USE_XPM_BITMAPS 1
#endif

// If this is 1, the sample will test an extra toolbar identical to the
// main one, but not managed by the frame. This can test subtle differences
// in the way toolbars are handled, especially on Mac where there is one
// native, 'installed' toolbar.
#define USE_UNMANAGED_TOOLBAR 0
  
// Define this as 0 for the platforms not supporting controls in toolbars
#define USE_CONTROLS_IN_TOOLBAR 1

#define USE_STD_IOSTREAM 1
// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "sample.xpm"
#endif

#if USE_XPM_BITMAPS
    #include "bitmaps/new.xpm"
    #include "bitmaps/open.xpm"
    #include "bitmaps/save.xpm"
    #include "bitmaps/copy.xpm"
    #include "bitmaps/cut.xpm"
    #include "bitmaps/preview.xpm"  // paste XPM
    #include "bitmaps/print.xpm"
    #include "bitmaps/help.xpm"
#endif // USE_XPM_BITMAPS

enum Positions
{
    TOOLBAR_LEFT,
    TOOLBAR_TOP,
    TOOLBAR_RIGHT,
    TOOLBAR_BOTTOM
};

#ifndef gui_h
#define gui_h

#include <wx/wx.h>
#include <wx/toolbar.h>
#include <wx/glcanvas.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
#include <wx/choice.h>
#include "names.h"
#include "devices.h"
#include "monitor.h"
#include <vector>
#include <wx/grid.h>
#include <wx/filedlg.h>

enum { 
  MY_SPINCTRL_ID = wxID_HIGHEST + 1,
  MY_TEXTCTRL_ID,
  TOOLBAR_TEXTCTRL_ID,
  MY_BUTTON_ID,
  MY_SCROLL_ID,
  MY_SLIDER_H_ID,
  MY_SLIDER_V_ID,
  MY_ZOOM_ID,
  MY_SWITCH_CONTROL,
  MY_HEIGHTCTRL_ID,
  CHANGE_SWITCH_STATE,
  ID_LISTBOX,
  ID_ADD,
  EDITOR_OPEN,
  EDITOR_SAVE,
  EDITOR_EXIT,
  EDITOR_RUN,
  OPEN_DIALOG,
  SAVE_DIALOG
}; // widget identifiers

class MyGLCanvas;
class MyEditor;
class MyFrame;

// ** Panel for the ListBox on the right ** //
class MyPanel: public wxPanel
{
public: 
  MyPanel(wxPanel *parent, names *names_mod = NULL, monitor *monitor_mod = NULL, devices *devices_mod = NULL, network *network_mod = NULL);

  void OnAdd(wxCommandEvent& event);
  void OnDelete(wxCommandEvent& event);
  void TranslateMonitorName(wxString string, name &device, name &out);

  names *nmz;                             // pointer to names class
  devices *dmz;                           // pointer to devices class
  monitor *mmz;                           // pointer to monitor class
  network *netz;                          // pointer to network class

  MyFrame *real_frame; // pointer to parent class, which is MyFrame
  wxListBox *m_lb;
  wxChoice *Cb;

  wxButton *m_addb;
  wxButton *m_deleteb;
};

class MyFrame: public wxFrame
{
 public:
  MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size, 
	  names *names_mod = NULL, devices *devices_mod = NULL, monitor *monitor_mod = NULL, network *network_mod = NULL,  
	  long style = wxDEFAULT_FRAME_STYLE); // constructor

  // ** For ListBox on the right ** //
  wxListBox *listbox;
  MyPanel *btnPanel;

  // ** For RadioControl on the right ** //
  wxChoice *switchcontrol;
  wxCheckBox *changestate;
  int cyclescompleted;                  // how many simulation cycles have been completed 
  MyGLCanvas *canvas;                     // OpenGL drawing area widget to draw traces
  wxSpinCtrl *cyc;                       // control widget to select the number of cycles
  wxSpinCtrl *view_spin;                  // control widget to select the number of cycles to DISPLAY
  wxSpinCtrl *height_spin;
  wxSlider *slider_h;                     // horizontal slider
  wxSlider *slider_v;                     // vertical slider

 private:
  MyEditor *TextBox;                      // TextBox for editting the file
  names *nmz;                             // pointer to names class
  devices *dmz;                           // pointer to devices class
  monitor *mmz;                           // pointer to monitor class
  network *netz;                           // pointer to network class
  void runnetwork(int ncycles);           // function to run the logic network

  // void OnDblClick(wxCommandEvent& event); // event handler when Double Click on item

  // ** FILE MENU ** // 
  //void OnNew(wxCommandEvent& event);      // event handler for NEW item
  void OnOpen(wxCommandEvent& event);     // event handler for OPEN item
  void OnSave(wxCommandEvent& event);     // event handler for SAVE item
  void OnExit(wxCommandEvent& event);     // event handler for EXIT item

  // ** VIEWING OPTION ** //
  void OnScroll(wxSpinEvent& event); // Allowing scrolling
  void OnHeight(wxSpinEvent& event);
  void OnZoom(wxCommandEvent& event); // Allowing zooming
  void OnScrollH(wxScrollEvent& event); // Scrolling horizontally
  void OnScrollV(wxScrollEvent& event); // Scrolling vertically

  // * SIMULATE MENU ** //
  void OnRun(wxCommandEvent& event);
  void OnResetMonitor(wxCommandEvent& event);

  // ** HELP MENU ** //
  void OnAbout(wxCommandEvent& event);    // event handler for about menu item
  void OnButton(wxCommandEvent& event);   // event handler for push button

  // ** CREATE TOOLBAR ** //
  //void OnToolNew(wxCommandEvent& event); // event handler for TOOLBAR NEW item

  void OnSpin(wxSpinEvent& event);        // event handler for spin control
  void OnText(wxCommandEvent& event);     // event handler for text entry field

  // Create Switch Control
  void OnSelectSwitch(wxCommandEvent& event);
  void OnChangeState(wxCommandEvent& event); 

  DECLARE_EVENT_TABLE()
};


class MyGLCanvas: public wxGLCanvas
{
 public:
  MyGLCanvas(MyFrame *parent, wxWindowID id = wxID_ANY, monitor* monitor_mod = NULL, names* names_mod = NULL, devices* devices_mod = NULL,
	     const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0,
	     const wxString& name = "MyGLCanvas", const wxPalette &palette=wxNullPalette); // constructor
  void Render(wxString example_text, bool redraw); // function to draw canvas contents
  int width;                         // width of the cycles
  int height;                        // height of traces
  int cycles;                        // how many cycles of the simulation
  //int cyclesdraw;                    // how many cycles that have been completed
  float offset_v;                    // offset in vertical 
  float offset_h;                    // offset in horizontal
 private:
  MyFrame *real_frame;               // pointer to parent class, which is MyFrame
  wxGLContext *context;              // OpenGL rendering context
  bool init;                         // has the OpenGL context been initialised?  
  monitor *mmz;                      // pointer to monitor class, used to extract signal traces
  names *nmz;                        // pointer to names class, used to extract signal names
  devices *dmz;                        // pointer to devices class, used to extract signal names
  void InitGL();                     // function to initialise OpenGL context
  void OnSize(wxSizeEvent& event);   // event handler for when canvas is resized
  void OnPaint(wxPaintEvent& event); // event handler for when canvas is exposed
  void OnMouse(wxMouseEvent& event); // event handler for mouse events inside canvas

  void DrawDashLine(vector<float> coords, bool orient);   // function to draw vertical dash line
  void DrawStripLine(vector<float> coords); // function to draw strip line of signal

  DECLARE_EVENT_TABLE()
};

// DECLARE MY TEXT EDITOR 
class MyEditor : public wxFrame {
    public:
      MyEditor(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size, long style = 0);
      wxTextCtrl *text; 
    private:
        void OnSave(wxCommandEvent &event); // the click event for "save"
        void OnOpen(wxCommandEvent &event); // the click event for "open"
        void OnExit(wxCommandEvent &event); // the click event for "close"
        void OnRun(wxCommandEvent &event);   // the click event for "run"
        
        DECLARE_EVENT_TABLE()
};

    
#endif /* gui_h */
