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
    std::cerr << wxString::Format("WebView created: %s", event.GetString()) << std::endl;
    if(!m_webView)
    {
        std::cerr << wxString::Format("Failed to create webview: %s", event.GetString()) << std::endl;
    }
    std::cerr << "here" << std::endl;
    wxWebViewChromium *handle = dynamic_cast<wxWebViewChromium*>(m_webView);
    dynamic_cast<wxWebViewChromium*>(m_webView)->SetRoot(wxFileName(m_directoryMapping));
    std::cerr << wxString::Format("Set root: %s", m_directoryMapping) << std::endl;
    handle->SetPageText("<html><body><h1>Hello World</h1></body></html>");
    std::cerr << wxString::Format("Load URL: %s", m_indexPath) << std::endl;
}

void wxReactView::OnWebViewScriptMessageReceived(wxWebViewEvent &event)
{
    const wxString message = event.GetString();
    if (m_handler)
    {
        m_handler->HandleMessage(this, message);
    }
}