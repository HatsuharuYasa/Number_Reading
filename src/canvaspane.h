#pragma once

#include "selectionpane.h"

class CanvasPane : public SelectionPane{
public:
    CanvasPane(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &text = "", const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);
private:
    wxString context_text;
    virtual void DrawContent(wxGraphicsContext *gc, const wxRect &rect, int roundness) const override;
};

CanvasPane::CanvasPane(wxWindow *parent, wxWindowID id, const wxString &text, const wxPoint &pos, const wxSize &size)
    : SelectionPane(parent, id, pos, size), context_text(text)
{
}

void CanvasPane::DrawContent(wxGraphicsContext *gc, const wxRect &rect, int roundness) const{
    gc->SetPen(*wxBLACK);
    gc->SetBrush(*wxWHITE);

    gc->DrawRoundedRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight(), roundness);

    gc->SetFont(*wxNORMAL_FONT, *wxBLACK);
    double textWidth, textHeight;
    gc->GetTextExtent(context_text, &textWidth, &textHeight);

    gc->DrawText(context_text, rect.GetX() + rect.GetWidth() / 2.0 - textWidth / 2.0, rect.GetY() + rect.GetHeight() / 2.0 - textHeight / 2.0);
}

