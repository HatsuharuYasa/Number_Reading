#include "canvas.h"
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

Canvas::Canvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size) : wxWindow(parent, id, pos, size){
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->SetBackgroundColour(*wxBLACK);

    this->Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
    this->Bind(wxEVT_LEFT_DOWN, &Canvas::OnMouseDown, this);
    this->Bind(wxEVT_MOTION, &Canvas::OnMouseMove, this);
    this->Bind(wxEVT_LEFT_UP, &Canvas::OnMouseUp, this);
    this->Bind(wxEVT_LEAVE_WINDOW, &Canvas::OnMouseLeave, this);
}

void Canvas::ClearCanvas(){
    for(auto& pt : squiggles){
        pt.clear();
    }
    squiggles.clear();
    Refresh();
}

void Canvas::OnPaint(wxPaintEvent &evt){
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();
    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

    if(gc){
        for(const auto &pointsVector : squiggles){
            if(pointsVector.size() > 1){
                gc->SetPen(wxPen(*wxWHITE, this->FromDIP(8)));
                gc->StrokeLines(pointsVector.size(), pointsVector.data());
            }
        }
        delete gc;
    }

}

void Canvas::OnMouseDown(wxMouseEvent &e){
    squiggles.push_back({});
    isDrawing = true;
}

void Canvas::OnMouseMove(wxMouseEvent &e){
    if(isDrawing){
        auto pt = e.GetPosition();
        auto &curSquiggles = squiggles.back();

        curSquiggles.push_back(pt);
        Refresh();
    }
}

void Canvas::OnMouseUp(wxMouseEvent &e){
    isDrawing = false;
}

void Canvas::OnMouseLeave(wxMouseEvent &e){
    isDrawing = false;
}