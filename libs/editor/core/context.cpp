#include "context.h"

#include <QGraphicsScene>

Context::Context()
{
    mScene = std::make_shared<QGraphicsScene>();
}