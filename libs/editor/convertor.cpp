#include "convertor.h"

#include "redcgl/include/curve.h"
#include "redcgl/include/edge.h"
#include "redcgl/include/polygon.h"

#ifdef WIN32
#include <cmath>
#endif  // WIN32

using Edge = redcgl::Edge;
using Polygon = redcgl::Polygon;

QRectF arc_edge_to_rect(double cx, double cy, double r)
{
    r = fabs(r);
    return {cx - r, cy - r, 2 * r, 2 * r};
}
QtArc edge_to_qt_arc(Edge& e)
{
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
    qt_arc._start_angle = (redcgl::rad_to_degree(redcgl::edge_get_sa(&e))) * 16;
    span_angle = redcgl::rad_to_degree(span_angle) * 16;
    qt_arc._span_angle = ccw ? span_angle : -span_angle;
    qt_arc._rect = arc_edge_to_rect(edge_get_ct_x(&e), edge_get_ct_y(&e), edge_get_radius(&e));
    return qt_arc;
}

Polygon* qt_rect_f_to_polygon(const QRectF& rect)
{
    Polygon* poly = NULL;
    Edge* edges = redcgl::create_empty_edge_array(4);

    auto top_left = rect.topLeft();
    auto btm_left = rect.bottomLeft();
    auto btm_right = rect.bottomRight();
    auto top_right = rect.topRight();

    Edge* edge0 = get_edge(edges, 0);
    Edge* edge1 = get_edge(edges, 1);
    Edge* edge2 = get_edge(edges, 2);
    Edge* edge3 = get_edge(edges, 3);

    redcgl::edge_set_values(edge0, top_left.x(), top_left.y(), btm_left.x(), btm_left.y(), 0, 0, 0, 0, 0);
    redcgl::edge_set_values(edge1, btm_left.x(), btm_left.y(), btm_right.x(), btm_right.y(), 0, 0, 0, 0, 0);
    redcgl::edge_set_values(edge2, btm_right.x(), btm_right.y(), top_right.x(), top_right.y(), 0, 0, 0, 0, 0);
    redcgl::edge_set_values(edge3, top_right.x(), top_right.y(), top_left.x(), top_left.y(), 0, 0, 0, 0, 0);

    redcgl::print_edges(edges, 4);
    return redcgl::create_polygon(edges, 4, nullptr);
}