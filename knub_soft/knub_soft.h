#ifndef __KNUB_SOFT_H__
#define __KNUB_SOFT_H__

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/tglbtn.h>
#include <stdint.h>
#include <iostream>
#include <fstream>

#ifdef LINUX
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "lo/lo.h"


enum { 
  ID_NOTEBOOK = 10,
  ID_MENU = 10000,
  ID_MAJ = 11,
  ID_BOOK = 12,
  ID_BUTTON = 13,
  ID_VALID_BUTTON_IP = 14,
  ID_BACKUP = 15
};

enum {
  BACKUP_REQUEST = 0x72,
  BACKUP_VALUE = 0x73
};


/*************** NOTEBOOK***********************/

/*class MyNotebook: public wxNotebook
{
public: 
  MyNotebook( wxWindow *parent, wxWindowID id);
  //  ~MyNotebook();
  wxWindow *tabWindow1, *tabWindow2;
  wxFlexGridSizer *grid1, *grid2; 
  void presetPage();

private:
  void OnTabChange(wxBookCtrlEvent& event);
  DECLARE_EVENT_TABLE()

};

BEGIN_EVENT_TABLE(MyNotebook, wxNotebook)
 EVT_NOTEBOOK_PAGE_CHANGED(ID_NOTEBOOK, MyNotebook::OnTabChange)
END_EVENT_TABLE()
*/


 /****************MAIN FRAME*******************/

class MyFrame: public wxFrame
{
 public: 
  MyFrame(wxWindow *parent, wxWindowID id, const wxString &title, 
		 const wxPoint& pos = wxDefaultPosition,
		 const wxSize& size = wxDefaultSize,
		 long style = wxDEFAULT_FRAME_STYLE );
  wxBookCtrl *book;
  
  
 private: 
  void OnAbout(wxCommandEvent &event);
  void OnQuit(wxCommandEvent &event);
  void OnMaj(wxCommandEvent &event);
  void OnBackup(wxCommandEvent &event);
  void OnButton(wxCommandEvent &event);
  void OnIdle(wxIdleEvent &event);
  
  DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
 EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
 EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
 EVT_MENU(ID_MAJ, MyFrame::OnMaj)
 EVT_MENU(ID_BACKUP, MyFrame::OnBackup)
 EVT_BUTTON(ID_BUTTON, MyFrame::OnButton)
 EVT_IDLE(MyFrame::OnIdle)
END_EVENT_TABLE()


/***********************INIT FRAME ***************/

class InitFrame: public wxFrame
{
 public: 
  InitFrame(wxWindow *parent, wxWindowID id, const wxString &title, 
		 const wxPoint& pos = wxDefaultPosition,
		 const wxSize& size = wxDefaultSize,
		 long style = wxDEFAULT_FRAME_STYLE );

  wxTextCtrl *ipTxtCtrl;
  wxButton *validButton;

  void OnButton(wxCommandEvent &event);

 private:
  DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(InitFrame, wxFrame)
 EVT_BUTTON(ID_VALID_BUTTON_IP, InitFrame::OnButton)
END_EVENT_TABLE()

/***********************OSC**********************/

class OSCmanager{

 public:
  OSCmanager();
  lo_address address;
  lo_server_thread serverThread;
  char* ip_address;
  bool isInit; 

 private:
}*myOSCmanager;

class MyApp: public wxApp
{
public :
  MyApp();
  virtual bool OnInit();
  /*MyFrame *myFrame;*/

 private: 

};

#endif
