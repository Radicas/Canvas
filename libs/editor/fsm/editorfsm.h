#ifndef CANVAS_EDITORFSM_H
#define CANVAS_EDITORFSM_H

#include "editorstate.h"
#include "eventhandler.h"

#include <QObject>

enum FSM_STATE
{
    SELECT = 0,
    DRAW_POLYGON,
    DRAW_CIRCLE,
};

class EditorState;
class EditorFsm : public QObject, public EventHandler
{
   public:
    EditorFsm();
    ~EditorFsm();

    void set_current_state(FSM_STATE state);

    FSM_STATE get_current_state();

    int mouseMoveEvent(QMouseEvent* event) override;
    int mousePressEvent(QMouseEvent* event) override;
    int mouseReleaseEvent(QMouseEvent* event) override;

   private:
    void setup();

    std::map<FSM_STATE, EditorState*> _fsm_states;
    FSM_STATE _current_state;
};

#endif  //CANVAS_EDITORFSM_H
