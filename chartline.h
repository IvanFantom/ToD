/*!
 * @author Denis Vashchuk
 * Class for work with split lines in chart
 *
*/

#ifndef CHARTLINE_H
#define CHARTLINE_H

#include <QtCore/QVector>
#include "point.h"

class ChartLine
{
private:
    struct LineInfo
    {
        //define the class numbers betwean which draw line
        int class0;
        int class1;
    };

    QVector< QVector<Point> > lines;
    QVector< LineInfo > linesInfo;

public:
    ChartLine();
    //add new ssplit line
    /*! @arg points source points
     *  @arg class0 first class
     *  @arg class1 second class
     */
    void addLine(const QVector<Point>&, int class0, int class1);
    //remove new split line
    void removeLine(int index);
    //get line by index
    QVector<Point> &lineAt(int index);
    //get line info by index
    LineInfo lineInfoAt(int index);
    //get count of split lines
    int count();
    //delete all split lines
    void removeAll();

    void operator=(ChartLine& line);
};

#endif // CHARTLINE_H
