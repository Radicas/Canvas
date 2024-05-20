#ifndef GRID_H_
#define GRID_H_

#include "length.h"
#include "point.h"

class Grid {
public:
    enum GRID_TYPE
    {
        NONE,
        DOTS,
        LINES
    };

    Grid();
    Grid(const Length& interval_x, const Length& interval_y);

    GRID_TYPE get_type() const { return _grid_type; }
    void setType(GRID_TYPE grid_type) { _grid_type = grid_type; }

    const Length& get_interval_x() const { return _interval_x; }
    bool set_interval_x(const Length& length);

    const Length& get_interval_y() const { return _interval_y; }
    bool set_interval_y(const Length& length);

    void map_to_grid(Point& point) const;
    Point mapped_to_grid(const Point& point) const;

   private:
    Length _interval_x;
    Length _interval_y;
    GRID_TYPE _grid_type;
};

#endif /* GRID_H_ */
