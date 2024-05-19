#include "grid.h"
#include "constant.h"
#include "def.h"

#include <limits>


Grid::Grid()
    : Grid(DEFAULT_GRID_INTERVAL_X, DEFAULT_GRID_INTERVAL_Y)
{
}

Grid::Grid(const Length &aIntervalX, const Length &aIntervalY)
    : mGridType(GridType::DOTS)
{
    setIntervalX(aIntervalX);
    setIntervalY(aIntervalY);
}

bool Grid::setIntervalX(const Length &aLength)
{
    if (aLength > 0) {
        mIntervalX = aLength;
        return true;
    }
    return false;
}

bool Grid::setIntervalY(const Length &aLength)
{
    if (aLength > 0) {
        mIntervalY = aLength;
        return true;
    }
    return false;
}

void Grid::mapToGrid(Point &point)
{
    Length x = point.x();
    Length y = point.y();


    auto mapLengthToGrid = [](Length& value, const Length& interval) {
        R_ASSERT(interval > 0);

        using UnsignedLength = Length;

        UnsignedLength uMaximum;
        UnsignedLength uValueAbs;

        if (value >= 0) {
            uMaximum = static_cast<UnsignedLength>(std::numeric_limits<Length>::max());
            uValueAbs = static_cast<UnsignedLength>(value);
        } else {
            uMaximum = static_cast<UnsignedLength>(std::numeric_limits<Length>::min());
            uValueAbs = - static_cast<UnsignedLength>(value);
        }

        UnsignedLength uInterval = static_cast<UnsignedLength>(interval);
        UnsignedLength uMapped;

        UnsignedLength uRemainder = uValueAbs % uInterval;
        if ((uRemainder << 1) > uInterval) {
            uMapped = uValueAbs - uRemainder + uInterval;
            if ( (uMapped < uValueAbs) || (uMapped > uMaximum) ) {
                uValueAbs -= uRemainder;
            } else {
                uValueAbs = uMapped;
            }
        } else {
            uValueAbs -= uRemainder;
        }

        if (value >= 0) {
            value = static_cast<Length>(uValueAbs);
        } else {
            value = static_cast<Length>(-uValueAbs);
        }
    };

    mapLengthToGrid(x, mIntervalX);
    mapLengthToGrid(y, mIntervalY);

    point.setX(x);
    point.setY(y);
}

Point Grid::mappedToGrid(const Point &point)
{
    Point p = point;
    mapToGrid(p);
    return p;
}
