#include "graphicsview.h"
#include "core/constant.h"
#include "eventhandler.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QScrollBar>
#include <QWheelEvent>
#include <cmath>

GraphicsView::GraphicsView(const Context& context, EventHandler* event_handler) : _event_handler(event_handler) {}

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
    if (mIsPanning)
    {
        auto pos = event->pos();
        auto delta = pos - mLastPos;
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        mLastPos = pos;
    }

    QGraphicsSceneMouseEvent e;
    e.setScreenPos(event->screenPos().toPoint());
    e.setScenePos(mapToScene(event->pos()));
    _event_handler->mouseMoveEvent(&e);
    return QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons().testFlag(Qt::MiddleButton))
    {
        mIsPanning = true;
        mLastPos = event->pos();
    }
    QGraphicsSceneMouseEvent e;
    e.setScreenPos(event->screenPos().toPoint());
    e.setScenePos(mapToScene(event->pos()));
    if (event->button() == Qt::LeftButton)
    {
        _event_handler->mouseLeftPressEvent(&e);
    }
    else if (event->button() == Qt::RightButton)
    {
        _event_handler->mouseRightPressEvent(&e);
    }
    return QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if (mIsPanning)
    {
        mIsPanning = false;
        event->accept();
        return;
    }
    QGraphicsSceneMouseEvent e;
    e.setScreenPos(event->screenPos().toPoint());
    e.setScenePos(mapToScene(event->pos()));
    if (event->button() == Qt::LeftButton)
    {
        _event_handler->mouseReleaseEvent(&e);
    }
    return QGraphicsView::mouseReleaseEvent(event);
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
    // 绘制十字光标
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
    newPen.setColor(Qt::yellow);
    newPen.setCosmetic(true);
    painter->setPen(newPen);
    painter->drawLine(-width, 0, width, 0);
    painter->drawLine(0, height, 0, -height);
    painter->setPen(oldPen);
}

void GraphicsView::drawBackground(QPainter* painter, const QRectF& rect)
{
    // 绘制格点
    QGraphicsView::drawBackground(painter, rect);

    if (mGrid.get_type() == Grid::NONE)
    {
        return;
    }

    QPen oldPen = painter->pen();
    QPen newPen(oldPen);
    newPen.setColor(Qt::gray);
    newPen.setCosmetic(true);
    painter->setPen(newPen);

    double intervalX = mGrid.get_interval_x();
    double intervalY = mGrid.get_interval_y();

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
    switch (mGrid.get_type())
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

    painter->setPen(oldPen);
}

void GraphicsView::scrollWithDegrees(const QPoint& numSteps) {}
