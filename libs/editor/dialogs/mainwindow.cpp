#include "mainwindow.h"

#include "core/context.h"
#include "fsm/editorfsm.h"
#include "graphics/graphicsview.h"

#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <memory>

MainWindow::MainWindow(const std::shared_ptr<Context>& aContext)
    : _view(nullptr), _context(aContext), _fsm(std::make_shared<EditorFsm>(*aContext))
{
    setup();
}

MainWindow::~MainWindow()
{
    if (_view)
    {
        _view->setScene(nullptr);
    }
}

void MainWindow::setup()
{
    static bool flag = false;
    if (flag)
    {
        return;
    }
    _view = new GraphicsView(*_context, _fsm.get());
    _view->setup();
    const auto& scene = _context->get_scene();
    _view->setScene(scene.get());
    setCentralWidget(_view);
    create_menu_bar();
    flag = true;
}

void MainWindow::create_menu_bar()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu* drawMenu = menuBar()->addMenu(tr("&Draw"));

    auto* drawPolygon = new QAction("Polygon");
    drawMenu->addAction(drawPolygon);
    connect(drawPolygon, &QAction::triggered, this, &MainWindow::action_polygon_triggered);

    auto* drawCircle = new QAction("Circle");
    drawMenu->addAction(drawCircle);
    connect(drawCircle, &QAction::triggered, this, &MainWindow::action_circle_triggered);
}

void MainWindow::action_polygon_triggered()
{
    printf("change state to DRAW POLYGON\n");
    _fsm->set_current_state(FSM_STATE::DRAW_POLYGON);
}

void MainWindow::action_circle_triggered()
{
    printf("change state to DRAW CIRCLE\n");
    _fsm->set_current_state(FSM_STATE::DRAW_CIRCLE);
}
