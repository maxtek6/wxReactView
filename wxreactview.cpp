#include <wxreactview.hpp>

#include <wx/sstream.h>
#include <wx/filesys.h>

class wxReactViewHandler : public wxWebViewHandler
{
public:
    wxReactViewHandler(const wxString &directoryMapping, const wxString &indexPath) : wxWebViewHandler("wxreactview"), m_directoryMapping(directoryMapping), m_indexPath(indexPath) {}
    virtual ~wxReactViewHandler() {}
    wxFSFile* GetFile(const wxString &uri) override
    {
        const wxString html = "<html><body><h1>Hello, wxReactView!</h1></body></html>";
        return new wxFSFile(new wxStringInputStream(html), uri, wxString("text/html"),"", wxDateTime::Now());
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
    m_webView.reset(dynamic_cast<wxWebViewChromium *>(wxWebView::New(parent, id, "", pos, size, wxWebViewBackendChromium, style, name)));
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
    m_webView->RegisterHandler(handler);
    m_webView->LoadURL(wxString::Format("wxreactview://%s", m_indexPath));
}
