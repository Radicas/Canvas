#include "graphicsarcitem.h"
#include "redcgl/include/redcgl.h"

GraphicsArcItem::GraphicsArcItem() {}

GraphicsArcItem::GraphicsArcItem(const QRectF& rect, int startAngle, int spanAngle, QGraphicsItem* parent)
    : QGraphicsItem(parent), _rect(rect), _start_angle(startAngle), _span_angle(spanAngle), _pen(Qt::black)
{
}

QRectF GraphicsArcItem::boundingRect() const
{
    return _rect;
}

void GraphicsArcItem::setPen(const QPen& pen)
{
    _pen = pen;
    update();
}

void GraphicsArcItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(_pen);
    painter->drawArc(_rect, _start_angle, _span_angle);
}

void GraphicsArcItem::setRect(const QRectF& rect)
{
    _rect = rect;
}

void GraphicsArcItem::setStartAngle(int angle)
{
    _start_angle = angle;
}

void GraphicsArcItem::setSpanAngle(int angle)
{
    _span_angle = angle;
}

QRectF GraphicsArcItem::getRect()
{
    return _rect;
}
int GraphicsArcItem::getStartAngle() const
{
    return _start_angle;
}
int GraphicsArcItem::getSpanAngle() const
{
    return _span_angle;
}

inline int toQtAngle(double radian)
{
    return static_cast<int>(radian * 180.0 / M_PI * 16);
}

void GraphicsArcItem::generate_arc(const QPointF& p1, const QPointF& p2, const QPointF& p3)
{
    double center_x, center_y, radius;
    redcgl::three_points_to_circle(p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y(), &center_x, &center_y, &radius);
    _rect.setRect(center_x - radius, center_y - radius, 2 * radius, 2 * radius);
    double start_angle = atan2(-(p1.y() - center_y), p1.x() - center_x);
    double end_angle = atan2(-(p2.y() - center_y), p2.x() - center_x);
    if (start_angle < 0)
        start_angle += 2 * M_PI;

    if (end_angle < 0)
        end_angle += 2 * M_PI;

    QPointF vec_s_to_e = p2 - p1;
    QPointF vec_s_to_p = p3 - p1;
    double cross = redcgl::cross_product(vec_s_to_e.x(), vec_s_to_e.y(), vec_s_to_p.x(), vec_s_to_p.y());
    int ccw = 0;
    if (cross > 0)
        ccw = 1;

    redcgl::Edge e;
    e.r = ccw ? -1 : 1;
    e.sa = start_angle;
    e.ea = end_angle;
    double span_angle = redcgl::sweep_angle(&e);

    _start_angle = (redcgl::rad_to_degree(start_angle)) * 16;
    end_angle = (redcgl::rad_to_degree(end_angle)) * 16;
    span_angle = redcgl::rad_to_degree(span_angle) * 16;
    _span_angle = ccw ? (int)span_angle : -(int)span_angle;

    printf("start angle: %lf\t end angle : %lf\t span angle : %lf\n", _start_angle / 16.0, end_angle / 16.0,
           _span_angle / 16.0);
}