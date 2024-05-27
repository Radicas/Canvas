#ifndef CANVAS_EDITORSTATE_DIG_RECT_H
#define CANVAS_EDITORSTATE_DIG_RECT_H

#include "core/context.h"
#include "core/grid.h"
#include "editorstate.h"
#include "graphics/graphicsarcitem.h"
#include "graphics/graphicsscene.h"
#include "graphics/graphicsshapeitem.h"
#include "redcgl/include/guts.h"
#include "redcgl/include/redcgl.h"

#include <QAction>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPen>
#include <QPointF>

class Editorstate_Dig_Rect : public EditorState
{
   public:
    enum class STATE
    {
        IDLE = 0,
        START,
        DIG_HOLE,
    };

    Editorstate_Dig_Rect(const Context& context);
    ~Editorstate_Dig_Rect();

    void entry() override;
    void exit() override;

    int mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseLeftPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseRightPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

   protected:
    void setup();

   private:
    QPointF _last_point{};                         // 上次点击的坐标
    QPointF _cursor_point{};                       // 当前点击的坐标
    STATE _state;                                  // 操作状态
    GraphicsShapeItem* _selected_shape = nullptr;  // 选中的shape
    QGraphicsRectItem* _preview;                   // 预览矩形
};

void Editorstate_Dig_Rect::setup()
{
    redcgl::test();
    _preview = new QGraphicsRectItem();
    _preview->setPen(QPen(Qt::white, 0));
    _preview->setBrush(Qt::NoBrush);
}
Editorstate_Dig_Rect::Editorstate_Dig_Rect(const Context& context) : EditorState(context), _state(STATE::IDLE)
{
    setup();
}
Editorstate_Dig_Rect::~Editorstate_Dig_Rect() {}
void Editorstate_Dig_Rect::entry()
{
    printf("enter dig rect\n");
    EditorState::entry();
}
void Editorstate_Dig_Rect::exit()
{
    printf("exit dig rect\n");
    EditorState::exit();
}
int Editorstate_Dig_Rect::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    Grid g;
    Point p = event->scenePos().toPoint();
    g.map_to_grid(p);
    _cursor_point = QPointF(p);
    switch (_state)
    {
        case STATE::IDLE:
        case STATE::START:
            break;
        case STATE::DIG_HOLE:
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
int Editorstate_Dig_Rect::mouseLeftPressEvent(QGraphicsSceneMouseEvent* event)
{
    Grid g;
    Point p = event->scenePos().toPoint();
    g.map_to_grid(p);
    _cursor_point = QPointF(p);
    switch (_state)
    {
        case STATE::IDLE:
        {
            if (_selected_shape != nullptr)
                return 1;
            QList<QGraphicsItem*> items = _context.get_scene()->items(event->scenePos());
            printf("catch items size: %d\n", items.size());
            if (items.empty())
                return 1;
            auto* item = dynamic_cast<GraphicsShapeItem*>(items[0]);
            item->set_pen(QPen(Qt::blue, 0));
            item->set_brush(QBrush(Qt::blue));
            item->update();
            _state = STATE::START;
            break;
        }
        case STATE::START:
        {
            _last_point = _cursor_point;
            _context.get_scene()->addItem(_preview);
            _state = STATE::DIG_HOLE;
            break;
        }
        case STATE::DIG_HOLE:
        {
            // TODO: 需要集成logicalOp
            break;
        }
    }
    return EditorState::mouseLeftPressEvent(event);
}
int Editorstate_Dig_Rect::mouseRightPressEvent(QGraphicsSceneMouseEvent* event)
{
    return EditorState::mouseRightPressEvent(event);
}
int Editorstate_Dig_Rect::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    return EditorState::mouseDoubleClickEvent(event);
}
int Editorstate_Dig_Rect::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    return EditorState::mouseReleaseEvent(event);
}

#endif  //CANVAS_EDITORSTATE_DIG_RECT_H
