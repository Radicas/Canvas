#include "editorstate_polygon.h"
#include "logic/polygon_shape_logic.h"

Editorstate_Polygon::Editorstate_Polygon(const Context& context) : EditorState(context), _menu(new QMenu())
{
    setup();
}
Editorstate_Polygon::~Editorstate_Polygon() {}
void Editorstate_Polygon::setup()
{
    add_menus();
    init_previews();
}
void Editorstate_Polygon::add_menus()
{
    auto* arc_action = new QAction("Add Arc");
    _menu->addAction(arc_action);
    connect(arc_action, &QAction::triggered, this,
            [arc_action, this]()
            {
                if (add_arc_edge)
                {
                    arc_action->setText("Add Arc");
                    // 进入这里时，如果预览线段存在，需要将预览线显示
                    for (int i = 0; i < _preview_temp_seg_cnt; ++i)
                    {
                        if (_preview_temp_seg[i])
                        {
                            _context.get_scene()->addItem(_preview_temp_seg[i]);
                        }
                    }
                    _context.get_scene()->removeItem(_preview_temp_arc_seg);
                    _context.get_scene()->removeItem(_preview_temp_arc);
                    add_arc_edge = 0;
                    _state = ADD_EDGE;
                }
                else
                {
                    arc_action->setText("Add Seg");
                    // 进入这里时，如果预览线段存在，需要将预览线移除
                    for (int i = 0; i < _preview_temp_seg_cnt; ++i)
                    {
                        if (_preview_temp_seg[i])
                        {
                            _context.get_scene()->removeItem(_preview_temp_seg[i]);
                        }
                    }
                    _context.get_scene()->addItem(_preview_temp_arc_seg);
                    _context.get_scene()->addItem(_preview_temp_arc);
                    add_arc_edge = 1;
                }
            });
}
void Editorstate_Polygon::entry()
{
    printf("enter draw polygon\n");
}
void Editorstate_Polygon::exit()
{
    printf("exit draw polygon\n");
}
int Editorstate_Polygon::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (add_arc_edge)
        return mouseMoveArcEvent(event);
    else
        return mouseMoveSegEvent(event);
}
int Editorstate_Polygon::mouseLeftPressEvent(QGraphicsSceneMouseEvent* event)
{
    if (add_arc_edge)
        return mouseLeftPressArcEvent(event);
    else
        return mouseLeftPressSegEvent(event);
}
int Editorstate_Polygon::mouseRightPressEvent(QGraphicsSceneMouseEvent* event)
{
    QPoint p = event->screenPos();
    _menu->exec(p);
    return 0;
}
int Editorstate_Polygon::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    printf("draw polygon right press\n");
    return 0;
}
int Editorstate_Polygon::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    printf("draw polygon release\n");
    return 0;
}
int Editorstate_Polygon::mouseMoveSegEvent(QGraphicsSceneMouseEvent* event)
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
            QPointF new_point;    // 45度拐角处的点
            double angle = 0.0;   // 鼠标到线段起点的角度
            int is_inclined = 0;  // 是否倾斜
            if (!_preview_lines.empty())
            {  // 第二次以及后续生成线段
                auto* last = _preview_lines.back();
                QPointF p1 = last->line().p1();
                QPointF p2 = last->line().p2();
                // 上一条线段向量
                QPointF vec = p2 - p1;
                // 上一个点到当前鼠标处的向量
                QPointF vec2 = _cursor_point - _last_point;
                // 上条线段是否是斜的
                is_inclined = (vec.x() != 0.0 && vec.y() != 0.0);

                // 计算角度，根据角度选择策略
                double dot = redcgl::dot_product(vec.x(), vec.y(), vec2.x(), vec2.y());
                double vec_mag = redcgl::vec_magnitude(vec.x(), vec.y());
                double vec_2_mag = redcgl::vec_magnitude(vec2.x(), vec2.y());
                angle = acos(dot / (fabs(vec_mag) * fabs(vec_2_mag)));
            }
            // 计算目标点
            save_two = cal_snap_45(angle, is_inclined, _last_point, _cursor_point, new_point);
            if (save_two)
            {
                update_line_item(_preview_temp_seg[0], _last_point, new_point);
                update_line_item(_preview_temp_seg[1], new_point, _cursor_point);
            }
            else
            {
                QPointF empty{};
                update_line_item(_preview_temp_seg[0], _last_point, _cursor_point);
                update_line_item(_preview_temp_seg[1], empty, empty);
            }
            break;
        }
        default:
            break;
    }

    return 0;
}
int Editorstate_Polygon::mouseLeftPressSegEvent(QGraphicsSceneMouseEvent* event)
{
    printf("draw polygon left press\n");
    Grid g;
    Point p = event->scenePos().toPoint();
    g.map_to_grid(p);
    _last_point = _cursor_point = QPointF(p);

    switch (_state)
    {
        case IDLE:
        {
            // 添加预览线段，显示
            for (auto& i : _preview_temp_seg)
            {
                _context.get_scene()->addItem(i);
            }
            _state = ADD_EDGE;
            break;
        }
        case ADD_EDGE:
        {
            // 点击后存储已有线段
            _preview_lines.emplace_back(_preview_temp_seg[0]);
            if (save_two == 1)
                _preview_lines.emplace_back(_preview_temp_seg[1]);

            // 存储shape边
            redcgl::Edge edge1;
            redcgl::empty_edge(&edge1);
            auto edge1_p1 = _preview_temp_seg[0]->line().p1();
            auto edge1_p2 = _preview_temp_seg[0]->line().p2();
            edge1.st = (redcgl::Point){edge1_p1.x(), edge1_p1.y()};
            edge1.et = (redcgl::Point){edge1_p2.x(), edge1_p2.y()};
            _shape_edges.emplace_back(edge1);
            if (save_two == 1)
            {
                redcgl::Edge edge2;
                redcgl::empty_edge(&edge2);
                auto edge2_p1 = _preview_temp_seg[1]->line().p1();
                auto edge2_p2 = _preview_temp_seg[1]->line().p2();
                edge2.st = (redcgl::Point){edge2_p1.x(), edge2_p1.y()};
                edge2.et = (redcgl::Point){edge2_p2.x(), edge2_p2.y()};
                _shape_edges.emplace_back(edge2);
            }

            // 检查是否闭合
            if (check_closed())
            {
                // 创建多边形
                add_graphics_polygon();
                // 去除预览线
                clear_preview_lines();
                _state = IDLE;
                return 0;
            }

            // 同时生成新线段
            for (auto& i : _preview_temp_seg)
            {
                i = new QGraphicsLineItem();
                update_line_item(i, _cursor_point, _cursor_point);
                i->setPen(QPen(Qt::red, 0));
                _context.get_scene()->addItem(i);
            }
            break;
        }
        default:
            break;
    }
    return 0;
}
int Editorstate_Polygon::mouseMoveArcEvent(QGraphicsSceneMouseEvent* event)
{
    Grid g;
    Point p = event->scenePos().toPoint();
    g.map_to_grid(p);
    _cursor_point = QPointF(p);
    switch (_state)
    {
        case IDLE:
            break;
        case ADD_EDGE:
        {
            // 鼠标点击到上一个点时直接跳过
            if (_cursor_point == _last_point)
                return true;

            // 更新辅助线段
            update_line_item(_preview_temp_arc_seg, _last_point, _cursor_point);

            break;
        }
        case ADD_ARC:
        {

#ifdef DEBUG_PREVIEW
            printf("arc start : (%lf, %lf)\t", _arc_start.x(), _arc_start.y());
            printf("last point : (%lf, %lf)\t", _last_point.x(), _last_point.y());
            printf("cursor point : (%lf, %lf)\n", _cursor_point.x(), _cursor_point.y());

            QRectF rect;
            rect.setRect(_arc_start.x() - 100000, _arc_start.y() - 100000, 200000, 200000);
            _temp_rect[0]->setRect(rect);
            _temp_rect[0]->update();

            rect.setRect(_last_point.x() - 100000, _last_point.y() - 100000, 200000, 200000);
            _temp_rect[1]->setRect(rect);
            _temp_rect[1]->update();

            rect.setRect(_cursor_point.x() - 100000, _cursor_point.y() - 100000, 200000, 200000);
            _temp_rect[2]->setRect(rect);
            _temp_rect[2]->update();

            double center_x, center_y, radius;
            redcgl::three_points_to_circle(_arc_start.x(), _arc_start.y(), _last_point.x(), _last_point.y(),
                                           _cursor_point.x(), _cursor_point.y(), &center_x, &center_y, &radius);
            rect.setRect(center_x - 100000, center_y - 100000, 200000, 200000);
            _temp_rect[3]->setRect(rect);
            _temp_rect[3]->update();
#endif
            _preview_temp_arc->generate_arc(_arc_start, _last_point, _cursor_point);
            _preview_temp_arc->update();
            break;
        }
        default:
            break;
    }
    return 0;
}
int Editorstate_Polygon::mouseLeftPressArcEvent(QGraphicsSceneMouseEvent* event)
{
    Grid g;
    Point p = event->scenePos().toPoint();
    g.map_to_grid(p);
    _cursor_point = QPointF(p);
    _arc_start = _last_point;
    switch (_state)
    {
        case IDLE:
            break;
        case ADD_EDGE:
        {
            // 移除辅助线段
            _context.get_scene()->removeItem(_preview_temp_arc_seg);
            _last_point = _cursor_point;

            // 添加辅助定位矩形
            for (int i = 0; i < _temp_rect_cnt; ++i)
            {
                _context.get_scene()->addItem(_temp_rect[i]);
            }
            _state = STATE::ADD_ARC;
            break;
        }
        case ADD_ARC:
        {
            // save arc
            QtArc qt_arc;
            qt_arc._rect = _preview_temp_arc->getRect();
            qt_arc._start_angle = _preview_temp_arc->getStartAngle();
            qt_arc._span_angle = _preview_temp_arc->getSpanAngle();

            redcgl::Edge edge = qt_arc_to_edge(qt_arc);
            _shape_edges.emplace_back(edge);

            _preview_temp_arc = new GraphicsArcItem();
            _preview_temp_arc->setPen(QPen(Qt::red, 0));
            _context.get_scene()->addItem(_preview_temp_arc_seg);
            _context.get_scene()->addItem(_preview_temp_arc);

            _state = STATE::ADD_EDGE;
            break;
        }
        default:
            break;
    }
    return 0;
}
void Editorstate_Polygon::init_previews()
{
    QPen pen(Qt::red, 0);
    // 线段的预览
    for (auto& i : _preview_temp_seg)
    {
        i = new QGraphicsLineItem();
        i->setPen(pen);
    }
    // 圆弧辅助线段预览
    _preview_temp_arc_seg = new QGraphicsLineItem();
    _preview_temp_arc_seg->setPen(pen);
    // 圆弧预览
    _preview_temp_arc = new GraphicsArcItem();
    _preview_temp_arc->setPen(pen);
    // 辅助矩形预览
    for (int i = 0; i < _temp_rect_cnt; ++i)
    {
        _temp_rect[i] = new QGraphicsRectItem();
        _temp_rect[i]->setPen(pen);
        _temp_rect[i]->setBrush(Qt::NoBrush);
    }
}
void Editorstate_Polygon::update_line_item(QGraphicsLineItem* l, QPointF& p1, QPointF& p2)
{
    QLineF line;
    line.setP1(p1);
    line.setP2(p2);
    if (p1 != p2)
        l->setLine(line);
    else
        l->setLine(QLineF());
    l->update();
}
int Editorstate_Polygon::check_closed()
{
    // TODO: 完善
    if (_preview_lines.size() < 3)
        return 0;
    auto last_p2 = _preview_lines.back()->line().p2();
    auto first_p1 = _preview_lines[0]->line().p1();
    return (fabs(last_p2.x() - first_p1.x()) < 1E-12 && fabs(last_p2.y() - first_p1.y()) < 1E-12);
}
int Editorstate_Polygon::add_graphics_polygon()
{
    GraphicsShapeItem* shape_item = new GraphicsShapeItem(_shape_edges);
    _shape_items.emplace_back(shape_item);
    _context.get_scene()->addItem(shape_item);
    shape_item->update();
    return 0;
}
int Editorstate_Polygon::clear_preview_lines()
{
    for (auto* line : _preview_lines)
    {
        _context.get_scene()->removeItem(line);
    }
    _preview_lines.clear();

    // 释放使用过的边
    _shape_edges.clear();

    return 0;
}