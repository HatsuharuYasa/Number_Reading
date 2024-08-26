#include "gridcanvas.h"
#include "penpane.h"
#include "inference.h"

#include <wx/wx.h>
#include <iostream>
#include <vector>
#include <utility>

class MyApp: public wxApp{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

class MyFrame: public wxFrame{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
    ~MyFrame(){
        wxLog::SetActiveTarget(nullptr);
        delete logger;
    }
private:
    void ClearCanvas(wxCommandEvent &);
    void ReadNumber(wxCommandEvent &);

    void SetupPenPanes(wxWindow *parent, wxSizer *sizer);
    void SelectPenPanes(PenPane* pane);

    wxLog *logger;
    GridCanvas *gridcanvas;

    std::vector<PenPane*> penPanes{};
    const int penCount = 3;

    ModelInference model;
};

bool MyApp::OnInit(){
    MyFrame *frame = new MyFrame("Read the Number App", wxDefaultPosition, wxSize(613, 461));
    frame->Show(true);
    return true;
};

enum{
    ID_BUTTON_CLEAR = wxID_LAST + 1,
    ID_BUTTON_READ
};

void MyFrame::SetupPenPanes(wxWindow *parent, wxSizer *sizer){
    for(int i = 0; i < penCount; i++){
        auto penPane = new PenPane(parent, wxID_ANY, i * FromDIP(3) + FromDIP(10));

        penPane->Bind(wxEVT_LEFT_DOWN, [this, penPane](wxMouseEvent &e){
            this->SelectPenPanes(penPane);
        });

        penPanes.push_back(penPane);
        sizer->Add(penPane, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, FromDIP(5));
    }
}

void MyFrame::SelectPenPanes(PenPane *pane){
    for(auto penPane:penPanes){
        penPane->selected = (penPane==pane);
        penPane->Refresh();
    }
}

void MyFrame::ClearCanvas(wxCommandEvent &e){
    gridcanvas->ClearCanvas();
}

void MyFrame::ReadNumber(wxCommandEvent &e){
    std::pair<int, int> gs = gridcanvas->GetSize();
    int pred = model.RunInference(gridcanvas->GetCanvas(), gs.first, gs.second);
    wxLogMessage(L"That is number %d", pred);
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size) 
: wxFrame(nullptr, wxID_ANY, title, pos, size, wxCAPTION | wxCLOSE_BOX | wxSYSTEM_MENU | wxMINIMIZE_BOX){
    logger = new wxLogWindow(this, "WxLog", true, false);
    wxLog::SetActiveTarget(logger);

    //Panel configuration
    wxPanel *control_panel = new wxPanel(this, wxID_ANY, 
                            wxDefaultPosition, wxDefaultSize);
    control_panel->SetBackgroundColour(*wxWHITE);

    wxPanel *canvas_panel = new wxPanel(this, wxID_ANY, 
                            wxDefaultPosition, wxDefaultSize);
    canvas_panel->SetBackgroundColour(wxColor(211, 211, 211));
    
    gridcanvas = new GridCanvas(canvas_panel, wxID_ANY, 
                            wxDefaultPosition, wxSize(400, 400),
                            std::make_pair(28, 28));

    //Sizer initialization
    auto sizer_main = new wxBoxSizer(wxHORIZONTAL);
    auto sizer_control_panel = new wxBoxSizer(wxVERTICAL);
    auto sizer_pen_panel = new wxBoxSizer(wxHORIZONTAL);
    auto sizer_canvas = new wxBoxSizer(wxHORIZONTAL);

    //Item configuration
    SetupPenPanes(control_panel, sizer_pen_panel);

    auto text_brush_size = new wxStaticText(control_panel, wxID_ANY, "Brush Size");

    auto button_clear = new wxButton(control_panel, ID_BUTTON_CLEAR, "Clear", wxDefaultPosition, wxSize(120, 40));
    auto button_read = new wxButton(control_panel, ID_BUTTON_READ, "Read number", wxDefaultPosition, wxSize(120, 40));

    //Sizer setting and owner management
    sizer_control_panel->Add(text_brush_size, 0, wxALIGN_CENTER | wxALL, FromDIP(3));
    sizer_control_panel->Add(sizer_pen_panel, 0, wxALIGN_CENTER | wxBOTTOM, FromDIP(50));
    sizer_control_panel->Add(button_clear, 0, wxALIGN_CENTER | wxBOTTOM, FromDIP(40));
    sizer_control_panel->Add(button_read, 0, wxALIGN_CENTER | wxALL, FromDIP(0));
    control_panel->SetSizerAndFit(sizer_control_panel);

    sizer_canvas->Add(gridcanvas, 0, wxALIGN_CENTER | wxALL, FromDIP(10));
    canvas_panel->SetSizer(sizer_canvas);
    
    sizer_main->Add(canvas_panel, 1, wxEXPAND | wxALL, FromDIP(5));
    sizer_main->Add(control_panel, 0, wxEXPAND | wxALL, FromDIP(5));
    this->SetSizer(sizer_main);

    //App initialization for configuration
    SelectPenPanes(penPanes[0]);

    button_clear->Bind(wxEVT_BUTTON, &MyFrame::ClearCanvas, this);
    button_read->Bind(wxEVT_BUTTON, &MyFrame::ReadNumber, this);
};

