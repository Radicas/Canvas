#ifndef CANVAS_CONVERTOR_H
#define CANVAS_CONVERTOR_H

#include <redcgl/include/redcgl.h>
#include <QRectF>

typedef struct qt_arc
{
    QRectF _rect;
    double _start_angle;
    double _span_angle;
} QtArc;

QRectF arc_edge_to_rect(double cx, double cy, double r);

QtArc edge_to_qt_arc(redcgl::Edge& e);

#endif  //CANVAS_CONVERTOR_H
