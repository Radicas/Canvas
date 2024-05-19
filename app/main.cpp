#include <QApplication>
#include "editor/core/context.h"
#include "editor/dialogs/mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<Context> context = std::make_shared<Context>();
    MainWindow w(context);
    w.show();
    return QApplication::exec();
}
