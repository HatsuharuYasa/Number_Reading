#include "canvas.h"
#include "gridcanvas.h"
#include "canvashub.h"

#include <wx/wx.h>

CanvasHub::CanvasHub(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, const std::pair<int, int> &grids)
    : wxPanel(parent, id, pos, size), gridSize(grids){
    this->SetBackgroundColour(wxColour(211, 211, 211));

    mainpanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(392, 392));
    mainpanel->SetBackgroundColour(this->GetBackgroundColour());

    this->gridcanvas = new GridCanvas(mainpanel, wxID_ANY, wxDefaultPosition, this->mainpanel->GetSize(), gridSize);
    this->vectorcanvas = new Canvas(mainpanel, wxID_ANY, wxDefaultPosition, this->mainpanel->GetSize(), gridSize);

    mainsizer = new wxBoxSizer(wxVERTICAL);
    subsizer = new wxBoxSizer(wxVERTICAL);
    
    subsizer->Add(gridcanvas, 1, wxALIGN_CENTER);
    mainpanel->SetSizer(subsizer);
    
    mainsizer->Add(mainpanel, 0, wxALIGN_CENTER);
    this->SetSizer(mainsizer);

    gridcanvas->Show(true);
    vectorcanvas->Show(false);

}

void CanvasHub::SwitchToVector(){
    if(GRIDCANVASSELECTED){
        GRIDCANVASSELECTED = false;
        vectorcanvas->ClearCanvas();
        vectorcanvas->Refresh();
        
        gridcanvas->Show(false);
        vectorcanvas->Show(true);

        subsizer->Detach(gridcanvas);
        subsizer->Add(vectorcanvas, 0, wxALIGN_CENTER);

        this->mainpanel->Layout();
    }
}

void CanvasHub::SwitchToGrid(){
    if(!GRIDCANVASSELECTED){
        GRIDCANVASSELECTED = true;
        gridcanvas->ClearCanvas();
        gridcanvas->Refresh();

        gridcanvas->Show(true);
        vectorcanvas->Show(false);

        subsizer->Detach(vectorcanvas);
        subsizer->Add(gridcanvas, 0, wxALIGN_CENTER);

        this->mainpanel->Layout();
    }
}

void CanvasHub::ClearCanvas(){
    if(GRIDCANVASSELECTED){
        gridcanvas->ClearCanvas();
        gridcanvas->Refresh();
    }
    else{
        vectorcanvas->ClearCanvas();
        vectorcanvas->Refresh();
    }
}

float** CanvasHub::GetCanvas() const{
    if(GRIDCANVASSELECTED){
        return gridcanvas->GetCanvas();
    }
    else{
        return vectorcanvas->GetCanvas();
    }
}

std::pair<int, int> CanvasHub::GetSize() const{
    return gridSize;
}

