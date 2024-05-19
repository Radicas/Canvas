#ifndef GRID_H_
#define GRID_H_

#include "length.h"
#include "point.h"

class Grid {
public:
    enum GridType {
        NONE,
        DOTS,
        LINES
    };

    Grid();
    Grid(const Length& aIntervalX, const Length& aIntervalY);

    GridType getType() const { return mGridType; }
    void setType(GridType aGridType) { mGridType = aGridType; }

    const Length& getIntervalX() const { return mIntervalX; }
    bool setIntervalX(const Length& aLength);

    const Length& getIntervalY() const { return mIntervalY; }
    bool setIntervalY(const Length& aLength);

    void mapToGrid(Point& point);
    Point mappedToGrid(const Point& point);

private:
    Length mIntervalX;
    Length mIntervalY;
    GridType mGridType;
};

#endif /* GRID_H_ */
