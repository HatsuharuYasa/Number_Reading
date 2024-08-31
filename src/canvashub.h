#pragma once

#include "canvas.h"
#include "gridcanvas.h"

#include <wx/wx.h>
#include <utility>

class CanvasHub : public wxPanel{
public:
    CanvasHub(wxWindow *parent, wxWindowID id = wxID_ANY, 
            const wxPoint &pos = wxDefaultPosition, 
            const wxSize &size = wxDefaultSize,
            const std::pair<int, int> &grids = std::make_pair(0, 0));
    void SwitchToVector();
    void SwitchToGrid();
    void ClearCanvas();
    float **GetCanvas() const;
    std::pair<int, int> GetSize() const;
    ~CanvasHub(){
        delete vectorcanvas, gridcanvas, mainpanel, mainsizer, subsizer;
    }
private:
    Canvas *vectorcanvas;
    GridCanvas *gridcanvas;
    wxPanel *mainpanel;

    wxBoxSizer *mainsizer;
    wxBoxSizer *subsizer;

    std::pair<int, int> gridSize;

    bool GRIDCANVASSELECTED = true;
};