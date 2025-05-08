#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/webview.h>
#include <wx/webview_chromium.h>
#include "include/cef_app.h"


class wxReactView
{
public:
    wxReactView(wxWindow *parent,
                wxWindowID id,
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize,
                long style = 0,
                const wxString &name = wxASCII_STR(wxWebViewNameStr));
    
    void Initialize(const wxString& directoryMapping,
                    const wxString& indexPath);

    void Send(const wxString& message);

private:
    static wxString GetWebviewBackend();
    void OnWebViewCreated(wxWebViewEvent& event);
    void OnWebViewScriptMessageReceived(wxWebViewEvent& event);
    std::unique_ptr<wxWebViewChromium> m_webView;
    wxString m_directoryMapping;
    wxString m_indexPath;
};

