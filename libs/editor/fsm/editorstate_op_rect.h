#ifndef CANVAS_EDITORSTATE_OP_RECT_H
#define CANVAS_EDITORSTATE_OP_RECT_H

#include "core/context.h"
#include "core/grid.h"
#include "editorstate.h"
#include "graphics/graphicsarcitem.h"
#include "graphics/graphicsscene.h"
#include "graphics/graphicsshapeitem.h"
#include "tools/timer.h"

#include <QAction>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPen>
#include <QPointF>

class Editorstate_Op_Rect : public EditorState
{
   public:
    enum class STATE
    {
        IDLE = 0,
        START,
        DO_OP,
    };

    Editorstate_Op_Rect(const Context& context);
    ~Editorstate_Op_Rect();

    void entry() override;
    void exit() override;

    int mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseLeftPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseRightPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

   protected:
    void do_op();
    void setup();
    void add_menus();
    QMenu* _menu;  // 右键菜单

   private:
    QPointF _last_point{};                         // 上次点击的坐标
    QPointF _cursor_point{};                       // 当前点击的坐标
    STATE _state;                                  // 操作状态
    GraphicsShapeItem* _selected_shape = nullptr;  // 选中的shape
    QGraphicsRectItem* _preview;                   // 预览矩形
    int _operation;                                // 多边形布尔操作
};

void Editorstate_Op_Rect::setup()
{
    _preview = new QGraphicsRectItem();
    _preview->setPen(QPen(Qt::white, 0));
    _preview->setBrush(Qt::NoBrush);
    add_menus();
}
void Editorstate_Op_Rect::add_menus()
{
    auto* and_action = new QAction("与");
    auto* or_action = new QAction("或");
    auto* andnot_action = new QAction("与非");
    _menu->addAction(and_action);
    _menu->addAction(or_action);
    _menu->addAction(andnot_action);
    connect(and_action, &QAction::triggered, this, [and_action, this]() { _operation = 0; });
    connect(or_action, &QAction::triggered, this, [or_action, this]() { _operation = 1; });
    connect(andnot_action, &QAction::triggered, this, [andnot_action, this]() { _operation = 3; });
}
Editorstate_Op_Rect::Editorstate_Op_Rect(const Context& context)
    : EditorState(context), _state(STATE::IDLE), _operation(1), _menu(new QMenu())
{
    setup();
}
Editorstate_Op_Rect::~Editorstate_Op_Rect() {}
void Editorstate_Op_Rect::entry()
{
    printf("enter dig rect\n");
    EditorState::entry();
}
void Editorstate_Op_Rect::exit()
{
    printf("exit dig rect\n");
    EditorState::exit();
}
int Editorstate_Op_Rect::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
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
        case STATE::DO_OP:
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

void Editorstate_Op_Rect::do_op()
{
    // auto* subject = _selected_shape->get_poly_head();

    // redcgl::Vertex vertices[4];
    // vertices[0] = redcgl::create_vertex(_last_point.x(), _last_point.y(), 0, 0, 0);
    // vertices[1] = redcgl::create_vertex(_last_point.x(), _cursor_point.y(), 0, 0, 0);
    // vertices[2] = redcgl::create_vertex(_cursor_point.x(), _cursor_point.y(), 0, 0, 0);
    // vertices[3] = redcgl::create_vertex(_cursor_point.x(), _last_point.y(), 0, 0, 0);

    // F_POLYELEM* elem_1 = redcgl::build_poly_elems(vertices, 4);
    // F_POLYHEAD* clipper = redcgl::build_poly_head(elem_1, NULL);

    // F_POLYHEAD* new_head = NULL;
    // {
    //     printf("real cost\n");
    //     Timer t;
    //     new_head = redcgl::logical_operation(subject, _operation, clipper);
    // }
    // GraphicsShapeItem* new_shape_item = new GraphicsShapeItem(new_head);

    // _context.get_scene()->addItem(new_shape_item);
    // _context.get_scene()->removeItem(_selected_shape);
    // _selected_shape = nullptr;

    // _context.get_scene()->removeItem(_preview);
}

int Editorstate_Op_Rect::mouseLeftPressEvent(QGraphicsSceneMouseEvent* event)
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
            _selected_shape = dynamic_cast<GraphicsShapeItem*>(items[0]);
            _selected_shape->set_pen(QPen(Qt::blue, 0));
            _selected_shape->set_brush(QBrush(Qt::blue));
            _selected_shape->update();
            _state = STATE::START;
            break;
        }
        case STATE::START:
        {
            _last_point = _cursor_point;
            _context.get_scene()->addItem(_preview);
            _state = STATE::DO_OP;
            break;
        }
        case STATE::DO_OP:
        {
            do_op();
            _state = STATE::IDLE;
            break;
        }
    }
    return EditorState::mouseLeftPressEvent(event);
}
int Editorstate_Op_Rect::mouseRightPressEvent(QGraphicsSceneMouseEvent* event)
{
    QPoint p = event->screenPos();
    _menu->exec(p);
    return 0;
}
int Editorstate_Op_Rect::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    return EditorState::mouseDoubleClickEvent(event);
}
int Editorstate_Op_Rect::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    return EditorState::mouseReleaseEvent(event);
}

#endif  //CANVAS_EDITORSTATE_OP_RECT_H
