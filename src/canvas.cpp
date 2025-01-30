#include "canvas.h"
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

Canvas::Canvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, const std::pair<int, int> &grids) 
    : wxWindow(parent, id, pos, size), gridSize(grids){
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
                gc->SetPen(wxPen(*wxWHITE, this->FromDIP(15)));
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

float** Canvas::GetCanvas() const{
    //Acquire the panel size
    wxSize panelSize = GetSize();

    //Create the bitmap with the same panel size
    wxBitmap bitmap(panelSize.GetWidth(), panelSize.GetHeight());

    //Create the MemoryDC
    wxMemoryDC memdc(bitmap);
    memdc.SetBackground(*wxBLACK_BRUSH);
    memdc.Clear();
    wxGraphicsContext *gc = wxGraphicsContext::Create(memdc);

    if(gc){
        for(const auto &pointsvector : squiggles){
            if(pointsvector.size() > 1){
                gc->SetPen(wxPen(*wxWHITE, this->FromDIP(15)));
                gc->StrokeLines(pointsvector.size(), pointsvector.data());
            }
        }
        delete gc;
    }

    memdc.SelectObject(wxNullBitmap);

    wxImage image = bitmap.ConvertToImage();
    wxImage resizedimage = image.Rescale(gridSize.first, gridSize.second, wxIMAGE_QUALITY_NORMAL);

    float **pd = (float**)new float*[gridSize.first];
    for(int i = 0; i < gridSize.first; i++){
        pd[i] = (float*)new float[gridSize.second]();
    }

    for(int i = 0; i < gridSize.first; i++){
        for(int j = 0; j < gridSize.second; j++){
            //Acquire the rgb value
            unsigned char r = resizedimage.GetRed(i, j);
            unsigned char g = resizedimage.GetGreen(i, j);
            unsigned char b = resizedimage.GetBlue(i, j);

            //Store the converted grayscale value to the pixeldata
            pd[i][j] = 0.299f * r + 0.587f * g + 0.114f * b;
        }
    }
    return pd;
}