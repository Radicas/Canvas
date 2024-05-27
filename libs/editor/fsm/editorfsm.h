#ifndef CANVAS_EDITORFSM_H
#define CANVAS_EDITORFSM_H

#include "editorstate.h"
#include "eventhandler.h"

#include <QObject>

enum FSM_STATE
{
    SELECT = 0,
    DRAW_POLYGON,
    DRAW_RECT,
    DRAW_CIRCLE,
    DIG_RECT,
};

class Context;
class EditorState;
class EditorFsm : public QObject, public EventHandler
{
   public:
    explicit EditorFsm(const Context& context);
    ~EditorFsm();

    void set_current_state(FSM_STATE state);

    FSM_STATE get_current_state();

    int mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseLeftPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseRightPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

   private:
    void setup();

    std::map<FSM_STATE, EditorState*> _fsm_states;
    FSM_STATE _current_state;
    const Context& _context;
};

#endif  //CANVAS_EDITORFSM_H
