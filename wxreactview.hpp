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

/**
 * @file wxreactview.hpp
 * @brief wxReactView API header file
 * @author John R. Patek Sr.
 */

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/webview.h>

/**
 * @typedef wxRequestHandler
 *
 * @brief A function type that handles HTTP requests in a wxWebView.
 */
using wxRequestHandler = std::function<void(const wxWebViewHandlerRequest &, wxSharedPtr<wxWebViewHandlerResponse>)>;

/**
 * @class wxReactContent
 *
 * @brief abstract class representing static content in a wxReact archive.
 */
class wxReactContent
{
public:
    /**
     * @brief Get the content type of the static content.
     * @return The content MIME type as a wxString.
     */
    virtual wxString GetContentType() const = 0;

    /**
     * @brief Get the response data for static content.
     * @return Shared pointer to wxWebViewHandlerResponseData containing
     * the content.
     */
    virtual wxSharedPtr<wxWebViewHandlerResponseData> GetContent() = 0;
};

/**
 * @class wxReactArchive
 *
 * @brief abstract class representing a collection of static content.
 */
class wxReactArchive
{
public:
    /**
     * @brief create a wxReactArchive instance from a Hyperpage database.
     * @param dbPath The path to the database file.
     */
    static wxSharedPtr<wxReactArchive> CreateHyperpageArchive(const wxString &dbPath);

    /**
     * @brief load static content from a specified path.
     * @param path The path used to identify the archived content to be
     * loaded.
     */
    virtual wxSharedPtr<wxReactContent> LoadContent(const wxString &path) = 0;
};

/**
 * @class wxReactView
 *
 * @brief Pairs a wxWebView with a wxReactArchive to display static
 * content and handle HTTP requests.
 */
class wxReactView
{
public:
    /**
     * @brief Constructor for wxReactView.
     * @param archive The wxReactArchive containing static content.
     * @param indexPage The initial page to load in the web view, defaults
     * to "index.html".
     */
    wxReactView(const wxSharedPtr<wxReactArchive> &archive, const wxString &indexPage = "index.html");

    /**
     * @brief bind a wxWebView to this wxReactView.
     * @param webView The wxWebView instance to bind.
     */
    void BindWebView(wxWebView *webView);

    /**
     * @brief Register an endpoint to serve over HTTP.
     * @param endpoint The endpoint path to register.
     * @param handler The request handler function to call when the endpoint
     * is accessed.
     */
    void RegisterEndpoint(const wxString &endpoint, const wxRequestHandler &handler);

private:
    wxSharedPtr<wxReactArchive> m_archive;
    wxString m_indexPage;
    wxSharedPtr<wxWebViewHandler> m_handler;
};

/**
 * @class wxReactHandler
 *
 * @brief extends wxApp
 */
class wxReactApp : public wxApp
{
public:
    /// @private
    virtual bool OnInit() override final;
    
    /**
     * @brief Starting point for the wxReact application.
     * @return true if the application started successfully, false otherwise.
     */
    virtual bool OnReady() = 0;

private:
    /// @private
    static bool StartProcess();
};

#endif