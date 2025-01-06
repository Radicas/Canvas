#ifndef CANVAS_EVENTHANDLER_H
#define CANVAS_EVENTHANDLER_H

class QGraphicsSceneMouseEvent;
class EventHandler
{
   public:
    EventHandler();
    ~EventHandler();
    virtual int mouseMoveEvent(QGraphicsSceneMouseEvent* event) = 0;
    virtual int mouseLeftPressEvent(QGraphicsSceneMouseEvent* event) = 0;
    virtual int mouseRightPressEvent(QGraphicsSceneMouseEvent* event) = 0;
    virtual int mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) = 0;
    virtual int mouseReleaseEvent(QGraphicsSceneMouseEvent* event) = 0;
};

#endif  //CANVAS_EVENTHANDLER_H
