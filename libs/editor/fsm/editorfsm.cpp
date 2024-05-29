#include "editorfsm.h"
#include "editorstate_circle.h"
#include "editorstate_op_rect.h"
#include "editorstate_polygon.h"
#include "editorstate_rect.h"
#include "editorstate_select.h"

EditorFsm::EditorFsm(const Context& context) : _current_state(SELECT), _fsm_states(), _context(context)
{
    setup();
}

EditorFsm::~EditorFsm()
{
    _current_state = SELECT;
    for (auto& v : _fsm_states)
    {
        delete v.second;
        v.second = nullptr;
    }
}

void EditorFsm::setup()
{
    _fsm_states[SELECT] = new Editorstate_Select(_context);
    _fsm_states[DRAW_POLYGON] = new Editorstate_Polygon(_context);
    _fsm_states[DRAW_RECT] = new Editorstate_Rect(_context);
    _fsm_states[DRAW_CIRCLE] = new Editorstate_Circle(_context);
    _fsm_states[DIG_RECT] = new Editorstate_Op_Rect(_context);
}

void EditorFsm::set_current_state(FSM_STATE state)
{
    _fsm_states[_current_state]->exit();
    _current_state = state;
    _fsm_states[_current_state]->entry();
}

FSM_STATE EditorFsm::get_current_state()
{
    return _current_state;
}

int EditorFsm::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (_current_state == SELECT)
        return 0;
    return _fsm_states[_current_state]->mouseMoveEvent(event);
}

int EditorFsm::mouseLeftPressEvent(QGraphicsSceneMouseEvent* event)
{
    return _fsm_states[_current_state]->mouseLeftPressEvent(event);
}
int EditorFsm::mouseRightPressEvent(QGraphicsSceneMouseEvent* event)
{
    return _fsm_states[_current_state]->mouseRightPressEvent(event);
}
int EditorFsm::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    return _fsm_states[_current_state]->mouseDoubleClickEvent(event);
}
int EditorFsm::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    return _fsm_states[_current_state]->mouseReleaseEvent(event);
}