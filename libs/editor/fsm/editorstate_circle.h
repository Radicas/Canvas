//
// Created by Radica on 2024/5/19.
//

#ifndef CANVAS_EDITORSTATE_CIRCLE_H
#define CANVAS_EDITORSTATE_CIRCLE_H

#include "editorstate.h"

class Editorstate_Circle : public EditorState
{
   public:
    Editorstate_Circle();
    ~Editorstate_Circle();

    void entry();
    void exit();

    int mouseMoveEvent(QMouseEvent* event);
    int mousePressEvent(QMouseEvent* event);
    int mouseReleaseEvent(QMouseEvent* event);
};

#endif  //CANVAS_EDITORSTATE_CIRCLE_H
