#pragma once

#include <wx/graphics.h>
#include <wx/settings.h>
#include <wx/dcbuffer.h>

class SelectionPane : public wxWindow{
public:
    SelectionPane(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);

    wxSize DoGetBestSize() const override{
        return FromDIP(wxSize(45, 45));
    }

    bool selected = false;

protected:
    virtual void DrawContent(wxGraphicsContext *gc, const wxRect &rect, int roundness) const = 0;

private:
    void OnPaint(wxPaintEvent &event);
};

SelectionPane::SelectionPane(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size) 
    : wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    this->Bind(wxEVT_PAINT, &SelectionPane::OnPaint, this);
}

void SelectionPane::OnPaint(wxPaintEvent &event){
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(wxBrush(this->GetParent()->GetBackgroundColour()));
    dc.Clear();

    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

    if(gc){
        wxRect selectionRect {0,0,this->GetSize().GetWidth(), this->GetSize().GetHeight()};
        selectionRect.Deflate(FromDIP(1));

        wxRect contentRect = selectionRect;
        contentRect.Deflate(FromDIP(2));

        const auto roundness = FromDIP(4);

        DrawContent(gc, contentRect, roundness);

        if(selected){
            gc->SetPen(*wxBLACK_PEN);
            gc->SetBrush(*wxTRANSPARENT_BRUSH);
            gc->DrawRoundedRectangle(selectionRect.GetX(), selectionRect.GetY(), selectionRect.GetWidth(), selectionRect.GetHeight(), roundness);
        }

        delete gc;
    }
}