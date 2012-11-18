/*!
 * @author Denis Vashchuk
*/


#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QtGui/QWidget>
#include <QtCore/QVector>
#include <QtGui/QMenu>
#include <QtGui/QAction>

#include "pointadddialog.h"
#include "chartline.h"

#define SHADOW_OFFSET 5
#define EDGE_OFFSET 0
#define POINT_SIZE 15
#define BORDER 6
#define STD_POINT_SIZE 20
#define FONT_SIZE 10
#define AXIS_EDGE_OFFSET 20
#define DEVISION 10
#define DEVISION_HEIGHT 2
#define DEFAULT_MAX 20
#define UNDEFINED_POINT_COLOR Qt::gray;

#define DRAW_AXIS 0

#include "point.h"
#include "exception.h"

enum AXIS { X, Y };

struct PointChartInfo
{
    Point point;
    bool selected;

    bool operator==(const struct PointChartInfo& pointInfo) const
    {
        return ( pointInfo.point == this->point );
    }
};

class ChartWidget: public QWidget
{
    Q_OBJECT
public:
    ChartWidget(QWidget *parrent = 0x0);
    void setMaxX(int x);
    void setMaxY(int y);
    int getMaxX();
    int getMaxY();

    //! add new point to chart
    void addPoint(const Point&);
    //! remove point from chart
    void removePoint(const Point&);
    //! remove point by index at points vector
    void removePoint(int index);
    /*! Get point at axis coord */
    Point pointAt(int, int) throw(NotFoundException);
    //! get all points from chart
    QVector<Point> getPoints() const;
    //! set new points
    void setPoints(QVector<Point> &srcPoints);
    //! set standart points
    void setStandard(QVector<Point>);
    //!get chart lines
    ChartLine& chartLines();
    //!remove all points from chart
    void RemoveAllPoints();

protected:
    int edgeOffset;
    int pointSize;
    int fontSize;
    int axisEdgeOffset;
    int devision;
    int xScale;
    int yScale;
    int maxX;
    int maxY;
    int stdPointSize;
    int pointBorder;

    //work program vars
    int selectedPointIndex;

    QVector<PointChartInfo> points;
    QVector<Point> standarts;

    QAction *actionAdd, *actionAddUndefined, *actionRemove;
    PointAddDialog* dialogAdd;
    ChartLine lines;

    virtual void paintEvent ( QPaintEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void contextMenuEvent(QContextMenuEvent *);


    //!  get point by pixel coord from chart. If point don't exist this function throw NotFoundException
    Point getPointByPixel(int, int) throw(NotFoundException);
    //! get point index in points by pixel coord from chart. If point don't exist this function return -1
    int getPointIndexByPixel(int, int);
    //! check for that - at this coord chart have point
    bool inPoint(Point p, int x, int y);
    //! reset all selected points
    void resetPoints();
    //! select point by index at points list
    void selectPoint(int index);
    //! move point to new pixel coord
    void moveToNewPos(int index, int x, int y);
    //! convert coord
    int convertCoordFromChart(int coord, AXIS a) const;
    int convertCoordFromWidget(int coord, AXIS a) const;

    void drawBackground(QPainter& painter);
    void drawAxis(QPainter& painter);
    void drawPoint(QPainter& painter, const Point& point, QColor color, int size);
    void drawPoint(QPainter& painter, const Point& point, QColor color);
    void drawStandartPoint(QPainter& painter, const Point& point, QColor color);
    //! draw line betwean points
    void drawLine(QPainter &painter, QVector<Point>, QColor color);
    void rescale();
    void initContextMenu();
    //! add point to from pixels coordinates
    void addPoint(int x, int y, ClassType type, int clas);

signals:
    void pointSelected(int index);
    void pointMoved(int index, int chartX, int chartY);
    void pointAdded(int index);
    void pointRemoved(int index);

};

#endif // CHARTWIDGET_H
