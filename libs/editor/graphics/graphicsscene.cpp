#include "graphicsscene.h"
#include <QPainter>

GraphicsScene::GraphicsScene(QObject* parent) : QGraphicsScene(parent)
{
    setBackgroundBrush(Qt::black);
}
