#include "grid.h"
#include "constant.h"
#include "def.h"

#include <limits>


Grid::Grid()
    : Grid(DEFAULT_GRID_INTERVAL_X, DEFAULT_GRID_INTERVAL_Y)
{
}

Grid::Grid(const Length& interval_x, const Length& interval_y) : _grid_type(GRID_TYPE::LINES)
{
    set_interval_x(interval_x);
    set_interval_y(interval_y);
}

bool Grid::set_interval_x(const Length& length)
{
    if (length > 0)
    {
        _interval_x = length;
        return true;
    }
    return false;
}

bool Grid::set_interval_y(const Length& length)
{
    if (length > 0)
    {
        _interval_y = length;
        return true;
    }
    return false;
}

void Grid::map_to_grid(Point& point) const
{
    Length x = point.x();
    Length y = point.y();

    auto mapLengthToGrid = [](Length& value, const Length& interval) {
        R_ASSERT(interval > 0);

        using UnsignedLength = std::make_unsigned<Length>::type;

        UnsignedLength uMaximum;
        UnsignedLength uValueAbs;

        if (value >= 0) {
            uMaximum = static_cast<UnsignedLength>(std::numeric_limits<Length>::max());
            uValueAbs = static_cast<UnsignedLength>(value);
        } else {
            uMaximum = static_cast<UnsignedLength>(std::numeric_limits<Length>::min());
            uValueAbs = - static_cast<UnsignedLength>(value);
        }

        auto uInterval = static_cast<UnsignedLength>(interval);
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

    mapLengthToGrid(x, _interval_x);
    mapLengthToGrid(y, _interval_y);

    point.setX((int)x);
    point.setY((int)y);
}

Point Grid::mapped_to_grid(const Point& point) const
{
    Point p = point;
    map_to_grid(p);
    return p;
}
