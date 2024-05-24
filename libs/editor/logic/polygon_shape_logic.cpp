#include "polygon_shape_logic.h"
#include <QRectF>

QRectF arc_edge_to_rect(double cx, double cy, double r)
{
    r = fabs(r);
    return {cx - r, cy - r, 2 * r, 2 * r};
}

QtArc edge_to_qt_arc(redcgl::Edge& e)
{
    if (e.r == 0.0)
        return {};
    QtArc qt_arc;

    int ccw = e.r > 0 ? 0 : 1;

    double span_angle = redcgl::sweep_angle(&e);

    qt_arc._start_angle = (redcgl::rad_to_degree(e.sa)) * 16;
    span_angle = redcgl::rad_to_degree(span_angle) * 16;
    qt_arc._span_angle = ccw ? span_angle : -span_angle;
    qt_arc._rect = arc_edge_to_rect(e.ct.x, e.ct.y, e.r);
    return qt_arc;
}

redcgl::Edge qt_arc_to_edge(const QtArc& arc)
{
    double center_x, center_y, radius;
    auto& rect = arc._rect;
    center_x = rect.x() + rect.width() / 2.0;
    center_y = rect.y() + rect.height() / 2.0;
    radius = rect.width() / 2.0;
    redcgl::Edge edge;
    redcgl::empty_edge(&edge);
    edge.ct.x = center_x;
    edge.ct.y = center_y;
    int ccw = arc._span_angle > 0 ? 1 : 0;
    edge.r = ccw ? -radius : radius;
    edge.sa = redcgl::degree_to_rad(arc._start_angle / 16.0);
    double span_angle = redcgl::degree_to_rad((arc._span_angle / 16.0));
    double tea = edge.sa + span_angle;
    edge.ea = fmod(tea, 2 * M_PI);

    double abs_r = fabs(edge.r);
    edge.st.x = edge.ct.x + abs_r + cos(edge.sa);
    edge.st.y = edge.ct.y + abs_r + sin(edge.sa);
    edge.et.x = edge.ct.x + abs_r + cos(edge.ea);
    edge.et.y = edge.ct.y + abs_r + sin(edge.ea);

    return edge;
}

int cal_snap_45_strategy_1(QPointF& st, QPointF mp, QPointF& sp)
{
    if (fabs(st.x() - mp.x()) < 1E-12 || fabs(st.y() - mp.y()) < 1E-12)
        return 0;
    QPointF vec_p1_p2 = mp - st;
    double abs_x = fabs(vec_p1_p2.x());
    double abs_y = fabs(vec_p1_p2.y());

    QPointF temp;

    // 斜45度
    if (fabs(abs_x - abs_y) < 1E-12)
        return 0;

    if (abs_x > abs_y)
    {  // 往x轴吸附
        temp.setX(mp.x());
        temp.setY(st.y());
    }
    else
    {  // 往y轴吸附
        temp.setX(st.x());
        temp.setY(mp.y());
    }

    QPointF vec_new = st - temp;
    double len = sqrt(vec_new.y() * vec_new.y() + vec_new.x() * vec_new.x());
    vec_new = vec_new / len;

    double dist = sqrt((mp.y() - temp.y()) * (mp.y() - temp.y()) + (mp.x() - temp.x()) * (mp.x() - temp.x()));

    sp = temp + vec_new * dist;
    return 1;
}
int cal_snap_45_strategy_2(QPointF& st, QPointF mp, QPointF& sp)
{
    return cal_snap_45_strategy_1(mp, st, sp);
}
int cal_snap_45(double angle, int inclined, QPointF& p1, QPointF& p2, QPointF& res)
{
    if (angle == 0.0)
        return cal_snap_45_strategy_1(p1, p2, res);

    if (((angle > 0) && (angle < M_PI_4)) || ((angle > M_PI_2) && (angle < (M_PI_2 + M_PI_4))))
    {
        if (inclined)
            return cal_snap_45_strategy_2(p1, p2, res);
        else
            return cal_snap_45_strategy_1(p1, p2, res);
    }
    else
    {
        if (inclined)
            return cal_snap_45_strategy_1(p1, p2, res);
        else
            return cal_snap_45_strategy_2(p1, p2, res);
    }
}

QPainterPath polygon_to_painter_path(const redcgl::Polygon& polygon)
{
    QPainterPath path;
    const redcgl::Polygon* curr_poly = &polygon;
    if (curr_poly == nullptr || curr_poly->edges == nullptr)
        return path;

    redcgl::Edge* first_edge = &curr_poly->edges[0];
    path.moveTo(first_edge->st.x, first_edge->st.y);  // 路径起点

    // TODO: 支持多个多边形以及孔洞
    while (curr_poly != nullptr)  // 遍历多边形
    {
        for (int i = 1; i < curr_poly->num_edges; ++i)  // 外轮廓
        {
            redcgl::Edge* curr_edge = &curr_poly->edges[i];
            if (curr_edge->r == 0.0)  // 线段
            {
                path.lineTo(curr_edge->st.x, curr_edge->st.y);
            }
            else
            {  // 圆弧
                QtArc qt_arc = edge_to_qt_arc(*curr_edge);
                path.arcTo(qt_arc._rect, qt_arc._start_angle, qt_arc._span_angle);
            }
        }
        path.closeSubpath();
        curr_poly = curr_poly->next;
    }
    return path;
}