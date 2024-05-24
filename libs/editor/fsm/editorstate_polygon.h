#ifndef CANVAS_EDITORSTATE_POLYGON_H
#define CANVAS_EDITORSTATE_POLYGON_H

#include "core/context.h"
#include "core/grid.h"
#include "editorstate.h"
#include "graphics/graphicsarcitem.h"
#include "graphics/graphicsscene.h"
#include "graphics/graphicsshapeitem.h"
#include "redcgl/include/redcgl.h"

#include <QAction>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPen>
#include <QPointF>

#define DEBUG_PREVIEW

enum STATE
{
    IDLE = 0,
    ADD_EDGE,
    ADD_ARC,
};

class Editorstate_Polygon : public EditorState
{
   public:
    Editorstate_Polygon(const Context& context);
    ~Editorstate_Polygon();
    void entry() override;
    void exit() override;
    int mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseLeftPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseRightPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseMoveSegEvent(QGraphicsSceneMouseEvent* event);
    int mouseLeftPressSegEvent(QGraphicsSceneMouseEvent* event);
    int mouseMoveArcEvent(QGraphicsSceneMouseEvent* event);
    int mouseLeftPressArcEvent(QGraphicsSceneMouseEvent* event);

   protected:
    void setup();
    void add_menus();
    QMenu* _menu;  // 右键菜单

   private:
    std::vector<redcgl::Edge> _shape_edges{};                     // shape的边
    std::vector<GraphicsShapeItem*> _shape_items{};               // 已有shape绘制对象
    std::vector<QGraphicsLineItem*> _preview_lines{};             // 已有的预览线段对象
    static const int _preview_temp_seg_cnt = 2;                   // 预览线数量
    QGraphicsLineItem* _preview_temp_seg[_preview_temp_seg_cnt];  // 新预览线段对象
    QGraphicsLineItem* _preview_temp_arc_seg{};                   // 圆弧预览线段
    GraphicsArcItem* _preview_temp_arc{};                         // 圆弧预览
    QPointF _last_point{};                                        // 上次点击的坐标
    QPointF _cursor_point{};                                      // 当前点击的坐标
    QPointF _arc_start{};                                         // 圆弧起点
    STATE _state;                                                 // 操作状态
    int save_two = 0;                                             // 是否保存两根预览线
    int add_arc_edge = 0;                                         // 是否添加圆弧边
    int _temp_rect_cnt = 5;                                       // 辅助框数量
    QGraphicsRectItem* _temp_rect[5];                             // 辅助框

    void init_previews();
    int check_closed();
    void update_line_item(QGraphicsLineItem* l, QPointF& p1, QPointF& p2);
    int add_graphics_polygon();
    int clear_preview_lines();
};

#endif  //CANVAS_EDITORSTATE_POLYGON_H
