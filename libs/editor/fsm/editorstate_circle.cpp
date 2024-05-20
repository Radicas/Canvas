#include "editorstate_circle.h"

Editorstate_Circle::Editorstate_Circle(const Context& context) : EditorState(context) {}

Editorstate_Circle::~Editorstate_Circle() {}

void Editorstate_Circle::entry() {}

void Editorstate_Circle::exit() {}

int Editorstate_Circle::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    return 0;
}
int Editorstate_Circle::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    printf("draw circle press\n");
    return 0;
}
int Editorstate_Circle::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    printf("draw circle release\n");
    return 0;
}
