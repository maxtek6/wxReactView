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

#ifndef WXREACTVIEW_HPP
#define WXREACTVIEW_HPP

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/webview.h>

#include <condition_variable>
#include <shared_mutex>

using wxRequestHandler = std::function<void(const wxWebViewHandlerRequest &, wxSharedPtr<wxWebViewHandlerResponse>)>;

class wxReactContent
{
public:
    virtual ~wxReactContent() = default;
    virtual wxString GetContentType() const = 0;
    virtual wxSharedPtr<wxWebViewHandlerResponseData> GetContent() = 0;
};

class wxReactArchive
{
public:
    static wxSharedPtr<wxReactArchive> CreateHyperpageArchive(const wxString &dbPath);
    virtual wxSharedPtr<wxReactContent> LoadContent(const wxString &path) = 0;
};

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

class wxReactApp : public wxApp
{
public:
    virtual bool OnInit() override final;
    virtual bool OnReady() = 0;

private:
    static bool StartProcess();
};

class wxReactView
{
public:
    wxReactView(wxWindow * parent, wxWindowID id,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = 0,
                 const wxString &name = wxASCII_STR(wxWebViewNameStr));

    void Initialize(
        wxReactHandler *handler, 
        const std::function<void(wxWebView*)>& onCreate = std::function<void(wxWebView*)>(),
        const wxString &indexPage = "index.html");

    wxWebView* GetWebView() const;

private:
    mutable bool m_ready;
    mutable std::shared_timed_mutex m_mutex;
    mutable std::condition_variable_any m_readyCondition;
    std::unique_ptr<wxWebView> m_webView;
    wxString m_indexPage;
    wxSharedPtr<wxWebViewHandler> m_handler;
};

#endif