/*!
 * @author Denis Vashchuk
 * Point struct. Used in all  program ( in algorithms and graphics )
*/

#ifndef POINT_H
#define POINT_H

#include <QString>

enum ClassType { DEFINED, UNDEFINED };

struct Point
{
    int x;
    int y;

    int clas;
    ClassType classType;
    const bool operator==(const struct Point& p1) const
    {
         return ( p1.x == x && p1.y == y && p1.clas == clas );
    }
};
#endif // POINT_H
