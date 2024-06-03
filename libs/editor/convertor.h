#ifndef CANVAS_CONVERTOR_H
#define CANVAS_CONVERTOR_H

#include <QRectF>

namespace redcgl
{
struct Edge;
}

typedef struct qt_arc
{
    QRectF _rect;
    double _start_angle;
    double _span_angle;
} QtArc;

QRectF arc_edge_to_rect(double cx, double cy, double r);

QtArc edge_to_qt_arc(redcgl::Edge& e);

#endif  //CANVAS_CONVERTOR_H
