#include <wxreactview.hpp>

wxWebView *wxReactView::NewWebView(wxWindow *parent,
                                   wxWindowID id,
                                   const wxPoint &pos,
                                   const wxSize &size,
                                   long style,
                                   const wxString &name)
{
    return wxWebView::New(parent, id, "", pos, size, wxReactView::GetWebviewBackend(), style, name);
}

wxReactView::wxReactView(wxWebView *webView,
                         const wxString &directoryMapping,
                         const wxString &indexPath)
    : m_webView(webView), m_directoryMapping(directoryMapping), m_indexPath(indexPath)
{
    m_webView->Bind(wxEVT_WEBVIEW_CREATED, &wxReactView::OnWebViewCreated, this);
}

void wxReactView::RegisterHandler(wxReactViewHandler *handler)
{
    m_handler = handler;
}

void wxReactView::Send(const wxString &message)
{
    if (m_handler)
    {
        m_handler->HandleMessage(this, message);
    }
}

wxString wxReactView::GetWebviewBackend()
{
    const char *backend;
#ifdef __WXMSW__
    backend = wxWebViewBackendChromium;
#else
    backend = wxWebViewBackendWebKit;
#endif
    return wxString(backend, wxConvUTF8);
}

void wxReactView::OnWebViewCreated(wxWebViewEvent &event)
{
    if(!m_webView)
    {
        wxLogError(wxString::Format("Failed to create webview: %s", event.GetString()));
    }
    if(!m_webView->GetNativeBackend())
    {
        wxLogError(wxString::Format("Failed to get native backend: %s", event.GetString()));
        return;
    }
    reinterpret_cast<wxWebViewChromium*>(m_webView->GetNativeBackend())->SetRoot(wxFileName(m_directoryMapping));
    m_webView->LoadURL("file://index.html");
}

void wxReactView::OnWebViewScriptMessageReceived(wxWebViewEvent &event)
{
    const wxString message = event.GetString();
    if (m_handler)
    {
        m_handler->HandleMessage(this, message);
    }
}