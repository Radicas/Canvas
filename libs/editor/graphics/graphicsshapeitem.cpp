#include "graphicsshapeitem.h"
#include "convertor.h"

GraphicsShapeItem::GraphicsShapeItem(F_POLYHEAD* poly_head)
    : _pen(Qt::red, 0), _brush(Qt::green), _rect(), _path(), _poly_head(poly_head)
{
    init_painter_path();
    _rect = _path.boundingRect();
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
F_POLYHEAD* GraphicsShapeItem::get_poly_head()
{
    return _poly_head;
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
QPainterPath GraphicsShapeItem::poly_head_to_path(F_POLYHEAD* poly_head)
{
    QPainterPath path;
    // 遍历所有的多边形
    while (poly_head)
    {
        F_POLYELEM* curr_elem = poly_head->APoint;  // 当前元素
        F_POLYELEM* first_elem = curr_elem;         // 第一个元素
        if (first_elem)
            path.moveTo(first_elem->x, first_elem->y);  // 设置路径起点
        else
            return {};  // 没有元素，返回空路径
        do
        {
            F_POLYELEM* next = curr_elem->Forwards;  // 作为线段或圆弧终点
            if (curr_elem->radius == 0.0)            // 线段
                path.lineTo(next->x, next->y);      // 绘制到下一个元素
            else                                     // 圆弧
            {
                redcgl::Point start{curr_elem->x, curr_elem->y};
                redcgl::Point end{next->x, next->y};
                redcgl::Point center{curr_elem->cx, curr_elem->cy};
                double start_angle, end_angle;
                redcgl::calculate_angle_rad(&start, &center, &start_angle);
                redcgl::calculate_angle_rad(&end, &center, &end_angle);

                double cross = redcgl::cross_product(curr_elem->x - curr_elem->cx, curr_elem->y - curr_elem->cy,
                                                     next->x - next->cx, next->y - next->cy);

                double span_angle = 0.0;  // 旋转角度，给qt画arc用

                if (cross < 0)  //  顺时针，外轮廓
                {
                    // 起始角度始终大于终止角度
                    double tmp_s = start_angle == 0.0 ? start_angle + 2 * M_PI : start_angle;
                    span_angle = tmp_s - end_angle;  // 计算出旋转角
                }
                else if (cross > 0)  // 逆时针，内轮廓
                {
                    // 终止角度始终大于起始角度
                    double tmp_e = end_angle == 0.0 ? end_angle + 2 * M_PI : end_angle;
                    span_angle = -(tmp_e - start_angle);  // 计算出旋转角
                }

                double abs_r = fabs(curr_elem->radius);
                QRectF rect(center.x - abs_r, center.y - abs_r, 2 * abs_r, 2 * abs_r);
                path.arcTo(rect, start_angle, span_angle);
            }
            curr_elem = next;  // 移动到下一个元素
        } while (curr_elem && curr_elem != first_elem);

        path.closeSubpath();
        // 处理孔洞
        if (poly_head->NextHole)
        {
            path.addPath(poly_head_to_path(poly_head->NextHole));
        }

        poly_head = poly_head->next;  // 移到下一个多边形
    }
    return path;
}
#include <qdebug.h>
int GraphicsShapeItem::init_painter_path()
{
    if (_poly_head == nullptr)
        return -1;
    auto* curr_head = _poly_head;
    _path = poly_head_to_path(curr_head);
    qDebug() << _path;
    return 1;
}
