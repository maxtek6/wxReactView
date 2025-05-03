#include <wx/wx.h>
#include <wx/webview.h>

class wxReactController
{
public:
    wxReactController(const wxString &buildDirectory);
    virtual ~wxReactController() = default;

    wxString GetBuildDirectory() const;
    void SetBuildDirectory(const wxString &buildDirectory);

    virtual void OnMessage(wxString &message) = 0;
    void Notify(wxString &message);
private:
    wxString m_buildDirectory;
};

class wxReactView : public wxWebView
{
public:
    static wxReactView *New(wxWindow *parent,
                            wxWindowID id,
                            wxReactController *controller,
                            const wxString &url = wxASCII_STR(wxWebViewDefaultURLStr),
                            const wxPoint &pos = wxDefaultPosition,
                            const wxSize &size = wxDefaultSize,
                            long style = 0,
                            const wxString &name = wxASCII_STR(wxWebViewNameStr));
private:
    void Initialize(wxReactController *controller);
    void OnCreated(wxWebViewEvent &event);
    void OnMessage(wxWebViewEvent &event);
    static wxString GetWebviewBackend();
    wxReactController *m_reactController = nullptr;
};