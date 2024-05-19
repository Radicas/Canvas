#ifndef VIEW_H_
#define VIEW_H_

#include "core/grid.h"

#include <QGraphicsView>

class EventHandler;
class GraphicsView : public QGraphicsView
{
   public:
    explicit GraphicsView(EventHandler* eh);

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
    bool is_panning;
    QPoint _last_pos;
    Grid _grid;
    EventHandler* _event_handler;
};

#endif /* VIEW_H_ */
