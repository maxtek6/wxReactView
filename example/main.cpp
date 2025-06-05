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
        
        // Create the main application frame and set its size.
        m_appFrame = std::make_unique<wxFrame>(nullptr, wxID_ANY, "wxReact Example");
        m_appFrame->SetSize(wxSize(800, 600));
        
        // Create the wxReactView and initialize a handler.
        m_reactView = std::make_unique<wxReactView>(m_appFrame.get(), wxID_ANY);
        m_reactView->Initialize(new wxReactHandler(archive));

        // Display the wxReactView in the main frame.
        m_appFrame->Show(true);
        
        return true;
    }
private:
    std::unique_ptr<wxFrame> m_appFrame;
    std::unique_ptr<wxReactView> m_reactView;
};

wxIMPLEMENT_APP(ExampleApp);