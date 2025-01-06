//
// Created by Radica on 2024/5/20.
//

#ifndef CANVAS_EDITORSTATE_SELECT_H
#define CANVAS_EDITORSTATE_SELECT_H

#include "editorstate.h"

class Context;
class Editorstate_Select : public EditorState
{
   public:
    explicit Editorstate_Select(const Context& context);
    ~Editorstate_Select() override;

    void entry() override;
    void exit() override;

    int mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseLeftPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseRightPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
};
#endif  //CANVAS_EDITORSTATE_SELECT_H
