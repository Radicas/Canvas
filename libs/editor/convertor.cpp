#include "convertor.h"

#include "redcgl/include/edge.h"
#include "redcgl/include/curve.h"

#ifdef WIN32
#include <cmath>
#endif  // WIN32

QRectF arc_edge_to_rect(double cx, double cy, double r)
{
    r = fabs(r);
    return {cx - r, cy - r, 2 * r, 2 * r};
}

QtArc edge_to_qt_arc(redcgl::Edge& e)
{
    using namespace redcgl;
    // 非圆
    if (is_arc_edge(&e))
        return {};

    QtArc qt_arc;

    // 圆形
    double radius = edge_get_radius(&e);
    if (edge_get_sa(&e) == 0.0 && edge_get_ea(&e) == 0.0)
    {
        double abs_r = fabs(radius);
        qt_arc._rect = {edge_get_ct_x(&e) - abs_r, edge_get_ct_y(&e) - abs_r, 2 * abs_r, 2 * abs_r};
        qt_arc._start_angle = 0;
        qt_arc._span_angle = 360 * 16;
        return qt_arc;
    }

    // 圆弧
    int ccw = radius > 0 ? 0 : 1;
    double span_angle = sweep_angle(&e);
    qt_arc._start_angle = (rad_to_degree(edge_get_sa(&e))) * 16;
    span_angle = rad_to_degree(span_angle) * 16;
    qt_arc._span_angle = ccw ? span_angle : -span_angle;
    qt_arc._rect = arc_edge_to_rect(edge_get_ct_x(&e), edge_get_ct_y(&e), edge_get_radius(&e));
    return qt_arc;
}