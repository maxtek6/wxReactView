#include <wxreactview.hpp>
#ifdef __WXMSW__
#include <wx/msw/webview_edge.h>
#include <WebView2.h>
#else

#endif

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
    backend = wxWebViewBackendEdge;
#else
    backend = wxWebViewBackendWebKit;
#endif
    return wxString(backend, wxConvUTF8);
}

void wxReactView::OnWebViewCreated(wxWebViewEvent &event)
{
    const wxString virtual_host("wxreactview.runtime");
    const wxString url = wxString::Format("https://%s/%s", virtual_host, m_indexPath);
    ICoreWebView2_3 * const handle = static_cast<ICoreWebView2_3 *>(m_webView->GetNativeBackend());
    if(handle)
    {
        HRESULT hr = handle->SetVirtualHostNameToFolderMapping(
            virtual_host.wc_str(),
            m_directoryMapping.wc_str(),
            COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);
        if(SUCCEEDED(hr))
        {
            m_webView->LoadURL(url);
            m_webView->AddScriptMessageHandler("wxreactviewipc");
            m_webView->Bind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED,
                &wxReactView::OnWebViewScriptMessageReceived, this);
        }
    }    
}

void wxReactView::OnWebViewScriptMessageReceived(wxWebViewEvent &event)
{
    const wxString message = event.GetString();
    if (m_handler)
    {
        m_handler->HandleMessage(this, message);
    }
}