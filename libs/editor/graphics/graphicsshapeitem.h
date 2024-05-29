#ifndef CANVAS_GRAPHICSSHAPEITEM_H
#define CANVAS_GRAPHICSSHAPEITEM_H

#include <redcgl/include/redcgl.h>
#include <QGraphicsItem>
#include <QPainter>

class GraphicsShapeItem : public QGraphicsItem
{
   public:
    explicit GraphicsShapeItem(F_POLYHEAD* poly_head);
    ~GraphicsShapeItem();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    redcgl::Polygon& get_polygon();
    F_POLYHEAD* get_poly_head();

    int set_pen(const QPen& pen);
    int set_brush(const QBrush& brush);
    int set_edges(const std::vector<redcgl::Edge>& edges);

    int append_edge(const redcgl::Edge& edge);

   protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    int init_polygon();
    int init_painter_path();
    int set_path_to_circle();
    QPainterPath poly_head_to_path(F_POLYHEAD* poly_head);

   private:
    QPen _pen;
    QBrush _brush;
    QRectF _rect;
    QPainterPath _path;
    F_POLYHEAD* _poly_head;
};

#endif  //CANVAS_GRAPHICSSHAPEITEM_H
