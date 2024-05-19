#ifndef CANVAS_EDITORSTATE_POLYGON_H
#define CANVAS_EDITORSTATE_POLYGON_H

#include "editorstate.h"

class Editorstate_Polygon : public EditorState
{
   public:
    Editorstate_Polygon();
    ~Editorstate_Polygon();

    void entry();
    void exit();

    int mouseMoveEvent(QMouseEvent* event);
    int mousePressEvent(QMouseEvent* event);
    int mouseReleaseEvent(QMouseEvent* event);
};

#endif  //CANVAS_EDITORSTATE_POLYGON_H
