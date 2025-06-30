/*
 * Copyright (c) 2025 Maxtek Consulting
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <wxreactview.hpp>
#include <hyperpage.hpp>

#include <unordered_map>

#include <wx/uri.h>
#include <wx/mstream.h>

#ifndef __WXMSW__
#include <wx/webview_chromium.h>
#include "include/cef_app.h"
#include "include/cef_client.h"
class ReactCefApp : public CefApp, public CefBrowserProcessHandler
{
public:
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override
    {
        return this;
    }
    IMPLEMENT_REFCOUNTING(ReactCefApp);
};
#endif

static wxString normalizePath(const wxString &path)
{
    wxString normalizedPath = path;
    if (!normalizedPath.StartsWith("/"))
    {
        normalizedPath.Prepend("/");
    }
    return normalizedPath;
}

class HyperpageResponseData : public wxWebViewHandlerResponseData
{
public:
    HyperpageResponseData(const wxSharedPtr<hyperpage::page> &page)
        : m_stream(new wxMemoryInputStream(page->get_content(), page->get_length()))
    {
    }

    virtual ~HyperpageResponseData() = default;

    wxInputStream *GetStream() override
    {
        return m_stream.get();
    }

private:
    std::unique_ptr<wxMemoryInputStream> m_stream;
};

class HyperpageContent : public wxReactContent
{
public:
    HyperpageContent(std::unique_ptr<hyperpage::page> &page) : m_page(page.release())
    {
        m_responseData = wxSharedPtr<wxWebViewHandlerResponseData>(new HyperpageResponseData(m_page));
    }

    virtual ~HyperpageContent() = default;

    wxString GetContentType() const override
    {
        return wxString::FromUTF8(m_page->get_mime_type());
    }

    wxSharedPtr<wxWebViewHandlerResponseData> GetContent() override
    {
        return m_responseData;
    }

private:
    wxSharedPtr<hyperpage::page> m_page;
    wxSharedPtr<wxWebViewHandlerResponseData> m_responseData;
};

class HyperpageArchive : public wxReactArchive
{
public:
    HyperpageArchive(const wxString &dbPath) : m_reader(dbPath.ToStdString())
    {
    }

    wxSharedPtr<wxReactContent> LoadContent(const wxString &path) override
    {
        std::unique_ptr<hyperpage::page> page = m_reader.load(path.ToStdString());
        wxSharedPtr<wxReactContent> content;
        if (page)
        {
            return wxSharedPtr<wxReactContent>(new HyperpageContent(page));
        }
        return content;
    }

private:
    hyperpage::reader m_reader;
};

wxSharedPtr<wxReactArchive> wxReactArchive::CreateHyperpageArchive(const wxString &dbPath)
{
    return wxSharedPtr<wxReactArchive>(new HyperpageArchive(dbPath));
}

class wxReactHandler : public wxWebViewHandler
{
public:
    wxReactHandler(const wxSharedPtr<wxReactArchive> &archive);
    virtual ~wxReactHandler() = default;
    void StartRequest(const wxWebViewHandlerRequest &request,
                      wxSharedPtr<wxWebViewHandlerResponse> response) override final;
    void RegisterEndpoint(const wxString &endpoint, const wxRequestHandler &handler);
private:
    wxSharedPtr<wxReactArchive> m_archive;
    std::unordered_map<wxString, wxRequestHandler> m_requestHandlers;
};

wxReactHandler::wxReactHandler(const wxSharedPtr<wxReactArchive> &archive)
    : wxWebViewHandler("https"), m_archive(archive)
{
    SetVirtualHost("wxreactview.ipc");
}

void wxReactHandler::StartRequest(const wxWebViewHandlerRequest &request,
                                  wxSharedPtr<wxWebViewHandlerResponse> response)
{
    const wxURI wxUri(request.GetURI());
    const wxString &path = normalizePath(wxUri.GetPath());
    if (path.StartsWith("/api/"))
    {
        auto it = m_requestHandlers.find(path);
        if (it != m_requestHandlers.end())
        {
            it->second(request, response);
        }
        else
        {
            response->SetStatus(404);
            response->FinishWithError();
        }
    }
    else
    {
        auto content = m_archive->LoadContent(path);
        if (content)
        {
            response->SetStatus(200);
            response->SetContentType(content->GetContentType());
            response->Finish(content->GetContent());
        }
        else
        {
            response->SetStatus(404);
            response->FinishWithError();
        }
    }
}

void wxReactHandler::RegisterEndpoint(const wxString &endpoint, const wxRequestHandler &handler)
{
    wxString key("/api" + endpoint);
    m_requestHandlers[key] = handler;
}

bool wxReactApp::StartProcess()
{
    bool result;
    #ifdef __WXMSW__
    result = true;
    #else
    CefMainArgs mainArgs;
    CefRefPtr<ReactCefApp> app = new ReactCefApp;
    result = CefExecuteProcess(mainArgs, app, nullptr);
    #endif
    return result;
}

bool wxReactApp::OnInit()
{
    return StartProcess() && OnReady();
}

wxReactView::wxReactView(const wxSharedPtr<wxReactArchive>& archive, const wxString &indexPage)
    : m_archive(archive), m_indexPage(indexPage)
{
    m_handler = wxSharedPtr<wxWebViewHandler>(new wxReactHandler(m_archive));
}

void wxReactView::BindWebView(wxWebView *webView)
{
#ifdef __WXMSW__
    // for some reason, Edge needs to catch this event to pass to StartRequest
    webView->Bind(wxEVT_WEBVIEW_NAVIGATING, [&](wxWebViewEvent &event) {
        const wxString url = event.GetURL();
        if (!url.StartsWith("https://wxreactview.ipc"))
        {
            event.Skip();
        }
    });
#endif
    webView->Bind(wxEVT_WEBVIEW_CREATED, [&](wxWebViewEvent &event) {
        wxWebView *object = reinterpret_cast<wxWebView*>(event.GetEventObject());
        object->RegisterHandler(m_handler);
        object->LoadURL("https:" + m_indexPage);
    });
}

void wxReactView::RegisterEndpoint(const wxString &endpoint, const wxRequestHandler &handler)
{
    if (m_handler)
    {
        static_cast<wxReactHandler*>(m_handler.get())->RegisterEndpoint(endpoint, handler);
    }
}
