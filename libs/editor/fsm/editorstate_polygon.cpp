#include "editorstate_polygon.h"
#include "core/context.h"
#include "core/grid.h"
#include "graphics/graphicsscene.h"

#include <QGraphicsSceneMouseEvent>

Editorstate_Polygon::Editorstate_Polygon(const Context& context) : EditorState(context) {}

Editorstate_Polygon::~Editorstate_Polygon() {}

void Editorstate_Polygon::entry() {}

void Editorstate_Polygon::exit() {}

int Editorstate_Polygon::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    Grid g;
    Point p = event->scenePos().toPoint();
    g.map_to_grid(p);
    _cursor_point = QPointF(p);
    switch (_state)
    {
        case IDLE:
        {
            break;
        }
        case ADD_EDGE:
        {
            // 鼠标点击到上一个点时直接跳过
            if (_cursor_point == _last_point)
                return true;
            QPointF new_point;
            // 获取上一条已经创建的线段
            double angle = 0.0;
            int is_inclined = 0;
            if (!_preview_lines.empty())
            {  // 第二次以及后续生成线段
                auto* last = _preview_lines.last();
                QPointF p1 = last->line().p1();
                QPointF p2 = last->line().p2();
                // 上一条线段向量
                QPointF vec = p2 - p1;
                // 上一个点到当前鼠标处的向量
                QPointF vec2 = _cursor_point - _last_point;
                // 上条线段是否是斜的
                is_inclined = (vec.x() != 0.0 && vec.y() != 0.0);

                // 计算角度，根据角度选择策略
                double dot = dot_product(vec.x(), vec.y(), vec2.x(), vec2.y());
                double vec_mag = vec_magnitude(vec.x(), vec.y());
                double vec_2_mag = vec_magnitude(vec2.x(), vec2.y());
                angle = acos(dot / (fabs(vec_mag) * fabs(vec_2_mag)));
            }
            // 计算目标点
            int has_two = cal_snap_45(angle, is_inclined, _last_point, _cursor_point, new_point);
            if (has_two)
            {
                update_line_item(_preview[0], _last_point, new_point);
                update_line_item(_preview[1], new_point, _cursor_point);
                save_two = 1;
            }
            else
            {
                QPointF empty{};
                update_line_item(_preview[0], _last_point, _cursor_point);
                update_line_item(_preview[1], empty, empty);
                save_two = 0;
            }
            break;
        }
        case ADD_POLYGON:
        {
            break;
        }
        default:
            break;
    }

    return true;

    return 0;
}

int Editorstate_Polygon::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QPolygonF poly;
    poly << QPointF(10000, 10000) << QPointF(10000, -10000) << QPointF(-10000, -10000) << QPointF(-10000, 10000);
    auto* poly_item = new QGraphicsPolygonItem();
    poly_item->setPolygon(poly);
    poly_item->setPen(QPen(Qt::red, 1));
    poly_item->setBrush(Qt::blue);
    poly_item->update();
    _context.get_scene()->addItem(poly_item);

    printf("draw polygon press\n");

    Grid g;
    Point p = event->scenePos().toPoint();
    g.map_to_grid(p);
    _last_point = _cursor_point = QPointF(p);

    switch (_state)
    {
        case IDLE:
        {
            // 第一次点击时，创建两个预览线
            for (auto& i : _preview)
            {
                i = new QGraphicsLineItem();
                update_line_item(i, _cursor_point, _cursor_point);
                _context.get_scene()->addItem(i);
            }
            _state = ADD_EDGE;
            break;
        }
        case ADD_EDGE:
        {
            // 点击后存储已有线段
            if (save_two == 1)
            {
                _preview_lines.append(_preview[0]);
                _preview_lines.append(_preview[1]);
            }
            else
            {
                _preview_lines.append(_preview[0]);
            }

            // 检查是否闭合
            // TODO: 检查degeneracy
            if (check_closed())
            {
                _state = ADD_POLYGON;
                return true;
            }

            // 同时生成新线段
            for (auto& i : _preview)
            {
                i = new QGraphicsLineItem();
                update_line_item(i, _cursor_point, _cursor_point);
                _context.get_scene()->addItem(i);
            }
            break;
        }
        case ADD_POLYGON:
        {
            _state = ADD_EDGE;
            break;
        }
        default:
            break;
    }
    return true;
    return 0;
}

int Editorstate_Polygon::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    printf("draw polygon release\n");
    return 0;
}
