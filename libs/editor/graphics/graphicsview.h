#ifndef GRAPHICS_VIEW_H_
#define GRAPHICS_VIEW_H_

#include <QGraphicsView>
#include "core/grid.h"

class Context;
class EventHandler;
class GraphicsView : public QGraphicsView
{
   public:
    GraphicsView(const Context& context, EventHandler* event_handler);

    virtual void setup();
   protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void drawForeground(QPainter* painter, const QRectF& rect) override;
    void drawBackground(QPainter* painter, const QRectF& rect) override;

    virtual void scrollWithDegrees(const QPoint& numSteps);
   private:
    bool mIsPanning;
    QPoint mLastPos;
    Grid mGrid;
    EventHandler* _event_handler;
};

#endif /* GRAPHICS_VIEW_H_ */
