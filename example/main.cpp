#include <wxreactview.hpp>
#include <wx/filename.h>
#include <wx/stdpaths.h>

class JsonReactViewHandler : public wxReactViewHandler
{
public:
    JsonReactViewHandler() = default;
    virtual ~JsonReactViewHandler() override = default;
    virtual void HandleMessage(wxReactView *source, const wxString &message) override
    {
        
    }
};

class wxReactFrame : public wxFrame
{
public:
    wxReactFrame(JsonReactViewHandler* hander, const wxString &title,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL) : wxFrame(nullptr, wxID_ANY, title, pos, size, style)
    {
        wxString exePath = wxFileName(wxStandardPaths::Get().GetExecutablePath()).GetPath();
        exePath.Append("\\dist");
        m_webView = wxReactView::NewWebView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, "wxReactView");
        m_reactView = std::make_unique<wxReactView>(m_webView, exePath);
        m_reactView->RegisterHandler(hander);
    }
    virtual ~wxReactFrame() override = default;
private:
    wxWebView *m_webView = nullptr;
    std::unique_ptr<wxReactView> m_reactView = nullptr;
};

class wxReactApp : public wxApp
{
public:
    virtual bool OnInit() override
    {
        wxInitialize();
        m_reactViewHandler = std::make_unique<JsonReactViewHandler>();   
        m_reactFrame = std::make_unique<wxReactFrame>(m_reactViewHandler.get(), "wxReactView", wxDefaultPosition, wxSize(800, 600));
        m_reactFrame->Show(true);
        return true;
    }
    virtual int OnExit() override
    {
        wxUninitialize();
        return 0;
    }
private:
    std::unique_ptr<wxReactFrame> m_reactFrame = nullptr;
    std::unique_ptr<JsonReactViewHandler> m_reactViewHandler = nullptr;
};

wxIMPLEMENT_APP_CONSOLE(wxReactApp);