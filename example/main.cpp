#include <wxreactview.hpp>

#include <wx/stdpaths.h>


static wxString GetArchivePath()
{
    // Get the path to the executable and append the archive name.
    wxString dbPath = wxFileName(wxStandardPaths::Get().GetExecutablePath()).GetPath();
    return wxFileName(dbPath, "hyperpage.db").GetFullPath();
}

class ExampleApp : public wxReactApp
{
public:
    virtual bool OnReady() override
    {
        // Initialize the wxReactView with a sample Hyperpage archive.
        auto archive = wxReactArchive::CreateHyperpageArchive(GetArchivePath());
        m_reactView.reset(new wxReactView(archive));

        // Create the main application frame and set its size.
        m_appFrame = std::make_unique<wxFrame>(nullptr, wxID_ANY, "wxReact Example");
        m_appFrame->SetSize(wxSize(800, 600));
        
        // Create the wxWebView and bind it.
        wxWebView *webView = wxWebView::New(m_appFrame.get(), wxID_ANY, "",
                                           wxDefaultPosition, wxDefaultSize,
                                           wxWebViewBackendEdge);
        m_reactView->BindWebView(webView);

        m_reactView->RegisterEndpoint("/EchoMessage", [&](const wxWebViewHandlerRequest &request, wxSharedPtr<wxWebViewHandlerResponse> response) {
            // Example endpoint handler that returns a simple JSON response.
            wxMessageBox(wxString::Format("Posted Message: %s", request.GetDataString().c_str()), "Message Received", wxOK | wxICON_INFORMATION, m_appFrame.get());
            response->SetStatus(200);
            response->Finish("");
        });

        // Display the wxReactView in the main frame.
        m_appFrame->Show(true);
        
        return true;
    }
private:
    std::unique_ptr<wxFrame> m_appFrame;
    std::unique_ptr<wxReactView> m_reactView;
};

wxIMPLEMENT_APP(ExampleApp);