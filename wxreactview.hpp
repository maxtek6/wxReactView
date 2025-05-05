#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/webview.h>
#include <wx/webview_chromium.h>

class wxReactViewHandler;

class wxReactView
{
public:
    static wxWebView *NewWebView(wxWindow *parent,
                            wxWindowID id,
                            const wxPoint &pos = wxDefaultPosition,
                            const wxSize &size = wxDefaultSize,
                            long style = 0,
                            const wxString &name = wxASCII_STR(wxWebViewNameStr));

    
    wxReactView(wxWebView *webView, 
                const wxString& directoryMapping, 
                const wxString& indexPath = "index.html");
    
    void RegisterHandler(wxReactViewHandler* handler);

    void Send(const wxString& message);

private:
    static wxString GetWebviewBackend();
    void OnWebViewCreated(wxWebViewEvent& event);
    void OnWebViewScriptMessageReceived(wxWebViewEvent& event);
    wxWebView* m_webView;
    wxString m_directoryMapping;
    wxString m_indexPath;
    wxReactViewHandler* m_handler = nullptr;
};

class wxReactViewHandler
{
public:
    virtual ~wxReactViewHandler() = default;
    virtual void HandleMessage(wxReactView *source, const wxString& message) = 0;
};