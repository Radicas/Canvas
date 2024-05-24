//
// Created by Radica on 2024/5/20.
//

#include "editorstate_select.h"
Editorstate_Select::Editorstate_Select(const Context& context) : EditorState(context) {}
Editorstate_Select::~Editorstate_Select() {}
void Editorstate_Select::entry()
{
    EditorState::entry();
}
void Editorstate_Select::exit()
{
    EditorState::exit();
}

int Editorstate_Select::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    return 0;
}
int Editorstate_Select::mouseLeftPressEvent(QGraphicsSceneMouseEvent* event)
{
    return 0;
}
int Editorstate_Select::mouseRightPressEvent(QGraphicsSceneMouseEvent* event)
{
    return 0;
}
int Editorstate_Select::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    return 0;
}
int Editorstate_Select::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    return 0;
}