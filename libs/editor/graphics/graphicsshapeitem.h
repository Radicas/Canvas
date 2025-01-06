#ifndef CANVAS_GRAPHICSSHAPEITEM_H
#define CANVAS_GRAPHICSSHAPEITEM_H

#include "redcgl/inc/redcgl/polygon.h"

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
    void polygon_to_poly_h();
    QPainterPath simple_polygon_to_path(redcgl::Polygon* poly);

    QPen _pen;
    QBrush _brush;
    QRectF _rect;
    QPainterPath _path;
    redcgl::Polygon* _polygon;
    redcgl::Poly_H* _poly_head;
};

#endif  //CANVAS_GRAPHICSSHAPEITEM_H