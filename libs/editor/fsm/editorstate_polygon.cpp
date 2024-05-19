#include "editorstate_polygon.h"

Editorstate_Polygon::Editorstate_Polygon() {}

Editorstate_Polygon::~Editorstate_Polygon() {}

void Editorstate_Polygon::entry() {}

void Editorstate_Polygon::exit() {}

int Editorstate_Polygon::mouseMoveEvent(QMouseEvent* event)
{
    return 0;
}

int Editorstate_Polygon::mousePressEvent(QMouseEvent* event)
{
    printf("draw polygon press\n");
    return 0;
}

int Editorstate_Polygon::mouseReleaseEvent(QMouseEvent* event)
{
    printf("draw polygon release\n");
    return 0;
}
