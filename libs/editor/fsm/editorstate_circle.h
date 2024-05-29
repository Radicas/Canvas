//
// Created by Radica on 2024/5/19.
//

#ifndef CANVAS_EDITORSTATE_CIRCLE_H
#define CANVAS_EDITORSTATE_CIRCLE_H

#include "core/context.h"
#include "core/grid.h"
#include "editorstate.h"
#include "graphics/graphicsarcitem.h"
#include "graphics/graphicsscene.h"
#include "graphics/graphicsshapeitem.h"
#include "redcgl/include/redcgl.h"

#include <QAction>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPen>
#include <QPointF>

class Editorstate_Circle : public EditorState
{
   public:
    enum class STATE
    {
        IDLE = 0,
        ADD_RECT,
    };

    Editorstate_Circle(const Context& context);
    ~Editorstate_Circle();

    void entry() override;
    void exit() override;

    int mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseLeftPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseRightPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

   protected:
    void setup();
    void add_circle();
    void clear_cache();

   private:
    QPointF _last_point{};                           // 上次点击的坐标
    QPointF _cursor_point{};                         // 当前点击的坐标
    STATE _state;                                    // 操作状态
    QGraphicsEllipseItem* _preview;                  // 预览矩形
    double _radius;                                  // 圆形半径
    std::vector<redcgl::Edge> _shape_edges{};        // shape的边
    std::vector<GraphicsShapeItem*> _shape_items{};  // 已有shape绘制对象
    QPen _pen;
    QBrush _brush;
};

Editorstate_Circle::Editorstate_Circle(const Context& context) : EditorState(context)
{
    setup();
}
Editorstate_Circle::~Editorstate_Circle() {}
void Editorstate_Circle::entry()
{
    printf("enter draw rect\n");
    EditorState::entry();
}
void Editorstate_Circle::exit()
{
    printf("exit draw rect\n");
    EditorState::exit();
}
int Editorstate_Circle::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    Grid g;
    Point p = event->scenePos().toPoint();
    g.map_to_grid(p);
    _cursor_point = QPointF(p);
    switch (_state)
    {
        case STATE::IDLE:
            break;
        case STATE::ADD_RECT:
        {
            _radius = redcgl::dist_pt_vs_pt(_last_point.x(), _last_point.y(), _cursor_point.x(), _cursor_point.y());
            QRectF rect(_last_point.x() - _radius, _last_point.y() - _radius, 2 * _radius, 2 * _radius);
            _preview->setRect(rect);
            _preview->setStartAngle(0);
            _preview->setSpanAngle(360 * 16);
            _preview->update();
        }
        break;
    }
    return EditorState::mouseMoveEvent(event);
}
int Editorstate_Circle::mouseLeftPressEvent(QGraphicsSceneMouseEvent* event)
{
    Grid g;
    Point p = event->scenePos().toPoint();
    g.map_to_grid(p);
    _cursor_point = QPointF(p);
    switch (_state)
    {
        case STATE::IDLE:
        {
            _last_point = _cursor_point;
            _context.get_scene()->addItem(_preview);
            _state = STATE::ADD_RECT;
            break;
        }
        case STATE::ADD_RECT:
        {
            add_circle();
            clear_cache();
            _state = STATE::IDLE;
            break;
        }
        default:
            break;
    }
    return EditorState::mouseLeftPressEvent(event);
}
int Editorstate_Circle::mouseRightPressEvent(QGraphicsSceneMouseEvent* event)
{
    return EditorState::mouseRightPressEvent(event);
}
int Editorstate_Circle::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    return EditorState::mouseDoubleClickEvent(event);
}
int Editorstate_Circle::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    return EditorState::mouseReleaseEvent(event);
}

void Editorstate_Circle::setup()
{
    _pen = {Qt::red, 0};
    _brush = {Qt::green};
    _preview = new QGraphicsEllipseItem();
    _preview->setPen(_pen);
    _preview->setBrush(_brush);
}
void Editorstate_Circle::add_circle()
{

}

void Editorstate_Circle::clear_cache()
{
    _context.get_scene()->removeItem(_preview);
    _shape_edges.clear();
}
#endif  //CANVAS_EDITORSTATE_CIRCLE_H
