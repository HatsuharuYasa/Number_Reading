#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <vector>
#include <utility>

class Canvas : public wxWindow{
public:
    Canvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, const std::pair<int, int> &grids = std::make_pair(0, 0));
    void ClearCanvas();
    float** GetCanvas() const;
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
    std::pair<int, int> gridSize;
};