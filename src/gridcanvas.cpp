#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <utility>

#include "gridcanvas.h"

GridCanvas::GridCanvas(wxWindow *parent, wxWindowID id, 
const wxPoint &pos, const wxSize &size, 
const std::pair<int, int> &grids) 
    : wxWindow(parent, id, pos, size), gridSize(grids)
{
    int w = size.GetWidth()/gridSize.first, h = size.GetHeight()/gridSize.second;
    pixelSize = (w < h) ? w : h;

    gridArray = (float**)new int*[gridSize.first];
    for(int i = 0; i < gridSize.first; i++){
        gridArray[i] = (float*)new int[gridSize.second]();
    }

    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->SetBackgroundColour(parent->GetBackgroundColour());

    this->Bind(wxEVT_PAINT, &GridCanvas::OnPaint, this);
    this->Bind(wxEVT_LEFT_DOWN, &GridCanvas::OnMouseDown, this);
    this->Bind(wxEVT_MOTION, &GridCanvas::OnMouseMove, this);
    this->Bind(wxEVT_LEFT_UP, &GridCanvas::OnMouseUp, this);
    this->Bind(wxEVT_LEAVE_WINDOW, &GridCanvas::OnMouseLeave, this);
}

void GridCanvas::OnPaint(wxPaintEvent &e){
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

    if(gc){
        for(int i = 0; i < gridSize.first; i++){
            for(int j = 0; j < gridSize.second; j++){
                int gridColor = gridArray[i][j];
                gc->SetBrush(wxBrush(wxColor(gridColor, gridColor, gridColor)));
                gc->DrawRectangle(i*pixelSize, j*pixelSize, 
                                    FromDIP(pixelSize), 
                                    FromDIP(pixelSize));
            }
        }
        delete gc;
    }
}

void GridCanvas::OnMouseDown(wxMouseEvent &e){
    int array_pt_x = e.GetX() / pixelSize;
    int array_pt_y = e.GetY() / pixelSize;

    gridArray[array_pt_x][array_pt_y] = 255 > gridArray[array_pt_x][array_pt_y] ? 255 : gridArray[array_pt_x][array_pt_y];
    isDrawing = true;
    Refresh();
}

void GridCanvas::OnMouseMove(wxMouseEvent &e){
    if(isDrawing){
        int array_pt_x = e.GetX() / pixelSize;
        int array_pt_y = e.GetY() / pixelSize;

        gridArray[array_pt_x][array_pt_y] = 255 > gridArray[array_pt_x][array_pt_y] ? 255 : gridArray[array_pt_x][array_pt_y];
    }
    Refresh();
}

void GridCanvas::OnMouseUp(wxMouseEvent &e){
    isDrawing = false;
}

void GridCanvas::OnMouseLeave(wxMouseEvent &e){
    isDrawing = false;
}

void GridCanvas::ClearCanvas(){
    for(int i = 0; i < gridSize.first; i++){
        for(int j = 0; j < gridSize.second; j++){
            gridArray[i][j] = 0;
        }
    }
    Refresh();
}

std::pair<int, int> GridCanvas::GetSize() const{
    return gridSize;
}

float** GridCanvas::GetCanvas() const{
    return gridArray;
}

