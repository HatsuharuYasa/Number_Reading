#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <vector>

class Canvas : public wxWindow{
public:
    Canvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
    void ClearCanvas();
    virtual ~Canvas() {}
private:
    void OnPaint(wxPaintEvent &);
    void OnMouseDown(wxMouseEvent &);
    void OnMouseUp(wxMouseEvent &);
    void OnMouseMove(wxMouseEvent &);
    void OnMouseLeave(wxMouseEvent &);

    bool m_reset = false;
    bool isDrawing{};
    std::vector<std::vector<wxPoint2DDouble>> squiggles;
};