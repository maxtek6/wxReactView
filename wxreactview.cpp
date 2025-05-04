#include <wxreactview.hpp>
#ifdef __WXMSW__
#include <wx/msw/webview_edge.h>
#include <WebView2.h>
#else

#endif
wxReactView *wxReactView::New(wxWindow *parent,
    wxWindowID id,
    const wxString& directoryMapping,
    const wxString &url,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name)
{
    std::unique_ptr<wxReactView> reactView(reinterpret_cast<wxReactView*>(wxWebView::New(parent, id, url, pos, size, wxReactView::GetWebviewBackend(), style, name)));
    reactView->Initialize(directoryMapping);
    return reactView.release();
}

void wxReactView::Initialize(const wxString& directoryMapping)
{
    m_directoryMapping = directoryMapping;
    Bind(wxEVT_WEBVIEW_CREATED, &wxReactView::OnCreated, this);
}

void wxReactView::OnCreated(wxWebViewEvent &event)
{
    auto *edgeView = dynamic_cast<wxWebViewEdge*>(this);
    auto *webViewHandle = static_cast<ICoreWebView2_3*>(edgeView->GetNativeBackend());
    if (webViewHandle)
    {
        HRESULT hr = webViewHandle->SetVirtualHostNameToFolderMapping(
            L"wxreactview.runtime", 
            m_directoryMapping.wc_str(), 
            COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);
    }
    LoadURL("https://wxreactview.runtime/index.html");
    AddScriptMessageHandler("wxreactview");
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