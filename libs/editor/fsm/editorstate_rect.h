#ifndef CANVAS_EDITORSTATE_RECT_H
#define CANVAS_EDITORSTATE_RECT_H

#include "core/context.h"
#include "core/grid.h"
#include "editorstate.h"
#include "graphics/graphicsarcitem.h"
#include "graphics/graphicsscene.h"
#include "graphics/graphicsshapeitem.h"
#include "redcgl/include/polygon.h"

#include <QAction>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPen>
#include <QPointF>

class Editorstate_Rect : public EditorState
{
   public:
    enum class STATE
    {
        IDLE = 0,
        ADD_RECT,
    };

    Editorstate_Rect(const Context& context);
    ~Editorstate_Rect();

    void entry() override;
    void exit() override;

    int mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseLeftPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseRightPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

   protected:
    void setup();
    void add_rect();
    void clear_cache();

   private:
    QPointF _last_point{};                           // 上次点击的坐标
    QPointF _cursor_point{};                         // 当前点击的坐标
    STATE _state;                                    // 操作状态
    QGraphicsRectItem* _preview;                     // 预览矩形
    std::vector<redcgl::Edge*> _shape_edges{};       // shape的边
    std::vector<GraphicsShapeItem*> _shape_items{};  // 已有shape绘制对象
    QPen _pen;
    QBrush _brush;
};

Editorstate_Rect::Editorstate_Rect(const Context& context) : EditorState(context)
{
    setup();
}
Editorstate_Rect::~Editorstate_Rect()
{
    if (_preview)
        delete _preview;
}
void Editorstate_Rect::entry()
{
    printf("enter draw rect\n");
    EditorState::entry();
}
void Editorstate_Rect::exit()
{
    printf("exit draw rect\n");
    EditorState::exit();
}
int Editorstate_Rect::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
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
            QRectF rect(_last_point.x(), _last_point.y(), _cursor_point.x() - _last_point.x(),
                        _cursor_point.y() - _last_point.y());
            _preview->setRect(rect);
            _preview->update();
        }
        break;
    }
    return EditorState::mouseMoveEvent(event);
}
int Editorstate_Rect::mouseLeftPressEvent(QGraphicsSceneMouseEvent* event)
{
    printf("draw rect left mouse press\n");
    Grid g;
    Point p = event->scenePos().toPoint();
    g.map_to_grid(p);
    _cursor_point = QPointF(p);
    switch (_state)
    {
        case STATE::IDLE:
        {
            _last_point = _cursor_point;
            _preview->setRect({});
            _context.get_scene()->addItem(_preview);
            _state = STATE::ADD_RECT;
            break;
        }
        case STATE::ADD_RECT:
        {
            add_rect();
            clear_cache();
            _state = STATE::IDLE;
            break;
        }
        default:
            break;
    }
    return EditorState::mouseLeftPressEvent(event);
}
int Editorstate_Rect::mouseRightPressEvent(QGraphicsSceneMouseEvent* event)
{
    return EditorState::mouseRightPressEvent(event);
}
int Editorstate_Rect::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    return EditorState::mouseDoubleClickEvent(event);
}
int Editorstate_Rect::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    return EditorState::mouseReleaseEvent(event);
}
void Editorstate_Rect::setup()
{
    _pen = {Qt::red, 0};
    _brush = {Qt::green};
    _preview = new QGraphicsRectItem();
    _preview->setPen(_pen);
    _preview->setBrush(_brush);

    _state = STATE::IDLE;
}
void Editorstate_Rect::add_rect()
{
    const auto& rect = _preview->rect();
    redcgl::Polygon* poly = qt_rect_f_to_polygon(rect);
    auto* shape_item = new GraphicsShapeItem(poly);
    _context.get_scene()->addItem(shape_item);
    shape_item->update();
}
void Editorstate_Rect::clear_cache()
{
    _context.get_scene()->removeItem(_preview);
    _shape_edges.clear();
}
#endif  //CANVAS_EDITORSTATE_RECT_H
