#include "convertor.h"

QRectF arc_edge_to_rect(double cx, double cy, double r)
{
    r = fabs(r);
    return {cx - r, cy - r, 2 * r, 2 * r};
}

QtArc edge_to_qt_arc(redcgl::Edge& e)
{
    // 非圆
    if (e.r == 0.0)
        return {};

    QtArc qt_arc;

    // 圆形
    if (e.sa == 0.0 && e.ea == 0.0)
    {
        double abs_r = fabs(e.r);
        qt_arc._rect = {e.ct.x - abs_r, e.ct.y - abs_r, 2 * abs_r, 2 * abs_r};
        qt_arc._start_angle = 0;
        qt_arc._span_angle = 360 * 16;
        return qt_arc;
    }

    // 圆弧
    int ccw = e.r > 0 ? 0 : 1;
    double span_angle = redcgl::sweep_angle(&e);
    qt_arc._start_angle = (redcgl::rad_to_degree(e.sa)) * 16;
    span_angle = redcgl::rad_to_degree(span_angle) * 16;
    qt_arc._span_angle = ccw ? span_angle : -span_angle;
    qt_arc._rect = arc_edge_to_rect(e.ct.x, e.ct.y, e.r);
    return qt_arc;
}