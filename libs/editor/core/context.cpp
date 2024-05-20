#include "context.h"

#include <QGraphicsScene>

Context::Context()
{
    _scene = std::make_shared<QGraphicsScene>();
}