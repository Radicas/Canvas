#include "graphicsshapeitem.h"
#include "convertor.h"

GraphicsShapeItem::GraphicsShapeItem(redcgl::Polygon* polygon)
    : _pen(Qt::red, 0), _brush(Qt::green), _rect(), _polygon(polygon), _poly_head(nullptr), _path()
{
    setup();
}

GraphicsShapeItem::~GraphicsShapeItem() {}
QRectF GraphicsShapeItem::boundingRect() const
{
    return _rect;
}
QPainterPath GraphicsShapeItem::shape() const
{
    return _path;
}
int GraphicsShapeItem::set_pen(const QPen& pen)
{
    _pen = pen;
    return 1;
}
int GraphicsShapeItem::set_brush(const QBrush& brush)
{
    _brush = brush;
    return 1;
}
void GraphicsShapeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(_pen);
    painter->setBrush(_brush);
    painter->drawPath(_path);

    painter->save();
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(_rect);
    painter->restore();
}

void GraphicsShapeItem::setup()
{
    void init_path();
    void init_poly_head();
}
void GraphicsShapeItem::init_path() {
    auto* curr_poly = _polygon;
    while(_polygon != nullptr)
    {
        // logic

    }
}
void GraphicsShapeItem::init_poly_head() {

}