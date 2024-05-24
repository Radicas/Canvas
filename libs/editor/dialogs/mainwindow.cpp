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

std::map<std::string, FSM_STATE> action_fsm_map{
    {"Polygon", FSM_STATE::DRAW_POLYGON},
    {"Rect", FSM_STATE::DRAW_RECT},
    {"Circle", FSM_STATE::DRAW_CIRCLE},
};

void MainWindow::create_menu_bar()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu* drawMenu = menuBar()->addMenu(tr("&Draw"));
    std::vector<QAction*> actions;

    // NOTE: 增加动作时只需要增加下面三种即可，同时写到上面的map里
    auto* drawPolygon = new QAction("Polygon");
    auto* drawRect = new QAction("Rect");
    auto* drawCircle = new QAction("Circle");

    drawMenu->addAction(drawPolygon);
    drawMenu->addAction(drawRect);
    drawMenu->addAction(drawCircle);

    actions.emplace_back(drawPolygon);
    actions.emplace_back(drawRect);
    actions.emplace_back(drawCircle);

    for (int i = 0; i < actions.size(); ++i)
    {
        connect(actions[i], &QAction::triggered, this,
                [i, actions, this]()
                {
                    auto action_name = actions[i]->text().toStdString();
                    _fsm->set_current_state(action_fsm_map[action_name]);
                });
    }
}
