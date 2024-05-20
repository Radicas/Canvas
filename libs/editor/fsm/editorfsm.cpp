#include "editorfsm.h"
#include "editorstate_circle.h"
#include "editorstate_polygon.h"

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
    _fsm_states[DRAW_POLYGON] = new Editorstate_Polygon(_context);
    _fsm_states[DRAW_CIRCLE] = new Editorstate_Circle(_context);
}

int EditorFsm::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (_current_state == SELECT)
        return 0;
    return _fsm_states[_current_state]->mouseMoveEvent(event);
}

int EditorFsm::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (_current_state == SELECT)
        return 0;
    return _fsm_states[_current_state]->mousePressEvent(event);
}

int EditorFsm::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (_current_state == SELECT)
        return 0;
    return _fsm_states[_current_state]->mouseReleaseEvent(event);
}

void EditorFsm::set_current_state(FSM_STATE state)
{
    _current_state = state;
}

FSM_STATE EditorFsm::get_current_state()
{
    return _current_state;
}
