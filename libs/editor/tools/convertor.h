#ifndef CANVAS_CONVERTOR_H
#define CANVAS_CONVERTOR_H

#include <QPainterPath>
#include <QRectF>

namespace redcgl
{
struct Edge;
struct Polygon;
}  // namespace redcgl

typedef struct qt_arc
{
    QRectF _rect;
    double _start_angle;
    double _span_angle;
} QtArc;

QRectF arc_edge_to_rect(double cx, double cy, double r);
QtArc edge_to_qt_arc(redcgl::Edge& e);
redcgl::Polygon* qt_rect_f_to_polygon(const QRectF& rect);

#endif  //CANVAS_CONVERTOR_H
