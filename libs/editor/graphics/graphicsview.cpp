#include "graphicsview.h"
#include "core/constant.h"
#include "eventhandler.h"

#include <QDebug>
#include <QScrollBar>
#include <QWheelEvent>
#include <cmath>
GraphicsView::GraphicsView(EventHandler* eh) : is_panning(false), _event_handler(eh) {}

void GraphicsView::setup()
{
    setBackgroundBrush(Qt::black);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setSceneRect({-DEFAULT_SCENE_SIZE, -DEFAULT_SCENE_SIZE, DEFAULT_SCENE_SIZE * 2, DEFAULT_SCENE_SIZE * 2});
    scale(DEFAULT_SCALE_SIZE, DEFAULT_SCALE_SIZE);
}

void GraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if (is_panning)
    {
        auto pos = event->pos();
        auto delta = pos - _last_pos;
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        _last_pos = pos;
    }
    _event_handler->mouseMoveEvent(event);
}

void GraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons().testFlag(Qt::MiddleButton))
    {
        is_panning = true;
        _last_pos = event->pos();
    }

    _event_handler->mousePressEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if (is_panning)
    {
        is_panning = false;
        event->accept();
        return;
    }
    _event_handler->mouseReleaseEvent(event);
}

void GraphicsView::wheelEvent(QWheelEvent* event)
{
    QPoint pixelDelta = event->pixelDelta();
    QPoint angleDelta = event->angleDelta();
    bool processedByPixels = false;
    if (!pixelDelta.isNull())
    {
        // 使用像素增量进行处理
    }

    if (!processedByPixels && !angleDelta.isNull())
    {
        // 使用角度增量进行处理
        QPoint numSteps = angleDelta / 8 / 15;
        scrollWithDegrees(numSteps);
        auto modifiers = event->modifiers();
        if (modifiers.testFlag(Qt::KeyboardModifier::ControlModifier))
        {
            verticalScrollBar()->setValue(verticalScrollBar()->value() + numSteps.y() * SCROLL_STEP);
        }
        else if (modifiers.testFlag(Qt::KeyboardModifier::ShiftModifier))
        {
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() + numSteps.y() * SCROLL_STEP);
        }
        else
        {
            qreal factor = std::pow(SCALE_STEP, numSteps.y());
            scale(factor, factor);
        }
    }
    event->accept();
}

void GraphicsView::drawForeground(QPainter* painter, const QRectF& rect)
{
    QGraphicsView::drawForeground(painter, rect);

    double factorX = transform().m11();
    double factorY = transform().m22();

    double crossHairSize = 100.0;

    double width = crossHairSize;
    double height = crossHairSize;
    if (crossHairSize * factorX < MIN_CROSS_HAIR_SIZE)
    {
        width = MIN_CROSS_HAIR_SIZE / factorX;
    }
    if (crossHairSize * factorY < MIN_CROSS_HAIR_SIZE)
    {
        height = MIN_CROSS_HAIR_SIZE / factorY;
    }

    QPen oldPen = painter->pen();
    QPen newPen(oldPen);
    newPen.setColor(Qt::red);
    newPen.setCosmetic(true);
    painter->setPen(newPen);
    painter->drawLine(-(int)width, 0, (int)width, 0);
    painter->drawLine(0, (int)height, 0, -(int)height);
    painter->setPen(oldPen);
}

void GraphicsView::drawBackground(QPainter* painter, const QRectF& rect)
{
    QGraphicsView::drawBackground(painter, rect);

    if (_grid.getType() == Grid::NONE)
    {
        return;
    }

    QPen oldPen = painter->pen();
    QPen newPen(oldPen);
    newPen.setColor(Qt::gray);
    newPen.setCosmetic(true);
    painter->setPen(newPen);

    double intervalX = _grid.getIntervalX();
    double intervalY = _grid.getIntervalY();

    // qDebug() << startX << endX << startY << endY;

    auto visibleRect = mapToScene(viewport()->rect()).boundingRect();

    int factorX = std::ceil(visibleRect.width() / intervalX / MAX_GRID_COUNT);
    int factorY = std::ceil(visibleRect.height() / intervalY / MAX_GRID_COUNT);

    int factor = std::max({factorX, factorY, 1});
    intervalX *= factor;
    intervalY *= factor;

    double minX = rect.left();
    double minY = rect.top();
    double maxX = rect.right();
    double maxY = rect.bottom();

    double startX = std::floor(minX / intervalX) * intervalX;
    double endX = std::ceil(maxX / intervalX) * intervalX;

    double startY = std::floor(minY / intervalY) * intervalY;
    double endY = std::ceil(maxY / intervalY) * intervalY;

    // int count = 0;
    switch (_grid.getType())
    {
        case Grid::DOTS:
        {
            QVarLengthArray<QPointF, 2000> dots;
            for (qreal x = startX; x < endX; x += intervalX)
            {
                for (qreal y = startY; y < endY; y += intervalY)
                {
                    dots.append(QPointF(x, y));
                    // ++count;
                }
            }
            painter->drawPoints(dots.data(), dots.size());
        }
        break;
        case Grid::LINES:
        {
            QVarLengthArray<QLineF, 500> lines;
            for (qreal x = startX; x < endX; x += intervalX)
            {
                lines.append(QLineF(x, rect.top(), x, rect.bottom()));
                // ++count;
            }
            for (qreal y = startY; y < endY; y += intervalY)
            {
                lines.append(QLineF(rect.left(), y, rect.right(), y));
                // ++count;
            }
            painter->setOpacity(0.5);
            painter->drawLines(lines.data(), lines.size());
        }
        break;
        case Grid::NONE:
            break;
    }

    // qDebug() << count;
    painter->setPen(oldPen);
}

void GraphicsView::scrollWithDegrees(const QPoint& numSteps) {}
