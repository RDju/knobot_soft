#include "wx/wxprec.h"
#include "knub_soft.h"

using namespace std;

/*************************************************************************\
*                                                                         *
*                              NOTEBOOK                                   *
*                                                                         *
\*************************************************************************/

/*MyNotebook::MyNotebook( wxWindow *parent, wxWindowID id): 
  wxNotebook( parent, id)
{
  
}*/

 //void MyNotebook::presetPage(){
  //Envoit demande de paramètres via OSC
 // wxButton *back = new wxButton(tabWindow1, -1, _("<"), wxPoint(20,20), wxDefaultSize/*wxSize(30,30)*/);
//wxStaticText *myStaticText = new wxStaticText(tabWindow1, -1, _("1"), wxPoint(60,20), wxDefaultSize/*wxSize(40,30)*/);
//wxButton *next = new wxButton(tabWindow1, -1, _(">"), wxPoint(100,20), wxDefaultSize/*wxSize(30,30)*/);
  //  wxTextCtrl *myControledText = new wxTextCtrl(tabWindow1, -1, _("balbla"), wxPoint(100,20), wxSize(30, 30));

 //}

/*****************************EVENTS**************************************/

/*void MyNotebook::OnTabChange(wxBookCtrlEvent& event){
  lo_send(myOSCmanager->address, "/INIT", "i", 1);
}*/



/*************************************************************************\
*                                                                         *
*                             INIT FRAME                                  *
*                                                                         *
\*************************************************************************/


InitFrame::InitFrame( wxWindow *parent, wxWindowID id, const wxString &title,
		  const wxPoint &position, const wxSize& size, long style ) :
  wxFrame( parent, id, title, position, size, style )
{
  ipTxtCtrl = new wxTextCtrl(this, -1, _(""), wxPoint(20,20), wxSize(300, 30));
  validButton = new wxButton(this, ID_VALID_BUTTON_IP, _("OK"), wxPoint(20, 50), wxDefaultSize);

}


/********************************Events***********************************/
void InitFrame::OnButton(wxCommandEvent &event){
  //vérifie si le contenu du ctrlText est valide
  //lance la phase d'initialisation
  //lance la fenêtre principale
  
  cout << ipTxtCtrl->GetValue().mb_str(wxConvUTF8) << endl;
  
  myOSCmanager->address = lo_address_new(ipTxtCtrl->GetValue().mb_str(wxConvUTF8)/*"192.168.144.22"*/, "9000");
  while (!(myOSCmanager->isInit)){
    lo_send(myOSCmanager->address, "/INIT", "i", 1);
    sleep(1);
  }

  MyFrame *frame = new MyFrame(NULL, -1, _("Knub soft"), wxPoint(20, 20), wxSize(400, 400) );
  frame->Show(true);
  this->Show(false);
}

/*************************************************************************\
*                                                                         *
*                             MAIN FRAME                                  *
*                                                                         *
\*************************************************************************/


MyFrame::MyFrame( wxWindow *parent, wxWindowID id, const wxString &title,
		  const wxPoint &position, const wxSize& size, long style ) :
  wxFrame( parent, id, title, position, size, style )
{

  //SetIcon(wxICON(sample));

  wxMenuBar *menubar = new wxMenuBar;
  wxMenu *menu = new wxMenu;
  menu->Append(ID_MAJ, _("MAJ"), _(""));
  menu->Append(wxID_ABOUT, _("About"), _(""));
  menu->Append(wxID_EXIT, _("Quit"), _(""));
  menubar->Append(menu, _("File"));
  SetMenuBar(menubar);

  book = new wxBookCtrl(this, ID_BOOK);
  wxPanel *panel = new wxPanel(book);
  new wxButton(panel, ID_BUTTON, _("button"));
  book->AddPage(panel, _("Presets"), true);

  panel = new wxPanel(book);
  
  book->AddPage(panel, _("prefs"));
  
}


/****************************Events***************************************/

void MyFrame::OnIdle(wxIdleEvent &event){
  //rien en fait -> utile ?
}

void MyFrame::OnAbout( wxCommandEvent &event){
  wxMessageDialog dialog(this, _("The Knobot\n by Combosquare"), 
			 _("About the Knobot"), wxOK|wxICON_INFORMATION);
  dialog.ShowModal();
}

void MyFrame::OnMaj(wxCommandEvent &event){
  system("./avrdude.sh");
  cout << "BIM" << endl;
}

 void MyFrame::OnButton(wxCommandEvent &event){
   wxMessageBox(_("click"), _("click"), wxOK | wxICON_INFORMATION, this); 
 }

void MyFrame::OnQuit(wxCommandEvent &event){
  Close(true);
}

/*************************************************************************\
*                                                                         *
*                                 OSC                                     *
*                                                                         *
\*************************************************************************/

void error(int num, const char *msg, const char *path){
  cout << "ERROR : bouuuuh\n" << endl;
  fflush(stdout);
}

int generic_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
{
  cout << "youpi" << endl;
  return 1;
}

int banane_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data){
  cout << "banana !" << endl;
  return 1;
}

int init_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data){
  myOSCmanager->isInit = true;
  return 1;
}


OSCmanager::OSCmanager(){

  serverThread = lo_server_thread_new("9002", error);
  lo_server_thread_add_method(serverThread, NULL, NULL, generic_handler, NULL);
  lo_server_thread_add_method(serverThread, "/BANANE", "i", banane_handler, NULL);
  lo_server_thread_add_method(serverThread, "/INIT", "i", init_handler, NULL);
  
  lo_server_thread_start(serverThread);

  /*#ifdef LINUX

  struct ifaddrs * ifAddrStruct=NULL;
  struct ifaddrs * ifa = NULL;
  void *tmpAddrPtr=NULL;

  getifaddrs(&ifAddrStruct);

  for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr->sa_family == AF_INET && !strcmp(ifa->ifa_name, "eth1")) {
      tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
      char addressBuffer[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
      ip_address=addressBuffer;
    }
  }

  printf(ip_address);
  if (ifAddrStruct != NULL) freeifaddrs(ifAddrStruct);

  #else
    printf("foiré");
    #endif*/
  isInit = false;
}



/*************************************************************************\
*                                                                         *
*                                APP                                      *
*                                                                         *
\*************************************************************************/


IMPLEMENT_APP(MyApp)

MyApp::MyApp()
{
}

bool MyApp::OnInit(){

  InitFrame *ipFrame = new InitFrame(NULL, -1, _("ip"), wxPoint(20, 20), wxSize(400,400));
  //wxTextCtrl *IpTxtCtrl = new wxTextCtrl(ipFrame, -1, _("Arduino IP"));

  ipFrame->Show(true);
   
  myOSCmanager = new OSCmanager();
  
  

   //frame->SetBackgroundColour(* wxRED);
   //   frame->Show(true);

 
  /*MyNotebook *noteBook = new MyNotebook(frame, ID_NOTEBOOK);
  noteBook->tabWindow1 = new wxWindow(noteBook, wxID_ANY);
  noteBook->tabWindow2 = new wxWindow(noteBook, wxID_ANY);
  //  wxWindow* window3 = new wxWindow(noteBook, wxID_ANY);
  //  wxWindow* window4 = new wxWindow(noteBook, wxID_ANY);
  
  noteBook->AddPage(noteBook->tabWindow1, _("Presets"));
  noteBook->AddPage(noteBook->tabWindow2, _("Preferences"));
  
  //    wxButton *btn=new wxButton(noteBook->tabWindow1, wxID_ANY, _T("Cliquez-moi !"));
  noteBook->presetPage();*/

  //frame->Show(true);
  
   


  return true;
}

