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

    void entry() override;
    void exit() override;

    int mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseLeftPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseRightPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif  //CANVAS_EDITORSTATE_CIRCLE_H
