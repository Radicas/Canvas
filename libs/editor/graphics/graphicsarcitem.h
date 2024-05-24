#ifndef QGRAPHICSARCITEM_H
#define QGRAPHICSARCITEM_H

#include <QGraphicsItem>
#include <QPainter>

class GraphicsArcItem : public QGraphicsItem
{
   public:
    GraphicsArcItem();
    GraphicsArcItem(const QRectF& rect, int startAngle, int spanAngle, QGraphicsItem* parent = nullptr);
    QRectF boundingRect() const override;
    void setPen(const QPen& pen);
    void setRect(const QRectF& rect);
    void setStartAngle(int angle);
    void setSpanAngle(int angle);

    QRectF getRect();
    int getStartAngle() const;
    int getSpanAngle() const;

    /**
     * @brif 生成圆弧
     *
     * @param p1 圆弧起点
     * @param p2 圆弧终点
     * @param p3 其他一点
     */
    void generate_arc(const QPointF& p1, const QPointF& p2, const QPointF& p3);

   protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

   private:
    QRectF _rect;         // 包围盒
    double _start_angle;  // 起始角度，需要乘以16
    double _span_angle;   // 旋转角度，正为逆时针，负为顺时针，需要乘以16
    QPen _pen;
};

#endif  // QGRAPHICSARCITEM_H
