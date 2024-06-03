#ifndef CANVAS_EDITORSTATE_POLYGON_H
#define CANVAS_EDITORSTATE_POLYGON_H

#include "convertor.h"
#include "core/context.h"
#include "core/grid.h"
#include "editorstate.h"
#include "graphics/graphicsarcitem.h"
#include "graphics/graphicsscene.h"
#include "graphics/graphicsshapeitem.h"
#include "redcgl/include/curve.h"
#include "redcgl/include/vector.h"

#include <QAction>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPen>
#include <QPointF>

#ifdef WIN32
#include <cmath>
#endif

//#define DEBUG_PREVIEW

class Editorstate_Polygon : public EditorState
{
   public:
    enum class STATE
    {
        IDLE = 0,
        ADD_EDGE,
        ADD_ARC,
    };

    Editorstate_Polygon(const Context& context);
    ~Editorstate_Polygon();
    void entry() override;
    void exit() override;
    int mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseLeftPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseRightPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseMoveSegEvent(QGraphicsSceneMouseEvent* event);
    int mouseLeftPressSegEvent(QGraphicsSceneMouseEvent* event);
    int mouseMoveArcEvent(QGraphicsSceneMouseEvent* event);
    int mouseLeftPressArcEvent(QGraphicsSceneMouseEvent* event);

   protected:
    void setup();
    void add_menus();
    void init_poly_head();
    QMenu* _menu;  // 右键菜单

   private:
    std::vector<GraphicsShapeItem*> _shape_items{};               // 已有shape绘制对象
    std::vector<QGraphicsLineItem*> _preview_lines{};             // 已有的预览线段对象
    static const int _preview_temp_seg_cnt = 2;                   // 预览线数量
    QGraphicsLineItem* _preview_temp_seg[_preview_temp_seg_cnt];  // 新预览线段对象
    QGraphicsLineItem* _preview_temp_arc_seg{};                   // 圆弧预览线段
    GraphicsArcItem* _preview_temp_arc{};                         // 圆弧预览
    std::vector<GraphicsArcItem*> _preview_temp_arcs{};           // 已有圆弧预览
    QPointF _last_point{};                                        // 上次点击的坐标
    QPointF _cursor_point{};                                      // 当前点击的坐标
    QPointF _arc_start{};                                         // 圆弧起点
    STATE _state;                                                 // 操作状态
    int save_two = 0;                                             // 是否保存两根预览线
    int add_arc_edge = 0;                                         // 是否添加圆弧边
    int _temp_rect_cnt = 5;                                       // 辅助框数量
    QGraphicsRectItem* _temp_rect[5];                             // 辅助框

    void init_previews();
    int check_closed();
    void update_line_item(QGraphicsLineItem* l, QPointF& p1, QPointF& p2);
    int add_graphics_polygon();
    int clear_preview_lines();
    redcgl::Edge* qt_arc_to_edge(const QtArc& arc);
    int cal_snap_45_strategy_1(QPointF& st, QPointF mp, QPointF& sp);
    int cal_snap_45_strategy_2(QPointF& st, QPointF mp, QPointF& sp);
    int cal_snap_45(double angle, int inclined, QPointF& p1, QPointF& p2, QPointF& res);
};

Editorstate_Polygon::Editorstate_Polygon(const Context& context) : EditorState(context), _menu(new QMenu())
{
    setup();
}
Editorstate_Polygon::~Editorstate_Polygon() {}
void Editorstate_Polygon::setup()
{
    init_poly_head();
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
                    _state = STATE::ADD_EDGE;
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
void Editorstate_Polygon::init_poly_head()
{
    // 创建一个空头
    // _poly_head = (POLYGON_H*)malloc(sizeof(POLYGON_H));
    // _poly_head->APoint = nullptr;
    // _poly_head->next = nullptr;
    // _poly_head->NextHole = nullptr;
    // _poly_head->mallocd_not_pooled = 0;
    // _poly_head->width = 0;
    // _poly_head->flag = 0;
    // _poly_head->pointer = nullptr;
    // _poly_head->userData = nullptr;
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
        case STATE::IDLE:
        {
            break;
        }
        case STATE::ADD_EDGE:
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
                double dot = redcgl::dot_product_v(vec.x(), vec.y(), vec2.x(), vec2.y());
                double vec_mag = redcgl::vec_magnitude_v(vec.x(), vec.y());
                double vec_2_mag = redcgl::vec_magnitude_v(vec2.x(), vec2.y());
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
        case STATE::IDLE:
        {
            // 添加预览线段，显示
            for (auto& i : _preview_temp_seg)
            {
                _context.get_scene()->addItem(i);
            }

            // 第一次点击，创建一个顶点元素
            // F_POLYELEM* elem = redcgl::build_poly_elem(_cursor_point.x(), _cursor_point.y(), 0, 0, 0);
            // elem->PToHead = _poly_head;
            // _poly_head->APoint = elem;
            _state = STATE::ADD_EDGE;
            break;
        }
        case STATE::ADD_EDGE:
        {
            // 点击后存储已有线段
            _preview_lines.emplace_back(_preview_temp_seg[0]);
            if (save_two == 1)
                _preview_lines.emplace_back(_preview_temp_seg[1]);

            // 创建一个元素
            // F_POLYELEM* elem1 = redcgl::build_poly_elem(_preview_temp_seg[1]->line().p1().x(),
            //                                             _preview_temp_seg[1]->line().p1().y(), 0, 0, 0);
            // elem1->PToHead = _poly_head;

            if (save_two == 1)
            {
                // F_POLYELEM* elem2 = redcgl::build_poly_elem(_preview_temp_seg[1]->line().p2().x(),
                //                                             _preview_temp_seg[1]->line().p2().y(), 0, 0, 0);
                // elem2->PToHead = _poly_head;
                // elem1->Forwards = elem2;
                // elem2->Backwards = elem1;
            }

            // 注意，添加的元素如果切换到圆弧了，需要去除
            // F_POLYELEM* elem_head = _poly_head->APoint;
            // F_POLYELEM* curr_elem = elem_head;
            // while (curr_elem->Forwards != nullptr)
            // {
            //     curr_elem = curr_elem->Forwards;
            // }
            // curr_elem->Forwards = elem1;
            // elem1->Backwards = curr_elem;

            // 检查是否闭合
            if (check_closed())
            {
                // 形成闭环
                // elem_head->Backwards = elem1;
                // elem1->Forwards = elem_head;
                // 创建多边形
                add_graphics_polygon();
                // 去除预览线
                clear_preview_lines();
                _state = STATE::IDLE;
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
        case STATE::IDLE:
            break;
        case STATE::ADD_EDGE:
        {
            // 鼠标点击到上一个点时直接跳过
            if (_cursor_point == _last_point)
                return true;

            // 更新辅助线段
            update_line_item(_preview_temp_arc_seg, _last_point, _cursor_point);

            break;
        }
        case STATE::ADD_ARC:
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
        case STATE::IDLE:
            break;
        case STATE::ADD_EDGE:
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
        case STATE::ADD_ARC:
        {
            // save arc
            QtArc qt_arc;
            qt_arc._rect = _preview_temp_arc->getRect();
            qt_arc._start_angle = _preview_temp_arc->getStartAngle();
            qt_arc._span_angle = _preview_temp_arc->getSpanAngle();

            // TODO: 修改poly_elem最后一个元素为圆弧,或者去除
            // TODO: 需要一个qt_arc转poly_elem的接口

            //            qt_arc_to_poly_elems(qt_arc);

            _preview_temp_arcs.emplace_back(_preview_temp_arc);  // 缓存起来，最后要移除
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
    GraphicsShapeItem* shape_item = new GraphicsShapeItem();
    _context.get_scene()->addItem(shape_item);
    shape_item->update();
    return 0;
}
int Editorstate_Polygon::clear_preview_lines()
{
    // 移除线段预览
    for (auto* line : _preview_lines)
    {
        _context.get_scene()->removeItem(line);
    }
    _preview_lines.clear();

    // 移除圆弧预览
    for (auto* arc : _preview_temp_arcs)
    {
        _context.get_scene()->removeItem(arc);
    }
    _preview_temp_arcs.clear();
    return 0;
}
redcgl::Edge* Editorstate_Polygon::qt_arc_to_edge(const QtArc& arc)
{
    // 可以作为中转，保留此接口，或许还可以用上
    double center_x, center_y, radius;
    auto& rect = arc._rect;
    center_x = rect.x() + rect.width() / 2.0;
    center_y = rect.y() + rect.height() / 2.0;
    radius = rect.width() / 2.0;

    double sx, sy, ex, ey, cx, cy, r, sa, ea;

    cx = center_x;
    cy = center_y;
    int ccw = arc._span_angle > 0 ? 1 : 0;
    r = ccw ? -radius : radius;
    sa = redcgl::degree_to_rad(arc._start_angle / 16.0);
    double span_angle = redcgl::degree_to_rad((arc._span_angle / 16.0));
    double tea = sa + span_angle;
    ea = fmod(tea, 2 * M_PI);

    double abs_r = fabs(r);
    sx = cx + abs_r + cos(sa);
    sy = cy + abs_r + sin(sa);
    ex = cx + abs_r + cos(ea);
    ey = cy + abs_r + sin(ea);

    redcgl::Edge* edge = redcgl::create_edge(sx, sy, ex, ey, cx, cy, r, sa, ea);
    return edge;
}
int Editorstate_Polygon::cal_snap_45_strategy_1(QPointF& st, QPointF mp, QPointF& sp)
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
int Editorstate_Polygon::cal_snap_45_strategy_2(QPointF& st, QPointF mp, QPointF& sp)
{
    return cal_snap_45_strategy_1(mp, st, sp);
}
int Editorstate_Polygon::cal_snap_45(double angle, int inclined, QPointF& p1, QPointF& p2, QPointF& res)
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

#endif  //CANVAS_EDITORSTATE_POLYGON_H
