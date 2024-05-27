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

#define STR_POLYGON "Polygon"
#define STR_RECT "Rect"
#define STR_CIRCLE "Circle"
#define STR_DIG_RECT "Dig Rect"

std::map<std::string, FSM_STATE> action_fsm_map{
    {STR_POLYGON, FSM_STATE::DRAW_POLYGON},
    {STR_RECT, FSM_STATE::DRAW_RECT},
    {STR_CIRCLE, FSM_STATE::DRAW_CIRCLE},
    {STR_DIG_RECT, FSM_STATE::DIG_RECT},
};

void MainWindow::create_menu_bar()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu* drawMenu = menuBar()->addMenu(tr("&Draw"));
    std::vector<QAction*> actions;

    // NOTE: 增加动作时只需要增加下面三种即可，同时写到上面的map里
    auto* drawPolygon = new QAction(STR_POLYGON);
    auto* drawRect = new QAction(STR_RECT);
    auto* drawCircle = new QAction(STR_CIRCLE);
    auto* ditRect = new QAction(STR_DIG_RECT);

    drawMenu->addAction(drawPolygon);
    drawMenu->addAction(drawRect);
    drawMenu->addAction(drawCircle);
    drawMenu->addAction(ditRect);

    actions.emplace_back(drawPolygon);
    actions.emplace_back(drawRect);
    actions.emplace_back(drawCircle);
    actions.emplace_back(ditRect);

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
