//
// Created by Radica on 2025/1/5.
//

#ifndef CANVAS_EDITORSTATE_CGAL_TEST_H
#define CANVAS_EDITORSTATE_CGAL_TEST_H

#include "core/context.h"
#include "core/grid.h"
#include "editorstate.h"
#include "graphics/graphicsarcitem.h"
#include "graphics/graphicsscene.h"
#include "graphics/graphicsshapeitem.h"
#include "redcgl/inc/redcgl/polygon.h"

#include <QAction>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPen>
#include <QPointF>

class EditorState_CGAL_Test : public EditorState
{
   public:
    enum class STATE
    {
        IDLE = 0,
    };

    explicit EditorState_CGAL_Test(const Context& context);
    ~EditorState_CGAL_Test() override;

    void entry() override;
    void exit() override;

    int mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseLeftPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseRightPressEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    int mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

   protected:
    void setup();

   private:
    STATE _state;                      //
    QPen _pen;                         //
    QBrush _brush;                     //
    std::vector<QPainterPath> _paths;  //
    QMenu* _menu;                      //

    void add_menus();

   private slots:
    void expand();
    void shrink();
};

#endif  //CANVAS_EDITORSTATE_CGAL_TEST_H
