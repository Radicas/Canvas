#ifndef CANVAS_EVENTHANDLER_H
#define CANVAS_EVENTHANDLER_H

class QMouseEvent;
class EventHandler
{
   public:
    EventHandler();
    ~EventHandler();
    virtual int mouseMoveEvent(QMouseEvent* event) = 0;
    virtual int mousePressEvent(QMouseEvent* event) = 0;
    virtual int mouseReleaseEvent(QMouseEvent* event) = 0;
};

#endif  //CANVAS_EVENTHANDLER_H
