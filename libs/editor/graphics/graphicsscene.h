#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include <QGraphicsScene>

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

   public:
    GraphicsScene(QObject* parent = nullptr);

};

#endif  // CUSTOMGRAPHICSSCENE_H
