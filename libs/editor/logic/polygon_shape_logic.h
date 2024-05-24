#ifndef CANVAS_POLYGON_SHAPE_LOGIC_H
#define CANVAS_POLYGON_SHAPE_LOGIC_H

#include <QPainterPath>
#include <QRectF>
#include "redcgl/include/redcgl.h"

typedef struct qt_arc
{
    QRectF _rect;
    double _start_angle;
    double _span_angle;
} QtArc;

QRectF arc_edge_to_rect(double cx, double cy, double r);
QtArc edge_to_qt_arc(redcgl::Edge& e);
redcgl::Edge qt_arc_to_edge(const QtArc& arc);

int cal_snap_45_strategy_1(QPointF& st, QPointF mp, QPointF& sp);
int cal_snap_45_strategy_2(QPointF& st, QPointF mp, QPointF& sp);
int cal_snap_45(double angle, int inclined, QPointF& p1, QPointF& p2, QPointF& res);

QPainterPath polygon_to_painter_path(const redcgl::Polygon& polygon);

#endif  //CANVAS_POLYGON_SHAPE_LOGIC_H
