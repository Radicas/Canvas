#ifndef CANVAS_EDITORSTATE_H
#define CANVAS_EDITORSTATE_H

#include <QObject>

class QMouseEvent;
class EditorState : public QObject
{
   public:
    EditorState();
    ~EditorState();

    virtual void entry();
    virtual void exit();

    virtual int mouseMoveEvent(QMouseEvent* event);
    virtual int mousePressEvent(QMouseEvent* event);
    virtual int mouseReleaseEvent(QMouseEvent* event);
};

#endif  //CANVAS_EDITORSTATE_H
