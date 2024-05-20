//
// Created by Radica on 2024/5/19.
//

#ifndef CANVAS_EDITORSTATE_CIRCLE_H
#define CANVAS_EDITORSTATE_CIRCLE_H

#include "editorstate.h"

class Context;
class Editorstate_Circle : public EditorState
{
   public:
    Editorstate_Circle(const Context& context);
    ~Editorstate_Circle();

    void entry();
    void exit();

    int mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    int mousePressEvent(QGraphicsSceneMouseEvent* event);
    int mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
};

#endif  //CANVAS_EDITORSTATE_CIRCLE_H
