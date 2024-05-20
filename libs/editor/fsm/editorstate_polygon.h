#ifndef CANVAS_EDITORSTATE_POLYGON_H
#define CANVAS_EDITORSTATE_POLYGON_H

#include "editorstate.h"

#include <QGraphicsLineItem>
#include <QPen>
#include <QPointF>

enum STATE
{
    IDLE = 0,
    ADD_EDGE,
    ADD_POLYGON,
};

class Editorstate_Polygon : public EditorState
{
   public:
    Editorstate_Polygon(const Context& context);
    ~Editorstate_Polygon();

    void entry();
    void exit();

    int mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    int mousePressEvent(QGraphicsSceneMouseEvent* event);
    int mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

   private:
    QVector<QGraphicsPolygonItem*> _items{};
    QVector<QGraphicsLineItem*> _preview_lines{};
    static const int _preview_cnt = 2;
    QGraphicsLineItem* _preview[_preview_cnt];
    QPointF _start_point{};
    QPointF _last_point{};
    QPointF _cursor_point{};
    STATE _state;
    int save_two = 0;

    double dot_product(double x1, double y1, double x2, double y2) { return x1 * x2 + y1 * y2; }

    double cross_product(double x1, double y1, double x2, double y2) { return x1 * y2 - x2 * y1; }

    double vec_magnitude(double x, double y) { return sqrt((x) * (x) + (y) * (y)); }

    void normalized(double* x, double* y)
    {
        double len = vec_magnitude(*x, *y);
        *x /= len;
        *y /= len;
    }

    static void update_line_item(QGraphicsLineItem* l, QPointF& p1, QPointF& p2)
    {
        QPen pen(Qt::red, 0);
        QLineF line;
        line.setP1(p1);
        line.setP2(p2);
        if (p1 != p2)
            l->setLine(line);
        else
            l->setLine(QLineF());
        l->setPen(pen);
        l->update();
    }

    static int cal_snap_45(double angle, int inclined, QPointF& p1, QPointF& p2, QPointF& res)
    {
        if (angle == 0.0)
            return cal_snap_45_strategy_1(p1, p2, res);

        if (((angle > 0) && (angle < M_PI_4)) || ((angle > M_PI_2) && (angle < (M_PI_2 + M_PI_4))))
        {
            if (inclined)
                return cal_snap_45_strategy_2(p1, p2, res);
            else
                return cal_snap_45_strategy_1(p1, p2, res);
        }
        else
        {
            if (inclined)
                return cal_snap_45_strategy_1(p1, p2, res);
            else
                return cal_snap_45_strategy_2(p1, p2, res);
        }
    }

    static int cal_snap_45_strategy_1(QPointF& st, QPointF mp, QPointF& sp)
    {
        if (fabs(st.x() - mp.x()) < 1E-12 || fabs(st.y() - mp.y()) < 1E-12)
            return 0;
        QPointF vec_p1_p2 = mp - st;
        double abs_x = fabs(vec_p1_p2.x());
        double abs_y = fabs(vec_p1_p2.y());

        QPointF temp;

        // 斜45度
        if (fabs(abs_x - abs_y) < 1E-12)
            return 0;

        if (abs_x > abs_y)
        {  // 往x轴吸附
            temp.setX(mp.x());
            temp.setY(st.y());
        }
        else
        {  // 往y轴吸附
            temp.setX(st.x());
            temp.setY(mp.y());
        }

        QPointF vec_new = st - temp;
        double len = sqrt(vec_new.y() * vec_new.y() + vec_new.x() * vec_new.x());
        vec_new = vec_new / len;

        double dist = sqrt((mp.y() - temp.y()) * (mp.y() - temp.y()) + (mp.x() - temp.x()) * (mp.x() - temp.x()));

        sp = temp + vec_new * dist;
        return 1;
    }

    static int cal_snap_45_strategy_2(QPointF& st, QPointF mp, QPointF& sp)
    {
        return cal_snap_45_strategy_1(mp, st, sp);
    }

    int check_closed()
    {
        if (_preview_lines.empty())
            return 0;
        return (_preview_lines.last()->line().p2() == _preview_lines.first()->line().p1());
    }
};

#endif  //CANVAS_EDITORSTATE_POLYGON_H
