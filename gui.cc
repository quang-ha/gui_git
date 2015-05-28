/// CAUTION!!!!! THESE GUI FUNCTIONS CURRENTLY ONLY WORK WITH THE ORIGINALLY
/// PROVIDED FUNCTIONS (i.e. empty files of name/devices...). THIS CAN BE DONE
/// BY REVERTING THOSE FILES BACK TO ORIGINAL COMMIT
/// git checkout ac14d330fa3c9ba5bc620db68a634aaa356a0487 *files

#include <wx/wxprec.h>

#include "wx/toolbar.h"
#include "wx/log.h"
#include "wx/image.h"
#include "wx/filedlg.h"
#include "wx/colordlg.h"
#include "wx/srchctrl.h"
#include <wx/textdlg.h>

#include "gui.h"
#include <GL/glut.h>
#include "wx_icon.xpm"
#include <iostream>
#include <algorithm>
#include <wx/grid.h>

using namespace std;

// MyGLCanvas ////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
  EVT_SIZE(MyGLCanvas::OnSize)
  EVT_PAINT(MyGLCanvas::OnPaint)
  EVT_MOUSE_EVENTS(MyGLCanvas::OnMouse)
END_EVENT_TABLE()
  
int wxglcanvas_attrib_list[5] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};

MyGLCanvas::MyGLCanvas(wxWindow *parent, wxWindowID id, monitor* monitor_mod, names* names_mod, const wxPoint& pos, 
		       const wxSize& size, long style, const wxString& name, const wxPalette& palette):
  wxGLCanvas(parent, id, wxglcanvas_attrib_list, pos, size, style, name, palette)
  // Constructor - initialises private variables
{
  context = new wxGLContext(this);
  mmz = monitor_mod;
  nmz = names_mod;
  init = false;
  cyclesdisplayed = 10;
  cycles = 10;
}

void MyGLCanvas::DrawStripLine(vector<float> coords)
{
  float x,y;
  glColor3f(1.0, 0.5, 0.0);
  glLineWidth(2.0); 
  glBegin(GL_LINE_STRIP);
  for (int i=0; (2*i)<coords.size(); i++)
  {
    x = coords[2*i];
    y = coords[2*i+1];
    glVertex2f(x, y);
  }
  glEnd();
}

void MyGLCanvas::DrawDashLine(vector<float> coords)
{
  wxString mystring;
  int w, h, x_dash;
  GetClientSize(&w, &h);
  glColor3f(1.0, 1.0, 1.0);
  glLineWidth(1.0);
  glLineStipple(0, 0xAAAA); 
  glEnable(GL_LINE_STIPPLE);
  for (int i = 0; i<coords.size(); i++)
  {
    if ((i%4) == 0)
    {
      glBegin(GL_LINES);
      glVertex2f(coords[i], h);
      glVertex2f(coords[i],0.0);
      glEnd();
      x_dash = i/4; 
        if (x_dash<10) glRasterPos2f(coords[i]-8, h-10);
        else if (x_dash<100) glRasterPos2f(coords[i]-15, h-10);
        mystring = wxString::Format(wxT("%i"),x_dash);
        for (int j = 0; j < mystring.Len(); j++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, mystring[j]);
    }
  }
  glDisable(GL_LINE_STIPPLE);
}

void MyGLCanvas::Render(wxString example_text)
  // Draws canvas contents - the following example writes the string "example text" onto the canvas
  // and draws a signal trace. The trace is artificial if the simulator has not yet been run.
  // When the simulator is run, the number of cycles is passed as a parameter and the first monitor
  // trace is displayed.

  // which return 0 if the number is cycle, and 1 if the number if cycledisplay
{ 
  cout << mmz->moncount() << endl;

  float y;
  unsigned int i;
  asignal s;
  int display;
  double x, x1;
  vector<float> coords;
  if (cycles < cyclesdisplayed) display = cycles;
  else display = cyclesdisplayed;

  // Get size of the canvas and work out the pulse width
  int w, h;
  GetClientSize(&w, &h);
  double pulse_width = (w)/(display);

  // Get the vertical offset from the slider
  float off_v = h*offset_v / 200.0;
  float off_h = w*offset_h / 200.0; 

  SetCurrent(*context);
  if (!init) {
    InitGL();
    init = true;
  }

  glClear(GL_COLOR_BUFFER_BIT);

  float y_min = (h-40.0)-30.0 + off_v;
  float y_max = (h-40.0)-10.0 + off_v;

  if ((cyclesdisplayed >= 0) && (mmz->moncount() > 0)) { // push the x into the vector
    for (i=0; i<=cycles; i++) {
      if (mmz->getsignaltrace(0, i, s)) {
	     if (s==low) y = y_min;
	     if (s==high) y = y_max;
       x = pulse_width*i - off_h;
       //if (x>w) x=w;
       coords.push_back(x);
       coords.push_back(y);
       if (i<cycles)
        {
          x1 = pulse_width*i + pulse_width - off_h;
          coords.push_back(x1);
          coords.push_back(y);
        }
      }
    }

  } else { // draw an artificial trace
    for (i=0; i<=cycles; i++) {
      if (i%2) y = y_min;
      else y = y_max;
       x = pulse_width*i - off_h;
       //if (x>w) x=w;
       coords.push_back(x);
       coords.push_back(y);
       if (i<cycles)
       {
        x1 = pulse_width*i + pulse_width - off_h;
        coords.push_back(x1);
        coords.push_back(y);
       }
    }
  }
  DrawStripLine(coords);
  DrawDashLine(coords);

  // Example of how to use GLUT to draw text on the canvas - THIS IS IMPORTANT
  glColor3f(0.0, 0.0, 1.0);
  glRasterPos2f(10, 100);
  for (i = 0; i < example_text.Len(); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, example_text[i]);

  // We've been drawing to the back buffer, flush the graphics pipeline and swap the back buffer to the front
  glFlush();
  SwapBuffers();
}

void MyGLCanvas::InitGL()
  // Function to initialise the GL context
{
  int w, h;

  GetClientSize(&w, &h);
  SetCurrent(*context);
  glDrawBuffer(GL_BACK);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glViewport(0, 0, (GLint) w, (GLint) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, -1, 1); 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void MyGLCanvas::OnPaint(wxPaintEvent& event)
  // Event handler for when the canvas is exposed
{
  int w, h;
  wxString text;

  wxPaintDC dc(this); // required for correct refreshing under MS windows
  GetClientSize(&w, &h);
  text.Printf("Canvas redrawn by OnPaint event handler, canvas size is %d by %d", w, h);
  Render(text);
}

void MyGLCanvas::OnSize(wxSizeEvent& event)
  // Event handler for when the canvas is resized
{
  init = false;; // this will force the viewport and projection matrices to be reconfigured on the next paint
}

void MyGLCanvas::OnMouse(wxMouseEvent& event)
  // Event handler for mouse events inside the GL canvas
{
  wxString text;
  int w, h;;

  GetClientSize(&w, &h);
  if (event.ButtonDown()) text.Printf("Mouse button %d pressed at %d %d", event.GetButton(), event.m_x, h-event.m_y);
  if (event.ButtonUp()) text.Printf("Mouse button %d released at %d %d", event.GetButton(), event.m_x, h-event.m_y);
  if (event.Dragging()) text.Printf("Mouse dragged to %d %d", event.m_x, h-event.m_y);
  if (event.Leaving()) text.Printf("Mouse left window at %d %d", event.m_x, h-event.m_y);

  if (event.ButtonDown() || event.ButtonUp() || event.Dragging() || event.Leaving()) Render(text);
}

// ***** MY PANEL ****** //
// MyFrame::MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size,
//      names *names_mod, devices *devices_mod, monitor *monitor_mod, long style):
//   wxFrame(parent, wxID_ANY, title, pos, size, styl

MyPanel::MyPanel(MyFrame *parent, names *names_mod, monitor *monitor_mod, devices *devices_mod, network *network_mod)
       : wxPanel(parent, wxID_ANY)
{
  nmz = names_mod;
  mmz = monitor_mod;
  dmz = devices_mod;
  netz = network_mod;

  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

  real_frame = parent;
  //MyFrame *real_frame = dynamic_cast<MyFrame*>(GetParent()->GetParent());
  m_lb = real_frame->listbox;

  wxArrayString str;
  devlink device = netz->devicelist();
  while (device!=NULL)
  {
    outplink output;
    output = device->olist;
    while (output!=NULL)
    {
      if (output->id == blankname) 
        str.Add(nmz->translate(device->id));
      else
        str.Add(nmz->translate(device->id)+'.'+nmz->translate(output->id));
      output = output->next;
    }
    device = device->next;
  }

  wxChoice *Cp = new wxChoice(this, ID_ADD, wxDefaultPosition, wxDefaultSize, str);
  // End of string list.

  m_deleteb = new wxButton(this, wxID_DELETE, wxT("Delete"));

  Connect(wxID_DELETE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyPanel::OnDelete) );
  Connect(ID_ADD, wxEVT_CHOICE, wxCommandEventHandler(MyPanel::OnAdd));

  vbox->Add(Cp);
  //vbox->Add(m_addb, 0, wxTOP, 5)
  vbox->Add(m_deleteb, 0, wxTOP, 5);

  SetSizer(vbox);
}

void MyPanel::TranslateMonitorName(wxString str, name &device, name &out)
{
  string temp = str.ToStdString();
    size_t dot_pos = str.find('.');
    if (dot_pos == string::npos)
    {
      device = nmz->lookup(temp);
      out = blankname;
    }
    else
    {
      device = nmz->lookup(temp.substr(0, dot_pos));
      out = nmz->lookup(temp.substr(dot_pos+1, string::npos));
    }
}

void MyPanel::OnAdd(wxCommandEvent& event) 
{
  bool OK;
  wxString str = event.GetString();
  cout << "Is there parent?: " << (real_frame == NULL) << endl;
  cout << "Is there canvas?: " << (real_frame->canvas == NULL) << endl;
  if (str.Len() > 0)
  {
    name device, out;
    TranslateMonitorName(str, device, out);
    m_lb->Append(str); 
    mmz->makemonitor(device, out, OK);
  }
  real_frame->canvas->Render("Add new monitor");
}

void MyPanel::OnDelete(wxCommandEvent& event) 
{
  bool OK;
  int sel = m_lb->GetSelection();
  if (sel != -1) {
      wxString str = m_lb->GetString(sel);
      name device, output;
      TranslateMonitorName(str, device, output);
      mmz->remmonitor(device, output, OK);
      m_lb->Delete(sel);
  }
  real_frame->canvas->Render("Remove monitor");
}

// ***** MY PANEL - ENDED ****** //


// MyFrame ///////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MyFrame, wxFrame)

  // ** TOOLBAR ** //

  // ** MENU ** //
  // FILE
  //EVT_MENU(wxID_NEW, MyFrame::OnNew)
  EVT_MENU(wxID_OPEN, MyFrame::OnOpen)
  EVT_MENU(wxID_SAVE, MyFrame::OnSave)
  EVT_MENU(wxID_EXIT, MyFrame::OnExit)

  // VIEW
  EVT_MENU(MY_ZOOM_ID, MyFrame::OnZoom)

  // SIMULATE
  EVT_MENU(wxID_FORWARD, MyFrame::OnRun)

  // HELP
  EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)

  // SPIN
  EVT_SPINCTRL(MY_SPINCTRL_ID, MyFrame::OnSpin)

  // SPIN DISPLAY
  EVT_SPINCTRL(MY_SCROLL_ID, MyFrame::OnScroll)

  // SCROLL
  EVT_COMMAND_SCROLL(MY_SLIDER_H_ID, MyFrame::OnScrollH)
  EVT_COMMAND_SCROLL(MY_SLIDER_V_ID, MyFrame::OnScrollV)

  // THE LIST BOX ON THE RIGHT

  // INPUT TEXT
  EVT_MENU(MY_TEXTCTRL_ID, MyFrame::OnText)


END_EVENT_TABLE()
  
MyFrame::MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size,
		 names *names_mod, devices *devices_mod, monitor *monitor_mod, network *network_mod, long style):
  wxFrame(parent, wxID_ANY, title, pos, size, style)
  // Constructor - initialises pointers to names, devices and monitor classes, lays out widgets
  // using sizers
{
  SetIcon(wxIcon(wx_icon));

  nmz = names_mod;
  dmz = devices_mod;
  mmz = monitor_mod;
  netz = network_mod;

  if (nmz == NULL || dmz == NULL || mmz == NULL) {
    cout << "Cannot operate GUI without names, devices and monitor classes" << endl;
    exit(1);
  }
  // ** MENU ** //
  // FILE 
  wxMenu *fileMenu = new wxMenu;
  fileMenu->Append(wxID_OPEN, "&Open\tCtrl-O");
  fileMenu->Append(wxID_SAVE, "&Save\tCtrl-S");
  fileMenu->Append(wxID_EXIT, "&Quit\tCtrl-Q");

  // VIEW
  wxMenu *viewMenu = new wxMenu;
  viewMenu->AppendCheckItem(MY_ZOOM_ID, "&Zoom...");

  // EDIT
  wxMenu *editMenu = new wxMenu;
  editMenu->Append(MY_TEXTCTRL_ID, "Open Text Editor...");

  // HELP
  wxMenu *helpMenu = new wxMenu;
  helpMenu->Append(wxID_ABOUT, "&About");

  // ** TOOLBAR ** // 
  wxToolBar *toolbar = CreateToolBar();

  // Open
  wxBitmap open_bmp("bitmaps/open.xpm", wxBITMAP_TYPE_XPM);
  toolbar->AddTool(wxID_OPEN, wxT("Open"), open_bmp);
  Connect(wxID_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnOpen));

  // Save
  wxBitmap save_bmp("bitmaps/save.xpm", wxBITMAP_TYPE_XPM);
  toolbar->AddTool(wxID_SAVE, wxT("Save"), save_bmp);
  Connect(wxID_SAVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnSave));
  toolbar->AddSeparator();

  // Number of Cycles

  wxStaticText *txt = new wxStaticText(toolbar, wxID_ANY, "Run");
  toolbar->AddControl(txt);
  wxSpinCtrl *spin = new wxSpinCtrl(toolbar, MY_SPINCTRL_ID, wxString("10"));
  spin->SetRange(1, 100);
  toolbar->AddControl(spin);

  // Static text
  wxStaticText *txt1 = new wxStaticText(toolbar, wxID_ANY, "Cycles");
  toolbar->AddControl(txt1);
  // RUN
  toolbar->AddSeparator();
  wxBitmap print_bmp("bitmaps/print.xpm", wxBITMAP_TYPE_XPM);
  toolbar->AddTool(wxID_FORWARD, wxT("Run"), print_bmp);
  Connect(wxID_FORWARD, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnRun));

  toolbar->Realize();
  Centre();

  // Draw the menuBar
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(fileMenu, "&File");
  menuBar->Append(editMenu, "&Edit");
  menuBar->Append(viewMenu, "&View");
  menuBar->Append(helpMenu, "&Help");
  SetMenuBar(menuBar);

  // // Top sizer
  wxBoxSizer *topsizer = new wxBoxSizer(wxHORIZONTAL);

  // // LEFT
  wxBoxSizer *sizer_left_top = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *sizer_left = new wxBoxSizer(wxVERTICAL);
  canvas = new MyGLCanvas(this, wxID_ANY, monitor_mod, names_mod);
  sizer_left->Add(canvas, 10, wxEXPAND | wxLEFT, 10);
  slider_h = new wxSlider(this, MY_SLIDER_H_ID, 0, 0, 200, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
  Connect(MY_SLIDER_H_ID, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(MyFrame::OnScrollH));
  sizer_left->Add(slider_h, 1, wxEXPAND | wxTOP | wxBOTTOM | wxLEFT, 5);
  sizer_left_top->Add(sizer_left, 3, wxEXPAND | wxTOP, 10);

  topsizer->Add(sizer_left_top, 100, wxEXPAND);

  // // middle, this is just slider
  wxBoxSizer *sizer_middle = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *sizer_middle_top = new wxBoxSizer(wxVERTICAL);
  slider_v = new wxSlider(this, MY_SLIDER_V_ID, 0, 0, 200, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL );
  Connect(MY_SLIDER_V_ID, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(MyFrame::OnScrollV));
  sizer_middle->Add(slider_v, 1, wxEXPAND | wxTOP, 10);
  sizer_middle_top->Add(sizer_middle, 1, wxEXPAND | wxBOTTOM, 40);
  topsizer->Add(sizer_middle_top, 1, wxEXPAND);

  // right, where the rest of the controls are
  wxBoxSizer *button_sizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *view_spin_sizer = new wxBoxSizer(wxHORIZONTAL);
  view_spin_sizer->Add(new wxStaticText(this, wxID_ANY, "Cycles to Display: "), 0, wxRIGHT, 10);
  view_spin = new wxSpinCtrl(this, MY_SCROLL_ID, wxString("10"));
  view_spin->SetRange(1, 100);
  view_spin_sizer->Add(view_spin, 0, wxRIGHT, 10);
  button_sizer->Add(view_spin_sizer, 0, wxALL, 10);

  // ** LISTBOX FINISHED ** //
  // TO-DO LIST: INTEGRATE WITH OUTPUT FROM DEVICES // 
  wxStaticBox *box = new wxStaticBox(this, wxID_ANY, "Output Monitor");
  wxStaticBoxSizer *button_sizer_panel = new wxStaticBoxSizer(box, wxHORIZONTAL);
  wxPanel *panel = new wxPanel(this, -1);
  wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *hbox_right = new wxBoxSizer(wxHORIZONTAL);
  listbox = new wxListBox(panel, ID_LISTBOX, wxDefaultPosition, wxDefaultSize); 
  hbox->Add(listbox, 1, wxEXPAND | wxALL, 5);
  btnPanel = new MyPanel(this, names_mod, monitor_mod, devices_mod, network_mod);
  hbox_right->Add(btnPanel, 1, wxEXPAND | wxALL, 5);
  hbox->Add(hbox_right, 1, wxEXPAND | wxALL, 5);
  panel->SetSizer(hbox);
  button_sizer_panel->Add(panel, wxALIGN_CENTRE);
  button_sizer->Add(button_sizer_panel, 2, wxALL, 10);

  // ** ADD CONTROL FOR SWITCHES ** // 
  // Box
  wxStaticBox *radio_text = new wxStaticBox(this, wxID_ANY, "Switch Control");
  wxStaticBoxSizer *radio = new wxStaticBoxSizer(radio_text, wxHORIZONTAL);
  // Add a list of switches
  wxArrayString str;
  devlink device = netz->devicelist();
  while (device!=NULL)
  {
    if (device->kind == aswitch) 
    {
      str.Add(nmz->translate(device->id));
    }
    device = device->next;
  }
  switchcontrol = new wxChoice(this, MY_SWITCH_CONTROL, wxDefaultPosition, wxDefaultSize, str);
  Connect(MY_SWITCH_CONTROL, wxEVT_CHOICE, wxCommandEventHandler(MyFrame::OnSelectSwitch));
  radio->Add(switchcontrol, 0, wxALL, 10);
  // Now add the state control
  changestate = new wxCheckBox(this, CHANGE_SWITCH_STATE, wxT("On"), wxDefaultPosition, wxDefaultSize);
  Connect(CHANGE_SWITCH_STATE, wxEVT_CHECKBOX, wxCommandEventHandler(MyFrame::OnChangeState));
  radio->Add(changestate, 0, wxALL, 10);

  button_sizer->Add(radio, 0, wxALL, 10);
  // **************
  topsizer->Add(button_sizer, 0, wxALL, 10);

  SetSizeHints(400, 100);
  SetSizer(topsizer);

}

void MyFrame::OnSelectSwitch(wxCommandEvent& event)
{
  bool OK;
  bool on;
  wxString str = event.GetString();
  if (str.Len() > 0)
  {
    devlink sw = netz->finddevice(nmz->lookup(str.ToStdString()));
    if (sw->swstate == high)
      changestate->SetValue(true); // Here you have to change the value 
    else if (sw->swstate == low)
      changestate->SetValue(false);
  }
  canvas->Update();  
}

void MyFrame::OnChangeState(wxCommandEvent& event)
{
  bool OK;
  int n = switchcontrol->GetCurrentSelection();
  wxString str = switchcontrol->GetString(n);
  if (event.IsChecked())
    dmz->setswitch(nmz->lookup(str.ToStdString()), high, OK);
  else
    dmz->setswitch(nmz->lookup(str.ToStdString()), low, OK);
  canvas->Update();
}

void MyFrame::OnOpen(wxCommandEvent &event)
{
  canvas->Render("Open button pressed");

  wxFileDialog dialog(this, wxT("Testing open file dialog"), wxEmptyString,
                    wxEmptyString, wxT("NYAYA files (*.nya)|*.nya"));

  //dialog.SetExtraControlCreator(&createMyExtraPanel);
  dialog.CentreOnParent();
  dialog.SetDirectory(wxGetHomeDir());

  // Print out the message
  if (dialog.ShowModal() == wxID_OK)
    {
        wxString info;
        wxWindow * const extra = dialog.GetExtraControl();
        info.Printf(wxT("Full file name: %s\n")
                    wxT("Path: %s\n")
                    wxT("Name: %s\n")
                    wxT("Custom window: %s"),
                    dialog.GetPath().c_str(),
                    dialog.GetDirectory().c_str(),
                    dialog.GetFilename().c_str());
        // This just shows some information for the selected file
        wxMessageDialog dialog2(this, info, wxT("Selected file"));
        dialog2.ShowModal();
        canvas->Render(info);
    }
}

void MyFrame::OnSave(wxCommandEvent &event)
{
  canvas->Render("Save button pressed");
}

void MyFrame::OnExit(wxCommandEvent &event)
  // Event handler for the exit menu item
{
  Close(true);
}

// ** ON VIEW ** //

void MyFrame::OnScrollH(wxScrollEvent &event)
{
  wxString text;
  canvas->offset_h = event.GetPosition();
  text.Printf("New Horizontal Position %d", event.GetPosition());
  canvas->Render(text);
}


void MyFrame::OnScrollV(wxScrollEvent &event)
{
  wxString text;
  canvas->offset_v = event.GetPosition();
  text.Printf("New Vertical Position %d", event.GetPosition());
  canvas->Render(text);
}

// Changing the number of cycles to DISPLAY
void MyFrame::OnScroll(wxSpinEvent& event)
{
  wxString text;
  canvas->cyclesdisplayed = event.GetPosition();
  text.Printf("New display value %d", event.GetPosition(), true);
  canvas->Render(text);
}

void MyFrame::OnZoom(wxCommandEvent& event)
{
  if (event.IsChecked())
    canvas->Render("Zoom activated");
  else
   canvas->Render("Zoom activated");
}

// ** SIMULATE ** //
void MyFrame::OnRun(wxCommandEvent &event)
{
  canvas->Render("Run pressed.");
}

// ** HELP MENU ** //

void MyFrame::OnAbout(wxCommandEvent &event)
  // Event handler for the about menu item
{
  wxMessageDialog about(this, "E. Dakin, K. Kyriakopolous and Q. Ha\nEaster 2015\nUsing NYAYA language.", "Logic Simulator ELEA", wxICON_INFORMATION | wxOK);
  about.ShowModal();
}

// Changing the number of cycles
void MyFrame::OnSpin(wxSpinEvent &event)
  // Event handler for the spin control
{
  wxString text;
  canvas->cycles = event.GetPosition();
  text.Printf("New spinctrl value %d", event.GetPosition());
  canvas->Render(text);
}

void MyFrame::OnText(wxCommandEvent &event)
  // Event handler for opening the text editor
{
    MyEditor *TextBox = new MyEditor(NULL, "Text Editor" , wxDefaultPosition, wxDefaultSize);
    TextBox->Show(True);
}

void MyFrame::runnetwork(int ncycles)
  // Function to run the network, derived from corresponding function in userint.cc
{
  bool ok = true;
  int n = ncycles;

  while ((n > 0) && ok) {
    dmz->executedevices (ok);
    if (ok) {
      n--;
      mmz->recordsignals ();
    } else
      cout << "Error: network is oscillating" << endl;
  }
  if (ok) cyclescompleted = cyclescompleted + ncycles;
  else cyclescompleted = 0;
}

// ************* MyEditor *******************//
BEGIN_EVENT_TABLE(MyEditor, wxFrame) // begin the event table for our Notepad class, which inherits wxFrame
    EVT_MENU(EDITOR_SAVE, MyEditor::OnSave) // set an event for our idSave, and the function OnSave
    EVT_MENU(EDITOR_OPEN, MyEditor::OnOpen) // set an event for open
    EVT_MENU(EDITOR_EXIT, MyEditor::OnExit) // set an event for exit
    EVT_MENU(EDITOR_RUN, MyEditor::OnRun)
END_EVENT_TABLE() // end the event table

MyEditor::MyEditor(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size,long style):
  wxFrame(parent, wxID_ANY, title, pos, size, style)
{
  // ** MENU BAR ** //
  // FILE 
  wxMenuBar *menuBar = new wxMenuBar;
  wxMenu *fileMenu = new wxMenu;
  fileMenu->Append(EDITOR_OPEN, "&Open\tCtrl-O");
  fileMenu->Append(EDITOR_SAVE, "&Save\tCtrl-S");
  fileMenu->AppendSeparator();
  fileMenu->Append(EDITOR_EXIT, "&Quit\tCtrl-Q");
  menuBar->Append(fileMenu, "&File");
  SetMenuBar(menuBar);

    // ** TOOLBAR ** // 
  wxToolBar *toolbar = CreateToolBar();
  // RUN
  wxBitmap print_bmp("bitmaps/print.bmp", wxBITMAP_TYPE_BMP);
  toolbar->AddTool(EDITOR_RUN, wxT("Compile"), print_bmp);
  Connect(EDITOR_RUN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyEditor::OnRun));
  toolbar->Realize();
  Centre();

  text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_MULTILINE);
}

void MyEditor::OnOpen(wxCommandEvent &event)
{
  wxFileDialog *openDialog = new wxFileDialog(this, wxT("Open File~"), wxT(""), wxT(""),
                                                wxT("NYAYA Files (*.nya)|*.nya"), wxFD_OPEN);
  int response = openDialog->ShowModal();
  if (response == wxID_OK) {
        text->LoadFile(openDialog->GetPath());
    }
}

void MyEditor::OnSave(wxCommandEvent &event)
{
  wxFileDialog *saveDialog = new wxFileDialog(this, wxT("Save File~"), wxT(""), wxT(""),
                                                wxT("NYAYA Files (*.nya)|*.nya"), wxFD_SAVE);
  int response = saveDialog->ShowModal();
  if (response == wxID_OK) {
    text->SaveFile(saveDialog->GetPath()); 
  }
}

void MyEditor::OnRun(wxCommandEvent &event)
{

}

void MyEditor::OnExit(wxCommandEvent &event)
{
  Close(true);
}