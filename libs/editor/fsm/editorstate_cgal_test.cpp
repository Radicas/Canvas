//
// Created by Radica on 2025/1/5.
//

#include "editorstate_cgal_test.h"

EditorState_CGAL_Test::EditorState_CGAL_Test(const Context& context)
    : EditorState(context), _state(STATE::IDLE), _paths({}), _menu(new QMenu())
{
    setup();
    add_menus();
}

EditorState_CGAL_Test::~EditorState_CGAL_Test() = default;

void EditorState_CGAL_Test::entry()
{
    printf("enter CGAL test\n");
    EditorState::entry();
}

void EditorState_CGAL_Test::exit()
{
    printf("exit CGAL test\n");
    EditorState::exit();
}

int EditorState_CGAL_Test::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    switch (_state)
    {
        case STATE::IDLE:
            break;
    }
    return EditorState::mouseMoveEvent(event);
}

int EditorState_CGAL_Test::mouseLeftPressEvent(QGraphicsSceneMouseEvent* event)
{
    switch (_state)
    {
        case STATE::IDLE:
        {
            break;
        }

        default:
            break;
    }
    return EditorState::mouseLeftPressEvent(event);
}

int EditorState_CGAL_Test::mouseRightPressEvent(QGraphicsSceneMouseEvent* event)
{
    QPoint p = event->screenPos();
    _menu->exec(p);
    return 0;
}

int EditorState_CGAL_Test::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    return EditorState::mouseDoubleClickEvent(event);
}

int EditorState_CGAL_Test::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    return EditorState::mouseReleaseEvent(event);
}

void EditorState_CGAL_Test::setup()
{
    _pen = {Qt::red, 0};
    _brush = {Qt::green};
    _state = STATE::IDLE;
}

void EditorState_CGAL_Test::add_menus()
{
    auto* expand = new QAction("Expand");
    _menu->addAction(expand);
    connect(expand, &QAction::triggered, this, &EditorState_CGAL_Test::expand);

    auto* shrink = new QAction("Shrink");
    _menu->addAction(shrink);
    connect(shrink, &QAction::triggered, this, &EditorState_CGAL_Test::shrink);
}

void EditorState_CGAL_Test::expand() {}
void EditorState_CGAL_Test::shrink() {}