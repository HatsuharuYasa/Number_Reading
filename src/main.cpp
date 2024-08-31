#include "canvashub.h"
#include "penpane.h"
#include "inference.h"
#include "canvaspane.h"

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
        delete logger, canvashub;
    }
private:
    void ClearCanvas(wxCommandEvent &);
    void ReadNumber(wxCommandEvent &, wxStaticText *);

    void SetupPenPanes(wxWindow *parent, wxSizer *sizer);
    void SelectPenPanes(PenPane* pane);

    void SetupCanvasPanes(wxWindow *parent, wxSizer *sizer);
    void SelectCanvasPanes(CanvasPane *pane);

    wxLog *logger;
    CanvasHub *canvashub;

    std::vector<PenPane*> penPanes{};
    std::vector<CanvasPane*> canvasPanes{};
    const int penCount = 3;

    bool GRIDCANVASSELECTED = true;

    ModelInference model;
};

bool MyApp::OnInit(){
    MyFrame *frame = new MyFrame("Read the Number App", wxDefaultPosition, wxSize(640, 480));
    frame->Show(true);
    return true;
};

enum{
    ID_BUTTON_CLEAR = wxID_LAST + 1,
    ID_BUTTON_READ
};

void MyFrame::SetupPenPanes(wxWindow *parent, wxSizer *sizer){//To set up the pen size selection
    for(int i = 0; i < penCount; i++){
        auto penPane = new PenPane(parent, wxID_ANY, i * FromDIP(3) + FromDIP(10));

        penPane->Bind(wxEVT_LEFT_DOWN, [this, penPane](wxMouseEvent &e){
            this->SelectPenPanes(penPane);
        });

        penPanes.push_back(penPane);
        sizer->Add(penPane, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, FromDIP(5));
    }
}

void MyFrame::SelectPenPanes(PenPane *pane){//To select the pen size
    for(auto penPane:penPanes){
        penPane->selected = (penPane==pane);
        penPane->Refresh();
    }
}

void MyFrame::SetupCanvasPanes(wxWindow *parent, wxSizer *sizer){//To set up the canvas type selection
    auto pane1 = new CanvasPane(parent, wxID_ANY, "Grid", wxDefaultPosition, wxSize(90, 45));
    auto pane2 = new CanvasPane(parent, wxID_ANY, "Vector", wxDefaultPosition, wxSize(90, 45));

    pane1->Bind(wxEVT_LEFT_DOWN, [this, pane1](wxMouseEvent &e){
        this->SelectCanvasPanes(pane1);
    });
    pane2->Bind(wxEVT_LEFT_DOWN, [this, pane2](wxMouseEvent &e){
        this->SelectCanvasPanes(pane2);
    });

    canvasPanes.push_back(pane1);
    canvasPanes.push_back(pane2);
    sizer->Add(pane1, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, FromDIP(5));
    sizer->Add(pane2, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, FromDIP(5));
}

void MyFrame::SelectCanvasPanes(CanvasPane *pane){//To select the canvas type
    if(pane == canvasPanes[0]){
        canvasPanes[0]->selected = true;
        canvasPanes[1]->selected = false;
        canvasPanes[0]->Refresh();
        canvasPanes[1]->Refresh();
        canvashub->SwitchToGrid();
        GRIDCANVASSELECTED = true;
    }
    else{
        canvasPanes[0]->selected = false;
        canvasPanes[1]->selected = true;
        canvasPanes[0]->Refresh();
        canvasPanes[1]->Refresh();
        canvashub->SwitchToVector();
        GRIDCANVASSELECTED = false;
    }
}

void MyFrame::ClearCanvas(wxCommandEvent &e){//To clear the canvas
    canvashub->ClearCanvas();
}

void MyFrame::ReadNumber(wxCommandEvent &e, wxStaticText *text_display){
    std::pair<int, int> gs = canvashub->GetSize();
    int pred = model.RunInference(canvashub->GetCanvas(), gs.first, gs.second);
    wxLogMessage(L"That is number %d", pred);
    text_display->SetLabel(wxString::Format("%d", pred));
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size) 
: wxFrame(nullptr, wxID_ANY, title, pos, size, wxCAPTION | wxCLOSE_BOX | wxSYSTEM_MENU | wxMINIMIZE_BOX){
    logger = new wxLogWindow(this, "WxLog", true, false);
    wxLog::SetActiveTarget(logger);

    //Panel configuration
    wxPanel *control_panel = new wxPanel(this, wxID_ANY, 
                            wxDefaultPosition, wxDefaultSize);
    control_panel->SetBackgroundColour(*wxWHITE);

    canvashub = new CanvasHub(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, std::make_pair(28, 28));

    //Sizer initialization
    auto sizer_main = new wxBoxSizer(wxHORIZONTAL);
    auto sizer_control_panel = new wxBoxSizer(wxVERTICAL);
    //auto sizer_pen_panel = new wxBoxSizer(wxHORIZONTAL);
    auto sizer_canvas_panel = new wxBoxSizer(wxHORIZONTAL);
    auto sizer_canvas = new wxBoxSizer(wxHORIZONTAL);

    //Item configuration
    //SetupPenPanes(control_panel, sizer_pen_panel);
    SetupCanvasPanes(control_panel, sizer_canvas_panel);

    //auto text_brushsize = new wxStaticText(control_panel, wxID_ANY, "Brush Size");
    auto text_numberread_pre = new wxStaticText(control_panel, wxID_ANY, "This number is");
    auto text_numberread = new wxStaticText(control_panel, wxID_ANY, "");
    wxFont font = text_numberread_pre->GetFont();
    font.SetPointSize(12);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    text_numberread_pre->SetFont(font);
    text_numberread->SetFont(font);
    auto text_canvastype = new wxStaticText(control_panel, wxID_ANY, "Canvas Type");

    auto button_clear = new wxButton(control_panel, ID_BUTTON_CLEAR, "Clear", wxDefaultPosition, wxSize(120, 40));
    auto button_read = new wxButton(control_panel, ID_BUTTON_READ, "Read number", wxDefaultPosition, wxSize(120, 40));

    //Sizer setting and owner management
    //sizer_control_panel->Add(text_brushsize, 0, wxALIGN_CENTER | wxALL, FromDIP(3));
    //sizer_control_panel->Add(sizer_pen_panel, 0, wxALIGN_CENTER | wxBOTTOM, FromDIP(50));
    sizer_control_panel->Add(text_canvastype, 0, wxALIGN_CENTER | wxALL, FromDIP(5));
    sizer_control_panel->Add(sizer_canvas_panel, 0, wxALIGN_CENTER);
    sizer_control_panel->AddSpacer(FromDIP(40));
    sizer_control_panel->Add(button_clear, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, FromDIP(10));
    sizer_control_panel->AddSpacer(FromDIP(40));
    sizer_control_panel->Add(button_read, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, FromDIP(10));
    sizer_control_panel->AddStretchSpacer();
    sizer_control_panel->Add(text_numberread_pre, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, FromDIP(10));
    sizer_control_panel->Add(text_numberread, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, FromDIP(10));
    sizer_control_panel->AddSpacer(FromDIP(40));
    control_panel->SetSizerAndFit(sizer_control_panel);
    
    sizer_main->Add(canvashub, 1, wxEXPAND | wxALL, FromDIP(1));
    sizer_main->Add(control_panel, 0, wxEXPAND | wxALL, FromDIP(1));
    this->SetSizer(sizer_main);

    //App initialization for configuration
    //SelectPenPanes(penPanes[0]);
    SelectCanvasPanes(canvasPanes[0]);

    button_clear->Bind(wxEVT_BUTTON, &MyFrame::ClearCanvas, this);
    button_read->Bind(wxEVT_BUTTON, [this, text_numberread](wxCommandEvent &e){
        ReadNumber(e, text_numberread);
    });
};

