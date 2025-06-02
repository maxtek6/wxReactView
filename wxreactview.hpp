#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/webview.h>

class wxReactView
{
public:
    wxReactView(wxWindow *parent,
                wxWindowID id,
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize,
                long style = 0,
                const wxString &name = wxASCII_STR(wxWebViewNameStr));
    
    void Initialize(const wxString& directoryMapping);

    void Send(const wxString& message);

private:
    static wxString GetWebviewBackend();
    void OnWebViewCreated(wxWebViewEvent& event);
    void OnWebViewScriptMessageReceived(wxWebViewEvent& event);
    std::unique_ptr<wxWebView> m_webView;
    wxString m_directoryMapping;
    wxString m_indexPath;
};

class wxReactApp : public wxApp
{
public:
    virtual bool OnInit() override final;
    virtual bool OnReady() = 0;
private:
    static bool StartProcess();
};