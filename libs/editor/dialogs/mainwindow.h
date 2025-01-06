#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "fsm/eventhandler.h"

#include <QMainWindow>

class GraphicsView;
class Context;
class EditorFsm;
class MainWindow : public QMainWindow
{
   public:
    explicit MainWindow(const std::shared_ptr<Context>& aContext);
    ~MainWindow() override;

   protected:
    void setup();
    void create_menu_bar();

   private:
    GraphicsView* _view;
    std::shared_ptr<Context> _context;
    std::shared_ptr<EditorFsm> _fsm;
};

#endif /* MAINWINDOW_H_ */
