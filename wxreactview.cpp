#include <wxreactview.hpp>
#ifdef __WXMSW__
#include <wx/msw/webview_edge.h>
#include <WebView2.h>
#else

#endif
wxReactView *wxReactView::New(wxWindow *parent,
    wxWindowID id,
    wxReactController *controller,
    const wxString &url,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name)
{
    std::unique_ptr<wxReactView> reactView(reinterpret_cast<wxReactView*>(wxWebView::New(parent, id, url, pos, size, wxReactView::GetWebviewBackend(), style, name)));
    reactView->Initialize(controller);
    return reactView.release();
}

void wxReactView::Initialize(wxReactController *controller)
{
    m_reactController = controller;
    Bind(wxEVT_WEBVIEW_CREATED, &wxReactView::OnCreated, this);
}

void wxReactView::OnCreated(wxWebViewEvent &event)
{
    auto *edgeView = dynamic_cast<wxWebViewEdge*>(this);
    auto *webViewHandle = static_cast<ICoreWebView2_3*>(edgeView->GetNativeBackend());
    if (webViewHandle)
    {
        HRESULT hr = webViewHandle->SetVirtualHostNameToFolderMapping(
            L"wxreact", 
            L"webview", 
            COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);
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