#ifndef CANVAS_GRAPHICSSHAPEITEM_H
#define CANVAS_GRAPHICSSHAPEITEM_H

#include "redcgl/include/polygon.h"

#include <QGraphicsItem>
#include <QPainter>

class GraphicsShapeItem : public QGraphicsItem
{
   public:
    explicit GraphicsShapeItem(redcgl::Polygon*);
    ~GraphicsShapeItem();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    int set_pen(const QPen& pen);
    int set_brush(const QBrush& brush);

   protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    void setup();
    void init_path();
    void init_poly_head();

   private:

    QPen _pen;
    QBrush _brush;
    QRectF _rect;
    QPainterPath _path;
    redcgl::Polygon* _polygon;
    redcgl::Poly_H* _poly_head;
};

#endif  //CANVAS_GRAPHICSSHAPEITEM_H
