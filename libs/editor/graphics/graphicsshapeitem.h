#ifndef CANVAS_GRAPHICSSHAPEITEM_H
#define CANVAS_GRAPHICSSHAPEITEM_H

#include <redcgl/include/redcgl.h>
#include <QGraphicsItem>
#include <QPainter>

class GraphicsShapeItem : public QGraphicsItem
{
   public:
    // TODO: 暂时默认shape只有一个多边形
    explicit GraphicsShapeItem(const std::vector<redcgl::Edge>& edges);
    ~GraphicsShapeItem();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    int set_pen(const QPen& pen);
    int set_brush(const QBrush& brush);
    int set_edges(const std::vector<redcgl::Edge>& edges);

    int append_edge(const redcgl::Edge& edge);

   protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    int init_polygon();
    int init_painter_path();
    int set_path_to_circle();

   private:
    QPen _pen;
    QBrush _brush;
    QRectF _rect;
    QPainterPath _path;
    redcgl::Polygon _polygon;
};

#endif  //CANVAS_GRAPHICSSHAPEITEM_H
