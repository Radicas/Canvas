#include "graphicsshapeitem.h"
#include "redcgl/inc/redcgl/edge.h"
#include "redcgl/inc/redcgl/polygon.h"
#include "tools/convertor.h"

#include <qdebug.h>

GraphicsShapeItem::GraphicsShapeItem(redcgl::Polygon* polygon)
    : _pen(Qt::red, 0), _brush(Qt::green), _rect(), _polygon(polygon), _poly_head(nullptr), _path()
{
    auto* curr_poly = _polygon;
    while (curr_poly != nullptr)
    {
        // outer
        _path.addPath(simple_polygon_to_path(curr_poly));

        qDebug() << _path;

        redcgl::Polygon* hole = redcgl::polygon_get_next_hole(curr_poly);
        redcgl::Polygon* curr_hole = hole;
        while (curr_hole != nullptr)
        {
            // holes
            _path.addPath(simple_polygon_to_path(curr_hole));
            curr_hole = redcgl::polygon_get_next_hole(curr_hole);
        }
        curr_poly = redcgl::polygon_get_next_hole(curr_poly);
    }
    polygon_to_poly_h();
}

GraphicsShapeItem::~GraphicsShapeItem() {}
QRectF GraphicsShapeItem::boundingRect() const
{
    return _rect;
}
QPainterPath GraphicsShapeItem::shape() const
{
    return _path;
}
int GraphicsShapeItem::set_pen(const QPen& pen)
{
    _pen = pen;
    return 1;
}
int GraphicsShapeItem::set_brush(const QBrush& brush)
{
    _brush = brush;
    return 1;
}
void GraphicsShapeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(_pen);
    painter->setBrush(_brush);
    painter->drawPath(_path);

    painter->save();
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(_rect);
    painter->restore();
}
void GraphicsShapeItem::polygon_to_poly_h()
{
    
}
QPainterPath GraphicsShapeItem::simple_polygon_to_path(redcgl::Polygon* poly)
{
    if (poly == NULL)
        return {};
    QPainterPath path;
    int edges_num = redcgl::polygon_get_edges_number(poly);
    if (edges_num == 0)
        return {};
    redcgl::Edge* edges = redcgl::polygon_get_edges(poly);
    redcgl::Edge* first_edge = redcgl::edge_at(edges, 0);
    path.moveTo(redcgl::edge_get_st_x(first_edge), redcgl::edge_get_st_y(first_edge));

    for (int i = 0; i < edges_num; ++i)
    {
        redcgl::Edge* edge = redcgl::edge_at(edges, i);
        double ex = redcgl::edge_get_et_x(edge);
        double ey = redcgl::edge_get_et_y(edge);
        double radius = redcgl::edge_get_radius(edge);

        if (radius == 0.0)  // 线段
        {
            path.lineTo(ex, ey);
        }
        else  // 圆弧
        {
            const QtArc qt_arc = edge_to_qt_arc(*edge);
            path.arcTo(qt_arc._rect, qt_arc._start_angle / 16, qt_arc._span_angle / 16);
        }
    }
    path.closeSubpath();
    return path;
}
