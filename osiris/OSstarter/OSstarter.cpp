#include <stdlib.h>
#include <wx/app.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <nwx/nwxFileUtil.h>

class starterApp : public wxApp
{
public:
  virtual bool OnInit();
  void run_main();
};

class starterFrame: public wxFrame
{
public:
  starterFrame(starterApp *parent):
    wxFrame(NULL, wxID_ANY, "OSIRIS", wxPoint(50,50), wxSize(200,200)),
    m_parent(parent)
    {
      wxMenu *pMenu = new wxMenu();
      pMenu->Append(wxID_EXIT);
      wxMenuBar *pMenuBar = new wxMenuBar();
      pMenuBar->Append(pMenu,"File");
      SetMenuBar(pMenuBar);
    }
  virtual ~starterFrame()
    {}
  void OnActivate(wxActivateEvent &e);
private:
  starterApp *m_parent;
  DECLARE_EVENT_TABLE()
};


void starterFrame::OnActivate(wxActivateEvent &e)
{
  if(e.GetActive())
  {
    m_parent->run_main();
  }
}

void starterApp::run_main()
{
  wxStandardPathsBase &sp(wxStandardPaths::Get());
  wxFileName fn(sp.GetExecutablePath());
  wxString s = fn.GetPath(
    wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR, 
    wxPATH_NATIVE);
  wxString sExePath = nwxFileUtil::GetRealPath(s);
  nwxFileUtil::EndWithSeparator(&sExePath);
  sExePath.Append(wxS("osirisAnalysis"));

  if(wxFileName::IsFileExecutable(sExePath))
  {
    int myArgc = argc;
    if(myArgc > 127)
    {
      myArgc = 127;
    }
    char *myArgv[128];
    const char *pStr;
    char *pStrCopy;
    myArgv[myArgc] = NULL;
    for(int i = 0; i < myArgc; ++i)
    {
      s = argv[i];
      pStr = s.utf8_str();
      pStrCopy = (char *)malloc(strlen(pStr) + 1);
      strcpy(pStrCopy, pStr);
      myArgv[i] = pStrCopy;
    }
    execvp(sExePath.utf8_str(), myArgv);
  }
}


BEGIN_EVENT_TABLE(starterFrame, wxFrame)
EVT_ACTIVATE(starterFrame::OnActivate)
END_EVENT_TABLE()

bool starterApp::OnInit()
{
  bool r = wxApp::OnInit();
  if(r)
  {
    starterFrame *pFrame = new starterFrame(this);
    pFrame->Show();
  }
  return r;
}

IMPLEMENT_APP(starterApp)
