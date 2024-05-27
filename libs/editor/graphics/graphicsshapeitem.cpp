#include "graphicsshapeitem.h"
#include "convertor.h"

GraphicsShapeItem::GraphicsShapeItem(const std::vector<redcgl::Edge>& edges)
    : _pen(Qt::red, 0), _brush(Qt::green), _rect(), _path()
{
    init_polygon();
    set_edges(edges);
    init_painter_path();
    _rect = _path.boundingRect();
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
int GraphicsShapeItem::set_edges(const std::vector<redcgl::Edge>& edges)
{
    _polygon.num_edges = (int)(edges.size());
    if (_polygon.edges == nullptr)
        _polygon.edges = (redcgl::Edge*)(malloc(_polygon.num_edges * sizeof(redcgl::Edge)));
    else
    {
        // 重新分配内存
        redcgl::Edge* temp = (redcgl::Edge*)(realloc(_polygon.edges, _polygon.num_edges * sizeof(redcgl::Edge)));
        if (temp == nullptr)
            return -1;
        _polygon.edges = temp;
    }

    // 覆盖旧数据
    for (int i = 0; i < _polygon.num_edges; ++i)
    {
        _polygon.edges[i] = edges[i];
    }
    return 1;
}
int GraphicsShapeItem::append_edge(const redcgl::Edge& edge)
{
    _polygon.num_edges++;
    redcgl::Edge* temp = (redcgl::Edge*)realloc(_polygon.edges, 1);
    if (temp == nullptr)
    {
        fprintf(stderr, "重新分配内存失败\n");
        free(_polygon.edges);  // 释放原来的数组
        return -1;
    }
    else
    {
        _polygon.edges = temp;  // 更新数组指针
    }
    _polygon.edges[_polygon.num_edges] = edge;
    return 1;
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
int GraphicsShapeItem::init_polygon()
{
    redcgl::empty_polygon(&_polygon);
    return 1;
}
int GraphicsShapeItem::set_path_to_circle()
{
    auto& edge = _polygon.edges[0];
    const QtArc arc = edge_to_qt_arc(edge);
    _path.moveTo(edge.st.x, edge.st.y);
    _path.arcTo(arc._rect, arc._start_angle / 16, arc._span_angle / 16);
    return 1;
}
int GraphicsShapeItem::init_painter_path()
{
    redcgl::Polygon* curr_poly = &_polygon;
    if (curr_poly == nullptr || curr_poly->edges == nullptr)
        return -1;

    // 当前shape是圆
    if (curr_poly->num_edges == 1 && curr_poly->edges[0].r != 0.0)
        return set_path_to_circle();

    while (curr_poly != nullptr)  // 遍历多边形
    {
        redcgl::Edge* first_edge = &curr_poly->edges[0];
        _path.moveTo(first_edge->st.x, first_edge->st.y);  // 路径起点
        for (int i = 1; i < curr_poly->num_edges; ++i)  // 外轮廓
        {
            redcgl::Edge* curr_edge = &curr_poly->edges[i];
            if (curr_edge->r == 0.0)  // 线段
            {
                _path.lineTo(curr_edge->st.x, curr_edge->st.y);
            }
            else
            {  // 圆弧
                const QtArc qt_arc = edge_to_qt_arc(*curr_edge);
                _path.arcTo(qt_arc._rect, qt_arc._start_angle / 16, qt_arc._span_angle / 16);
            }
        }
        redcgl::Polygon* curr_hole = curr_poly->next_hole;
        while (curr_hole != nullptr)
        {
            redcgl::Edge* first_hole_edge = &curr_poly->edges[0];
            _path.moveTo(first_hole_edge->st.x, first_hole_edge->st.y);  // 路径起点
            for (int i = 1; i < curr_hole->num_edges; ++i)               // 外轮廓
            {
                redcgl::Edge* curr_edge = &curr_hole->edges[i];
                if (curr_edge->r == 0.0)  // 线段
                {
                    _path.lineTo(curr_edge->st.x, curr_edge->st.y);
                }
                else
                {  // 圆弧
                    const QtArc qt_arc = edge_to_qt_arc(*curr_edge);
                    _path.arcTo(qt_arc._rect, qt_arc._start_angle / 16, qt_arc._span_angle / 16);
                }
            }
            curr_hole = curr_hole->next_hole;
        }
        _path.closeSubpath();
        curr_poly = curr_poly->next;
    }
    return 1;
}
