#pragma once

#include <wx/wx.h>
#include <utility>

class GridCanvas : public wxWindow{
public:
    GridCanvas(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, 
            const wxSize &size = wxDefaultSize, 
            const std::pair<int, int> &grids = std::make_pair(0, 0));
    void ClearCanvas();
    int** GetCanvas() const;
    std::pair<int, int> GetSize() const;
    ~GridCanvas(){
        for(int i = 0; i < gridSize.first; i++){
            delete[] gridArray[i];
        }
        delete[] gridArray;
    }
private:
    void OnPaint(wxPaintEvent &);
    void OnMouseDown(wxMouseEvent &);
    void OnMouseMove(wxMouseEvent &);
    void OnMouseUp(wxMouseEvent &);
    void OnMouseLeave(wxMouseEvent &);
    
    std::pair<int, int> gridSize;
    int **gridArray;

    int pixelSize;
    bool isDrawing = false;
};