#include "editorstate.h"

EditorState::EditorState(const Context& context) : _context(context) {}

EditorState::~EditorState() {}

void EditorState::entry() {}

void EditorState::exit() {}

int EditorState::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    return 0;
}
int EditorState::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    return 0;
}
int EditorState::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    return 0;
}
