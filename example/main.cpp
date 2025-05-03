#include <wxreactview.hpp>

class wxReactFrame : public wxFrame
{
public:
    wxReactFrame(wxWindow *parent, wxWindowID id, const wxString &title,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL)
    {

    }
    virtual ~wxReactFrame() override = default;
private:
    wxReactView *m_reactView = nullptr;
};

class wxReactApp : public wxApp
{
public:
    virtual bool OnInit() override;
    virtual int OnExit() override;
private:
    wxReactView *m_webview = nullptr;
    wxReactController *m_handler = nullptr;
};

wxIMPLEMENT_APP(wxReactApp);