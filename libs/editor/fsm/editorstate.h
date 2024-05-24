#ifndef CANVAS_EDITORSTATE_H
#define CANVAS_EDITORSTATE_H

#include <QObject>

class QGraphicsSceneMouseEvent;
class Context;
class EditorState : public QObject
{
   public:
    EditorState(const Context& context);
    ~EditorState();

    virtual void entry();
    virtual void exit();

    virtual int mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual int mouseLeftPressEvent(QGraphicsSceneMouseEvent* event);
    virtual int mouseRightPressEvent(QGraphicsSceneMouseEvent* event);
    virtual int mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
    virtual int mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

   protected:
    const Context& _context;
};

#endif  //CANVAS_EDITORSTATE_H
