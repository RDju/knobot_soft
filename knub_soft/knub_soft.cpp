#include "wx/wxprec.h"
#include "knub_soft.h"
#include "serial.h"

using namespace std;
 
Serial port;
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
  ipTxtCtrl = new wxTextCtrl(this, -1, _("192.168.144.22"), wxPoint(20,20), wxSize(200, 30));
  validButton = new wxButton(this, ID_VALID_BUTTON_IP, _("OK"), wxPoint(20, 50), wxDefaultSize);

}


/********************************Events***********************************/
void InitFrame::OnButton(wxCommandEvent &event){
  //TODO: vérifie si le contenu du ctrlText est valide
  
  myOSCmanager->address = lo_address_new(ipTxtCtrl->GetValue().mb_str(wxConvUTF8), "9000");
  
  uint8_t counter = 0;
  while (!(myOSCmanager->isInit) && counter < 10){
    lo_send(myOSCmanager->address, "/INIT", "i", 1);
    sleep(1);
    counter++;
  }

  if( myOSCmanager->isInit ){
    MyFrame *frame = new MyFrame(NULL, -1, _("Knub soft"), wxPoint(20, 20), wxSize(400, 400) );
    frame->Show(true);
    this->Show(false);
  } else {
      wxMessageDialog dialog(this, _("Bad IP address"), 
			 _("Warning"), wxOK|wxICON_EXCLAMATION);
      dialog.ShowModal();
  }
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
  menu->Append(ID_BACKUP, _("Backup"), _(""));
  menu->Append(wxID_ABOUT, _("About"), _(""));
  menu->Append(wxID_EXIT, _("Quit"), _(""));
  menubar->Append(menu, _("File"));
  SetMenuBar(menubar);
  CreateStatusBar(1);

  book = new wxBookCtrl(this, ID_BOOK);
  wxPanel *panel = new wxPanel(book);
  new wxButton(panel, ID_BUTTON, _("button"));
  book->AddPage(panel, _("Presets"), true);

  panel = new wxPanel(book);
  
  book->AddPage(panel, _("Prefs"));
  
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
  
  wxFrame *majFrame = new wxFrame(this, -1, _("Select port"), wxDefaultPosition, wxSize(200, 65));
  wxArrayString portList = port.port_list();//TODO: none
  wxChoice *listbox = new wxChoice(majFrame, -1, wxPoint(10, 5), wxSize(150, 30), portList, wxLB_SORT |wxLB_EXTENDED);
  majFrame->Show(true);
  
  wxString name = listbox->GetString(listbox->GetSelection());
  
  majFrame->Show(false);
  
  SetStatusText(_("Uploading programm..."));

  ofstream fichier("avrdude.sh", ios::out | ios::trunc);
  if (!fichier.fail()){
    fichier << "PORT=";
    //    fichier << "/dev/ttyACM0" << endl;
    fichier << name.mb_str() << endl;
    #ifdef LINUX || MACOSX
    fichier << "avrdude -c stk500v2 -p atmega2560 -P $PORT -b 115200 -D -U flash:w:knub_servo.cpp.hex 2> avrdude.log";
#elif defined(WINDOWS)
    fichier << "..\avr\bin\avrdude -CD:..\avr\etc\avrdude.conf -patmega2560 -cwiring -P$PORT -b115200 -D -Uflash:w:knub_servo.cpp.hex:i";
    #else
    cout << "ratéééé" << endl;
#endif
    
    /*  fichier.seekp(5, ios::beg);
	fichier<<"ceci est un test";*/
    fichier.close();
  }
  
  //TODO: sélection du port
  system("./avrdude.sh");
  //TODO: Vérifier le bon déroulement dans avrdude.log
  //erreur si pas branché, ne répond pas, fichier non trouvé
  SetStatusText(_("Upload Done"));
  cout << "BIM" << endl;
}

void MyFrame::OnBackup(wxCommandEvent &event){
  //TODO: on récupère toutes les données de l'eeprom externe via port série
  wxFrame *backupFrame = new wxFrame(this, -1, _("BACKUP"), wxDefaultPosition, wxSize(200, 65));
  wxArrayString portList = port.port_list();//TODO: none
  wxChoice *listbox = new wxChoice(backupFrame, -1, wxPoint(10, 5), wxSize(150, 30), portList, wxLB_SORT |wxLB_EXTENDED);
  wxButton *validButton = new wxButton(backupFrame, -1, _("OK"),wxPoint(10, 35), wxSize(50,30));

  backupFrame->Show(true);

  //  wxString name = listbox->GetCurrentSelection();

  port.Close();
  port.Open(listbox->GetString(listbox->GetSelection()));
  port.Set_baud(57600);
  if(port.Is_open()){
    cout << "port open" << endl;
    uint8_t buf[1024];
    buf[0] = BACKUP_REQUEST;
    port.Write(buf, 1);
    
    int r = port.Input_wait(40);
    if(r>0){
      r = port.Read(buf, sizeof(buf));
      if(r<=0){
	//continue;
      } else {
	uint8_t msn = *buf & 0xF0;
	if ( msn == BACKUP_VALUE){
	  //TODO: récup les infos
	}  
      }


    }
  } else {
    cout << "error opening port" << endl;
  }
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
  cout << "ERROR" << endl;
  fflush(stdout);
}

int generic_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
{
  return 1;
}

int init_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data){
  myOSCmanager->isInit = true;
  return 1;
}


OSCmanager::OSCmanager(){

  serverThread = lo_server_thread_new("9002", error);
  lo_server_thread_add_method(serverThread, NULL, NULL, generic_handler, NULL);
  lo_server_thread_add_method(serverThread, "/INIT", "i", init_handler, NULL);
  
  lo_server_thread_start(serverThread);

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

  InitFrame *ipFrame = new InitFrame(NULL, -1, _("ip"), wxPoint(20, 20), wxSize(300,100));
  ipFrame->Show(true);
   
  myOSCmanager = new OSCmanager();
 
  return true;
}
