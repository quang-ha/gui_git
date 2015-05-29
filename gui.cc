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
#include "interpret.h"
#include "parser.h"

using namespace std;

// MyGLCanvas ////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
  EVT_SIZE(MyGLCanvas::OnSize)
  EVT_PAINT(MyGLCanvas::OnPaint)
  EVT_MOUSE_EVENTS(MyGLCanvas::OnMouse)
END_EVENT_TABLE()
  
int wxglcanvas_attrib_list[5] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};

MyGLCanvas::MyGLCanvas(MyFrame *parent, wxWindowID id, monitor* monitor_mod, names* names_mod, devices* devices_mod, const wxPoint& pos, 
		       const wxSize& size, long style, const wxString& name, const wxPalette& palette):
  wxGLCanvas(parent, id, wxglcanvas_attrib_list, pos, size, style, name, palette)
  // Constructor - initialises private variables
{
  context = new wxGLContext(this);
  real_frame = (MyFrame *) this->GetParent();
  mmz = monitor_mod;
  nmz = names_mod;
  dmz = devices_mod;
  init = false;
  width = 0;
  cycles = 0;
}

void MyGLCanvas::DrawStripLine(vector<float> coords)
{
    float x,y;
    glColor3f(1.0, 0.5, 0.0);
    glLineWidth(2.0); 
    glBegin(GL_LINE_STRIP);
    for (int i=0  ; (2*i)<coords.size(); i++)
    {
      x = coords[2*i];
      y = coords[2*i+1];
      glVertex2f(x, y);
    }
    glEnd();
}

void MyGLCanvas::DrawDashLine(vector<float> coords, bool orient)
{
  int w, h, x_dash;
  glColor3f(1.0, 1.0, 1.0);
  GetClientSize(&w, &h);
  glLineWidth(1.0);
  glLineStipple(0, 0xAAAA); 
  glEnable(GL_LINE_STIPPLE);
  if (orient) // This means they are drawing vertical dash lines
  {
    wxString mystring;
    for (int i = 0; i<coords.size(); i++)
      {
        glBegin(GL_LINES);
        glVertex2f(coords[i], h);
        glVertex2f(coords[i],0.0);
        glEnd();
        if (i<10) glRasterPos2f(coords[i]-8, h-10);
        else if (i<100) glRasterPos2f(coords[i]-15, h-10);
        mystring = wxString::Format(wxT("%i"),i);
        for (int j = 0; j < mystring.Len(); j++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, mystring[j]);
      }
  }
  else // Drawing the horizontal dash lines, no need to write any text
  {
    glBegin(GL_LINES);
    glVertex2f(0, coords[0]);
    glVertex2f(w, coords[0]);
    glEnd();
  }
  glDisable(GL_LINE_STIPPLE);
}

void MyGLCanvas::Render(wxString example_text, bool redraw)
  // Draws canvas contents - the following example writes the string "example text" onto the canvas
  // and draws a signal trace. The trace is artificial if the simulator has not yet been run.
  // When the simulator is run, the number of cycles is passed as a parameter and the first monitor
  // trace is displayed.

  // which return 0 if the number is cycle, and 1 if the number if cycledisplay
{ 
  // ** Clear the canvas
  if (redraw)
   {
    wxPaintDC dc(this);  
    dc.Clear();
    }

  SetCurrent(*context);
  if (!init) {
    InitGL();
    init = true;
  }

  glDrawBuffer(GL_BACK);
  glClear(GL_COLOR_BUFFER_BIT);
  // ** Clear the canvas

  float y;
  unsigned int i;
  asignal s;
  float x, x1;
  vector<float> coords, coords_dash; // Vector to store the coordinate to draw the signal trace
  coords_dash.clear();
  coords.clear();
  int draw = real_frame->cyclescompleted;

  if ((width>0) && (mmz->moncount()>0))
  {
    // Get size of the canvas and work out the pulse width
    int w, h;
    GetClientSize(&w, &h);
    float pulse_width = w/width;
    float extra_off = 20.0;
    float pulse_rise = (h-20*(height+1))/(height);

    // Get the vertical offset from the slider
    // float off_v = 20*h*offset_v / (mmz->moncount()*pulse_rise); 
    // float off_h = 20*w*offset_h / (cycles*pulse_width); 
    float off_v = offset_v*(mmz->moncount()*pulse_rise) / 200.0 ;
    float off_h = offset_h*(cycles*pulse_width) / 200.0 ;

    //float y_min; // = (h-10.0)-pulse_rise*2 + off_v;
    //float y_max; // = (h-10.0)-pulse_rise + off_v;

    // Draw Dash line
    for (int j=0; j<=draw; j++)
    {
      coords_dash.push_back(pulse_width*j - off_h);
    }
    DrawDashLine(coords_dash, true);
    coords_dash.clear();

    name device;
    name output;
    // Draw Trace of Each Monitor Points
      for (int l=0; l<mmz->moncount(); l++)
      {
        for (i=0; i<=draw; i++) 
          {
            if (mmz->getsignaltrace(l, i, s)) 
            {
	            if ((s==low) or (s==falling)) y = h - 20*(l+1) - pulse_rise*(l+1) + off_v;
  	          if ((s==high) or (s==rising)) y = h - 20*(l+1) - pulse_rise*l + off_v;
              x = pulse_width*i - off_h;
              coords.push_back(x);
              coords.push_back(y);
              if (i<draw) // This is a clumsy way to deal with the end of the signal
                {
                  x1 = pulse_width*i + pulse_width - off_h;
                  coords.push_back(x1);
                  coords.push_back(y);
                }
            } 
          }
        if (!coords.empty())
          {
            coords_dash.push_back(h - 20*(l+1) - pulse_rise*(l+1) + off_v);
            DrawDashLine(coords_dash, false);
            DrawStripLine(coords); // Draw with changing colours everytime
            int monpoint = l;
            mmz->getmonname(monpoint, device, output);
            glRasterPos2f(10.0, h - 20*(l+1) - pulse_rise*(l+1) + 10 + off_v);
            string outputstr;
            if (output!=blankname)
              outputstr = dmz->dvs->operator[](device) + '.' +dmz->dvs->operator[](output);
            else
              outputstr = dmz->dvs->operator[](device);
            wxString outputname(outputstr);
            for (int j = 0; j < outputname.Len(); j++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, outputname[j]);          
          }
        coords.clear();
        coords_dash.clear();
      }
  }
  // Example of how to use GLUT to draw text on the canvas - THIS IS IMPORTANT
  glColor3f(1.0, 1.0, 1.0);
  glRectf(8,8, example_text.Len()*7.5, 20);
  glColor3f(0.0, 0.0, 0.0);
  glRasterPos2f(10, 10);
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
  //text.Printf("Canvas redrawn by OnPaint event handler, canvas size is %d by %d", w, h);
  Render("Resize Canvas", true);
}

void MyGLCanvas::OnSize(wxSizeEvent& event)
  // Event handler for when the canvas is resized
{
  init = false;; // this will force the viewport and projection matrices to be reconfigured on the next paint
}

void MyGLCanvas::OnMouse(wxMouseEvent& event)
  // Event handler for mouse events inside the GL canvas
{
  // wxString text;
  // int w, h;;

  // GetClientSize(&w, &h);
  // if (event.ButtonDown()) text.Printf("Mouse button %d pressed at %d %d", event.GetButton(), event.m_x, h-event.m_y);
  // if (event.ButtonUp()) text.Printf("Mouse button %d released at %d %d", event.GetButton(), event.m_x, h-event.m_y);
  // if (event.Dragging()) text.Printf("Mouse dragged to %d %d", event.m_x, h-event.m_y);
  // if (event.Leaving()) text.Printf("Mouse left window at %d %d", event.m_x, h-event.m_y);

  // if (event.ButtonDown() || event.ButtonUp() || event.Dragging() || event.Leaving()) Render(text, false);
}

// ***** MY PANEL ****** //

MyPanel::MyPanel(wxPanel *parent, names *names_mod, monitor *monitor_mod, devices *devices_mod, network *network_mod)
       : wxPanel(parent, wxID_ANY)
{
  nmz = names_mod;
  mmz = monitor_mod;
  dmz = devices_mod;
  netz = network_mod;

  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

  real_frame = (MyFrame *) parent->GetParent();
  m_lb = real_frame->listbox;

  // Add the monitoring point to the list box
  wxArrayString mons; mons.Empty();
  name dev; name out;
  for (int i=0; i<mmz->moncount(); i++)
  {
    mmz->getmonname(i, dev, out);
    if (out == blankname) 
      mons.Add(dmz->dvs->operator[](dev));
    else
      mons.Add(dmz->dvs->operator[](dev)+'.'+dmz->dvs->operator[](out));
  } 
  m_lb->Append(mons);

  wxArrayString str;
  string temp;
  devlink device = netz->devicelist();
  while (device!=NULL)
  {
    outplink output;
    output = device->olist;
    while (output!=NULL)
    {
      temp = dmz->dvs->operator[](device->id);
      if (temp.find('/') ==string::npos)
      {
        if (output->id == blankname)
          str.Add(temp);
        else
          str.Add(temp+'.'+dmz->dvs->operator[](output->id));
      }
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
      device = dmz->dvs->lookup(temp);
      out = blankname;
    }
    else
    {
      device = dmz->dvs->lookup(temp.substr(0, dot_pos));
      out = dmz->dvs->lookup(temp.substr(dot_pos+1, string::npos));
    }
}

void MyPanel::OnAdd(wxCommandEvent& event) 
{
  bool OK;
  wxString str = event.GetString();
  if (str.Len() > 0)
  {
    name device, out;
    TranslateMonitorName(str, device, out);
    m_lb->Append(str); 
    mmz->makemonitor(device, out);
  }
  real_frame->canvas->Render("Add new monitor", true);
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
  cout << mmz->moncount() << endl;
  real_frame->canvas->Render("Remove monitor", true);
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
  EVT_SPINCTRL(MY_HEIGHTCTRL_ID, MyFrame::OnHeight)

  // SPIN DISPLAY
  EVT_SPINCTRL(MY_SCROLL_ID, MyFrame::OnScroll)

  // SCROLL
  EVT_COMMAND_SCROLL(MY_SLIDER_H_ID, MyFrame::OnScrollH)
  EVT_COMMAND_SCROLL(MY_SLIDER_V_ID, MyFrame::OnScrollV)

  // THE LIST BOX ON THE RIGHT

  // INPUT TEXT
  EVT_MENU(MY_TEXTCTRL_ID, MyFrame::OnText)
  EVT_MENU(TOOLBAR_TEXTCTRL_ID, MyFrame::OnText)


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
  // // ** MENU ** //
  // // FILE 
  // wxMenu *fileMenu = new wxMenu;
  // fileMenu->Append(wxID_OPEN, "&Open\tCtrl-O");
  // // fileMenu->Append(wxID_SAVE, "&Save\tCtrl-S"); // ** This needs to be replaced with eport ** 
  // fileMenu->Append(wxID_EXIT, "&Quit\tCtrl-Q");

  // // VIEW
  // wxMenu *viewMenu = new wxMenu;
  // viewMenu->AppendCheckItem(MY_ZOOM_ID, "&Zoom...");

  // // EDIT
  // wxMenu *editMenu = new wxMenu;
  // editMenu->Append(MY_TEXTCTRL_ID, "Open Text Editor...");

  // // HELP
  // wxMenu *helpMenu = new wxMenu;
  // helpMenu->Append(wxID_ABOUT, "&About");

  // // Draw the menuBar
  // wxMenuBar *menuBar = new wxMenuBar;
  // menuBar->Append(fileMenu, "&File");
  // menuBar->Append(editMenu, "&Edit");
  // menuBar->Append(viewMenu, "&View");
  // menuBar->Append(helpMenu, "&Help");
  // SetMenuBar(menuBar);

  // // ** TOOLBAR ** // 
  // wxToolBar *toolbar = CreateToolBar();

  // // Open
  // wxBitmap open_png("bitmaps/recent.png", wxBITMAP_TYPE_PNG);
  // toolbar->AddTool(wxID_OPEN, wxT("Open"), open_png);
  // Connect(wxID_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnOpen));

  // // Save
  // wxBitmap edit_png("bitmaps/edit-6.png", wxBITMAP_TYPE_PNG);
  // toolbar->AddTool(TOOLBAR_TEXTCTRL_ID, wxT("Preview"), edit_png);
  // toolbar->AddSeparator();
  // toolbar->Realize();
  // Centre();

  // ** TOP SIZER ** // 
  wxBoxSizer *topsizer = new wxBoxSizer(wxHORIZONTAL);

  // // LEFT
  wxBoxSizer *sizer_left_top = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *sizer_left = new wxBoxSizer(wxVERTICAL);
  canvas = new MyGLCanvas(this, wxID_ANY, monitor_mod, names_mod, devices_mod);
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
    view_spin_sizer->Add(new wxStaticText(this, wxID_ANY, "Width: "), 0, wxRIGHT, 10);
      view_spin = new wxSpinCtrl(this, MY_SCROLL_ID, wxString("0"));
      view_spin->SetValue(0);
      view_spin->SetRange(0, 100);
    view_spin_sizer->Add(view_spin, 0, wxRIGHT, 10);
  button_sizer->Add(view_spin_sizer, 1, wxALL, 10);
  
  wxBoxSizer *height_spin_sizer = new wxBoxSizer(wxHORIZONTAL);
    height_spin_sizer->Add(new wxStaticText(this, wxID_ANY, "Height: "), 0, wxRIGHT, 10);
      height_spin = new wxSpinCtrl(this, MY_HEIGHTCTRL_ID, wxString("0"));
      height_spin->SetValue(0);
      height_spin->SetRange(0, 100);
    height_spin_sizer->Add(height_spin, 0, wxRIGHT, 10);
  button_sizer->Add(height_spin_sizer, 1, wxALL, 10);

  // ** LISTBOX FINISHED ** //
  // TO-DO LIST: INTEGRATE WITH OUTPUT FROM DEVICES // 
  wxStaticBox *box = new wxStaticBox(this, wxID_ANY, "Output Monitor");
  wxStaticBoxSizer *button_sizer_panel = new wxStaticBoxSizer(box, wxHORIZONTAL);
      wxPanel *panel = new wxPanel(this, -1);
        wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
        wxBoxSizer *hbox_right = new wxBoxSizer(wxHORIZONTAL);
          listbox = new wxListBox(panel, ID_LISTBOX, wxDefaultPosition, wxDefaultSize); 
          hbox->Add(listbox, 1, wxEXPAND | wxALL, 5);
          btnPanel = new MyPanel(panel, names_mod, monitor_mod, devices_mod, network_mod);
        hbox_right->Add(btnPanel, 1, wxEXPAND | wxALL, 5);
      hbox->Add(hbox_right, 1, wxEXPAND | wxALL, 5);
    panel->SetSizer(hbox);
    button_sizer_panel->Add(panel, wxALIGN_CENTRE);
  button_sizer->Add(button_sizer_panel, 3, wxALL, 10);

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
      str.Add(dmz->dvs->operator[](device->id));
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
  // ************** ADD THE CYCLES AND RUN *** ///
   // Number of Cycles
  wxStaticBox *run_text = new wxStaticBox(this, wxID_ANY, "Execute");
  wxStaticBoxSizer *run_sizer_top = new wxStaticBoxSizer(run_text, wxVERTICAL);
  wxBoxSizer *run_sizer = new wxBoxSizer(wxHORIZONTAL);
  //wxStaticText *txt = new wxStaticText(this, wxID_ANY, "Run for: ");
  cyc = new wxSpinCtrl(this, MY_SPINCTRL_ID);
  cyc->SetValue(2);
  cyc->SetRange(1, 100);
  //run_sizer->Add(txt, 0, wxALL, 10);
  run_sizer->Add(cyc, 0, wxALL, 10);
  // Static text
  wxStaticText *txt1 = new wxStaticText(this, wxID_ANY, "Cycles");
  run_sizer->Add(txt1, 1);
  run_sizer_top->Add(run_sizer, 0, wxALL, 10);
  // RUN and RESET MONITORs
  wxBoxSizer *run_sizer_2 = new wxBoxSizer(wxHORIZONTAL);
  wxButton *run = new wxButton(this, wxID_FORWARD, wxT("Run"));
  Connect(wxID_FORWARD, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnRun));
  wxButton *reset = new wxButton(this ,wxID_RESET, wxT("Reset Monitor"));
  Connect(wxID_RESET, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnResetMonitor));
  run_sizer_2->Add(run, 0, wxALL, 10);
  run_sizer_2->Add(reset, 0, wxALL, 10);
  run_sizer_top->Add(run_sizer_2, 1);
  button_sizer->Add(run_sizer_top, 1);
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
    devlink sw = netz->finddevice(dmz->dvs->lookup(str.ToStdString()));
    if (sw->swstate == high)
      changestate->SetValue(true); // Here you have to change the value 
    else if (sw->swstate == low)
      changestate->SetValue(false);
  }
  canvas->Render("Select New Switch", true);  
}

void MyFrame::OnChangeState(wxCommandEvent& event)
{
  int n = switchcontrol->GetCurrentSelection();
  wxString str = switchcontrol->GetString(n);
  if (event.IsChecked())
    dmz->setswitch(str.ToStdString(), high);
  else
    dmz->setswitch(str.ToStdString(), low);
  canvas->Render("Change Switch State", true);  
}

void MyFrame::OnOpen(wxCommandEvent &event)
{
  vector<nyaya_exception> e; vector<nyaya_exception> w;

  canvas->Render("Open button pressed", true);

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
                    wxT("Name: %s\n"),
                    dialog.GetPath().c_str(),
                    dialog.GetDirectory().c_str(),
                    dialog.GetFilename().c_str());
        // This just shows some information for the selected file
        wxMessageDialog dialog2(this, info, wxT("Selected file"));
        dialog2.ShowModal();
        canvas->Render(info, false);
    }
  string FileName = dialog.GetPath().ToStdString();
  cout << "This is the File Name " << FileName << endl;
  //
  // interpret(dmz,netz,mmz,&e,&w,FileName);
}

void MyFrame::OnSave(wxCommandEvent &event)
{
  canvas->Render("Save button pressed", false);
}

void MyFrame::OnResetMonitor(wxCommandEvent& event)
{
  mmz->resetmonitor();
  canvas->Render("Monitor Reset", true);
}

void MyFrame::OnExit(wxCommandEvent &event)
  // Event handler for the exit menu item
{
  Close(true);
}

void MyFrame::OnHeight(wxSpinEvent &event)
{
  canvas->height = event.GetPosition();
  wxString txtheight;
  txtheight.Printf("New width value %d", event.GetPosition());
  canvas->Render(txtheight, true);
}

void MyFrame::OnScrollH(wxScrollEvent &event)
{
  wxString text;
  canvas->offset_h = event.GetPosition();
  text.Printf("Vertical Scrolling %d", event.GetPosition());
  canvas->Render(text, false);
}


void MyFrame::OnScrollV(wxScrollEvent &event)
{
  wxString text;
  canvas->offset_v = event.GetPosition();
  text.Printf("Horizontal Scrolling %d", event.GetPosition());
  canvas->Render(text, false);
}

// Changing the number of cycles to DISPLAY
void MyFrame::OnScroll(wxSpinEvent& event)
{
  wxString txtwidth;
  canvas->width = event.GetPosition();
  txtwidth.Printf("New width value %d", event.GetPosition());
  canvas->Render(txtwidth, true);
}

void MyFrame::OnZoom(wxCommandEvent& event)
{
  if (event.IsChecked())
    canvas->Render("Zoom activated", false);
  else
   canvas->Render("Zoom activated", false);
}

// ** SIMULATE ** //
void MyFrame::OnRun(wxCommandEvent &event)
{
  if (mmz->moncount()>0)
  {
    canvas->cycles = cyc->GetValue();
    runnetwork(canvas->cycles); 
    if (canvas->width < cyclescompleted) canvas->width = cyclescompleted;
    view_spin->SetValue(cyclescompleted);
    height_spin->SetValue(mmz->moncount());
    canvas->height = mmz->moncount();
    wxString text("Run.");
    canvas->Render(text, true);
  }
  else
  {
    wxMessageDialog error(this, "No monitor points found. \nPlease select at least one.", "Error", wxICON_INFORMATION | wxICON_ERROR);
    error.ShowModal();
  }
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
  text.Printf("Run %d cycles", event.GetPosition());
  canvas->Render(text, false);
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
    if (dmz->executedevices ()) {
      n--;
      mmz->recordsignals ();
    } else
      cout << "Error: network is oscillating" << endl;
  }
  if (ok) cyclescompleted = cyclescompleted + ncycles;
  else cyclescompleted = 0;
  canvas->Render("Cycle run", true);
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
  wxBitmap run_png("bitmaps/run-build-configure.png", wxBITMAP_TYPE_PNG);
  toolbar->AddTool(EDITOR_RUN, wxT("Compile"), run_png);
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
  // This is to execute when users have finished editting the text
}

void MyEditor::OnExit(wxCommandEvent &event)
{
  Close(true);
}
