#include <wxreactview.hpp>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <yyjson.h>
#include <wx/webview_chromium_impl.h>
#include <wx/msw/private.h>
class MyCefApp : public CefApp, public CefBrowserProcessHandler
{
public:
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override
    {
        return this;
    }
    IMPLEMENT_REFCOUNTING(MyCefApp);
};


class wxReactFrame : public wxFrame
{
public:
    wxReactFrame(const wxString &title,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL) : wxFrame(nullptr, wxID_ANY, title, pos, size, style)
    {
        if (!wxWebView::IsBackendAvailable(wxWebViewBackendChromium))
        {
            wxLogError("wxWebView backend is not available.");
            return;
        }
        wxString directoryMapping = wxFileName(wxStandardPaths::Get().GetExecutablePath()).GetPath();
        directoryMapping.Append("\\dist");
        m_reactView = std::make_unique<wxReactView>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, "wxReactView");
        m_reactView->Initialize(directoryMapping, "index.html");
    }
    virtual ~wxReactFrame() override = default;

private:
    std::unique_ptr<wxReactView> m_reactView = nullptr;
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override
    {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        wxReactFrame *frame = new wxReactFrame("wxReactView Example",
                                               wxPoint(50, 50), wxSize(800, 600), wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
        frame->Show();

        return true;
    }

    virtual int OnExit() override
    {
        FreeConsole();
        return 0;
    }
};

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    CefMainArgs main_args(hInstance);
    CefRefPtr<MyCefApp> app = new MyCefApp();
    int exit_code = CefExecuteProcess(main_args, app, nullptr);
    if (exit_code >= 0)
    {
        return exit_code;
    }
    wxApp::SetInstance(new MyApp());
    wxInitialize();
    wxTheApp->CallOnInit();
    wxTheApp->OnRun();
    wxTheApp->OnExit();
    wxTheApp->CleanUp();
    wxUninitialize();
    return 0;
}