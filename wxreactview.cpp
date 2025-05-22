#include <wxreactview.hpp>

#include <wx/sstream.h>
#include <wx/filesys.h>
#include <wx/uri.h>
#include <wx/wfstream.h>
#include "include/cef_request.h"
#include "include/cef_scheme.h"

class wxReactViewHandler : public wxWebViewHandler
{
public:
    wxReactViewHandler(const wxString &directoryMapping, const wxString &indexPath) : wxWebViewHandler("wxreactview"), m_directoryMapping(directoryMapping), m_indexPath(indexPath) {}
    virtual ~wxReactViewHandler() {}

    void StartRequest(const wxWebViewHandlerRequest &request,
                      wxSharedPtr<wxWebViewHandlerResponse> response) override
    {
        std::cerr << "StartRequest: " << request.GetRawURI() << std::endl;
        wxString uri = request.GetURI();
        std::cerr << "uri: " << uri << std::endl;
        wxURI url(uri);
        std::cerr << "scheme: " << url.GetScheme() << std::endl;
        std::cerr << "server: " << url.GetServer() << std::endl;
        std::cerr << "path: " << url.GetPath() << std::endl;
        std::cerr << "query: " << url.GetQuery() << std::endl;
        std::cerr << "fragment: " << url.GetFragment() << std::endl;
        wxFile file(m_directoryMapping + "\\" + url.GetPath());
        
        if (file.IsOpened())
        {
            wxString buffer;
            file.ReadAll(&buffer);
            wxSharedPtr<wxWebViewHandlerResponseData> data;
            response->SetHeader("Access-Control-Allow-Origin", "*");
            response->SetHeader("Access-Control-Allow-Headers", "*");
            response->SetStatus(200);
            response->SetContentType("text/html");
            response->Finish(buffer);
        }
        else
        {
            response->SetStatus(404);
            response->FinishWithError();
        }
    }

private:
    wxString m_directoryMapping;
    wxString m_indexPath;
};

wxReactView::wxReactView(wxWindow *parent,
                         wxWindowID id,
                         const wxPoint &pos,
                         const wxSize &size,
                         long style,
                         const wxString &name)
{
    m_webView.reset(wxWebView::New(parent, id, "", pos, size, wxWebViewBackendChromium, style, name));
}

void wxReactView::Initialize(const wxString &directoryMapping,
                             const wxString &indexPath)
{
    m_directoryMapping = directoryMapping;
    m_indexPath = indexPath;

    m_webView->Bind(wxEVT_WEBVIEW_CREATED, &wxReactView::OnWebViewCreated, this);
}

void wxReactView::OnWebViewCreated(wxWebViewEvent &event)
{
    wxSharedPtr<wxWebViewHandler> handler(new wxReactViewHandler(m_directoryMapping, m_indexPath));
    m_webView->ShowDevTools();
    m_webView->RegisterHandler(handler);
    m_webView->LoadURL("wxreactview:index.html");
}
