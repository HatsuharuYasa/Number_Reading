#pragma once

#include "selectionpane.h"

class PenPane : public SelectionPane{
public:
    PenPane(wxWindow *parent, wxWindowID id = wxID_ANY, int penW = 1, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);
    int penWidth;
private:
    virtual void DrawContent(wxGraphicsContext *gc, const wxRect &rect, int roundness) const override;
};

PenPane::PenPane(wxWindow *parent, wxWindowID id, int penW, const wxPoint &pos, const wxSize &size)
    : SelectionPane(parent, id, pos, size), penWidth(penW)
{
}

void PenPane::DrawContent(wxGraphicsContext *gc, const wxRect &rect, int roundness) const{
    gc->SetPen(*wxGREY_PEN);
    gc->SetBrush(*wxGREY_BRUSH);

    gc->DrawRoundedRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight(), roundness);

    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 2 - penWidth / 2, 
                    rect.GetY() + rect.GetHeight() / 2 - penWidth / 2,
                    penWidth, penWidth);
}