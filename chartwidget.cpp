#include "chartwidget.h"
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QMouseEvent>
#include <QtCore/QDebug>
#include <QtCore/QList>
#include "classcolor.h"

ChartWidget::ChartWidget(QWidget* parrent):QWidget(parrent), selectedPointIndex(-1)
{
    this->setWindowTitle("Theory of solutions searching chart window");
    this->setWindowIcon(QIcon(":/images/icon.png"));
    dialogAdd = new PointAddDialog(this);

    initContextMenu();

    edgeOffset = EDGE_OFFSET;
    pointSize = POINT_SIZE;
    fontSize = FONT_SIZE;
    axisEdgeOffset = AXIS_EDGE_OFFSET;
    devision = DEVISION;
    stdPointSize = STD_POINT_SIZE;
    maxX = DEFAULT_MAX;
    maxY = DEFAULT_MAX;
    pointBorder = BORDER;
    rescale();

    return;
}

void ChartWidget::setMaxX(int x)
{
    maxX = x;
    rescale();
    repaint();
}

void ChartWidget::setMaxY(int y)
{
    maxY = y;
    rescale();
    repaint();
}

int ChartWidget::getMaxX()
{
    return maxX;
}

int ChartWidget::getMaxY()
{
    return maxY;
}

void ChartWidget::addPoint(const Point &p)
{
    int x_pix = convertCoordFromChart(p.x, X) ;
    int y_pix = convertCoordFromChart(p.y, Y);

    if(x_pix > width()-edgeOffset )
        setMaxX(p.x);

    if(y_pix > height()-edgeOffset-axisEdgeOffset )
        setMaxY(p.y);

    PointChartInfo pointInfo;
    pointInfo.selected = false;
    pointInfo.point = p;

    points.append(pointInfo);
}

void ChartWidget::removePoint(const Point &p)
{
    PointChartInfo pointInfo;
    pointInfo.point = p;
    int index = points.indexOf(pointInfo);

    if(index == -1 )
        return;

    points.remove(index);

    repaint();

    emit pointRemoved(index);
}

void ChartWidget::removePoint(int index)
{
    if(points.count() > index && index >= 0 ) {
        points.remove(index);
        repaint();
     } else
        throw OutOfIndexException();
}

Point ChartWidget::pointAt(int ax, int ay) throw(NotFoundException)
{
    /* ax - scaled coordinat, ay - scaled coordinat */
    foreach(PointChartInfo p, points) {
        if(p.point.x = ax && p.point.y==ay)
            return p.point;
    }

    throw NotFoundException();
}

QVector<Point> ChartWidget::getPoints() const
{
    QVector<Point> list;

    foreach(PointChartInfo p, points)
        list.append(p.point);

    return list;
}

void ChartWidget::setPoints(QVector<Point> &srcPoints)
{
    points.clear();

    foreach(Point p, srcPoints)
        addPoint(p);

    repaint();
}

void ChartWidget::drawLine(QPainter& painter, QVector<Point> linePoints, QColor color)
{
    QPen pen;
    pen.setColor(color);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);

    for(int i=0; i<linePoints.count()-1; i++) {
        Point curPoint = linePoints[i];
        Point nextPoint = linePoints[i+1];

        painter.drawLine(convertCoordFromChart(curPoint.x, X),
                         convertCoordFromChart(curPoint.y, Y),
                         convertCoordFromChart(nextPoint.x, X),
                         convertCoordFromChart(nextPoint.y, Y));
    }
}

void ChartWidget::setStandard(QVector<Point> std)
{
    standarts.clear();
    foreach(Point p, std)
        standarts.append(p);

    repaint();
}

void ChartWidget::paintEvent(QPaintEvent *event)
{
    rescale();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //draw chart background
    drawBackground(painter);
    //draw chart axis
    drawAxis(painter);
    //draw user defined points
    foreach(PointChartInfo p, points) {
        if( p.selected)
            drawPoint(painter, p.point, Qt::green);
        else
            drawPoint(painter, p.point, ClassColor::classToColor(p.point.clas) );
    }
    //draw standart points
    foreach(Point p, standarts) {
        drawStandartPoint(painter, p, ClassColor::classToColor(p.clas) );
    }

    //draw split lines
    for(int i=0; i<lines.count(); i++) {
        drawLine(painter, lines.lineAt(i), ClassColor::classToColor(lines.lineInfoAt(i).class0) );
    }
}

void ChartWidget::mousePressEvent(QMouseEvent *event)
{
    int index = getPointIndexByPixel(event->x(), event->y());

    if(event->button() == Qt::LeftButton) {
        //if not found
        if( index == -1 ) {
            //точка не выделена
            selectedPointIndex = -1;
            return;
        }

        //select point
        selectPoint(index);
        //this->setCursor(Qt::ClosedHandCursor);
        this->setCursor(Qt::ClosedHandCursor);
        qDebug() << "Point founded at " << event->x() << " " << event->y();
    }
    else if(event->button() == Qt::RightButton) {
        QList<QAction*> actionsList;

        if(index == -1) {
            actionsList.append(actionAdd);
        }
        else {
            actionsList.append(actionRemove);
        }

        QAction* res =  QMenu::exec(actionsList, event->globalPos());
        if( res  == actionRemove ) {
            removePoint(index);
        } else if( res == actionAdd ) {
            if(dialogAdd->exec() == QDialog::Rejected)
                return;

            Point p;
            p.classType = dialogAdd->getClassType();
            if( p.classType == UNDEFINED )
                p.clas = -1;
            else
                 p.clas = dialogAdd->getClassNumber();
            p.x = convertCoordFromWidget(event->x(), X);
            p.y = convertCoordFromWidget(event->y(), Y);
            addPoint(p);

            repaint();
            emit pointAdded(points.count()-1);
        }

    }
}

void ChartWidget::mouseReleaseEvent(QMouseEvent *event)
{
    this->setCursor(Qt::ArrowCursor);

    if(event->button() == Qt::LeftButton) {
        //if not found
        if( selectedPointIndex != -1 ) {

            if(selectedPointIndex != -1 )
                moveToNewPos(selectedPointIndex, event->x(), event->y());
            return;
        }
        qDebug() << "Point founded at " << event->x() << " " << event->y();
    }
}

void ChartWidget::contextMenuEvent(QContextMenuEvent *e)
{

}

void ChartWidget::drawBackground(QPainter &painter)
{
    painter.setPen(Qt::SolidLine);
    painter.setBrush(Qt::SolidPattern);
    painter.fillRect(edgeOffset, edgeOffset,
                     this->width()-2*edgeOffset, this->height()-2*edgeOffset,
                     Qt::white);
}

void ChartWidget::drawPoint(QPainter &painter, const Point &point, QColor color)
{

    drawPoint(painter, point, color, pointSize);


    QPen pen( Qt::SolidLine );
    pen.setColor(Qt::red);
    painter.setPen( pen );

    if(point.classType ==UNDEFINED)
        painter.drawText(convertCoordFromChart(point.x, X),
                           convertCoordFromChart(point.y, Y),
                           "?"
                           );

}

void ChartWidget::drawPoint(QPainter &painter, const Point &point, QColor color, int pointSize)
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);


    brush.setColor( ClassColor::getShadowColor(color));
    painter.setBrush( brush );
    painter.setPen(Qt::NoPen);

    painter.drawEllipse( convertCoordFromChart(point.x, X)+SHADOW_OFFSET,
                        convertCoordFromChart(point.y, Y)+SHADOW_OFFSET,
                         pointSize, pointSize );

    brush.setColor( ClassColor::borderColor);
    painter.setBrush( brush );
    painter.setPen(Qt::NoPen);

    painter.drawEllipse( convertCoordFromChart(point.x, X)-pointBorder/2,
                        convertCoordFromChart(point.y, Y)-pointBorder/2,
                         pointSize+pointBorder, pointSize+pointBorder );


    brush.setColor(color);
    painter.setBrush( brush );
    painter.setPen(Qt::NoPen);

    painter.drawEllipse( convertCoordFromChart(point.x, X),
                        convertCoordFromChart(point.y, Y),
                         pointSize, pointSize );
}

void ChartWidget::drawStandartPoint(QPainter &painter, const Point &point, QColor color)
{
    drawPoint(painter, point, color, stdPointSize);
}

void ChartWidget::rescale()
{
    xScale = width() / maxX;
    yScale = height() / maxY;
}

void ChartWidget::initContextMenu()
{
    actionAdd = new QAction("Add point", 0x0);
    actionAdd->setIcon(QIcon(":/images/add.png"));

    actionAddUndefined = new QAction("Add point", 0x0);
    actionAddUndefined->setIcon(QIcon(":/images/add.png"));

    actionRemove = new QAction("Remove point", 0x0);
    actionRemove->setIcon(QIcon(":/images/remove.png"));
}

void ChartWidget::addPoint(int x, int y, ClassType type, int clas)
{
}

void ChartWidget::drawAxis(QPainter &painter)
{

    int dx = 10;
    int dy = 10;

    int yx = axisEdgeOffset + edgeOffset;
    int yy = height() - yx;
    int yheight = edgeOffset + axisEdgeOffset;
    int ywidth = yx;

    painter.drawLine(yx, yy, ywidth, yheight);
#if DRAW_AXIS
    int y_count = maxY;
    devision = (height() - 2*edgeOffset - 2*axisEdgeOffset) / maxY;

    for( int i = 0; i<y_count; i++ ) {
        painter.drawLine(yx-DEVISION_HEIGHT, yy-devision*i, yx+DEVISION_HEIGHT, yy-devision*i);
    }
#endif

    int xx = axisEdgeOffset + edgeOffset;
    int xy = height() - xx;
    int xheight = xy;
    int xwidth = width() - xx;
    painter.drawLine(xx, xy, xwidth, xheight);
#if DRAW_AXIS
    int x_count = maxX;
    devision = (width() - 2*edgeOffset - 2*axisEdgeOffset) / maxX;
    for( int i = 1; i<x_count; i++) {
        painter.drawLine(xx+devision*i, xy+DEVISION_HEIGHT, xx+devision*i, xheight-DEVISION_HEIGHT);
    }
#endif
}

Point ChartWidget::getPointByPixel(int x, int y) throw (NotFoundException)
{
    foreach(PointChartInfo p, points) {
        if( inPoint(p.point, x, y))
            return p.point;
    }
    throw NotFoundException();
}

int ChartWidget::getPointIndexByPixel(int x, int y)
{
    for(int i=0; i<points.count(); i++) {
        PointChartInfo p =  points[i];
        if( inPoint(p.point, x, y))
            return i;
    }

    return -1;
}

bool ChartWidget::inPoint(Point point, int x, int y)
{
    int leftTopX = ( (point.x)*xScale ) + edgeOffset + axisEdgeOffset;
    int leftTopY = height() - ( point.y )*yScale - edgeOffset - axisEdgeOffset;

    if(x > leftTopX && x < leftTopX+pointSize )
        if(y>leftTopY && y < leftTopY+pointSize )
            return true;

    return false;
}

void ChartWidget::resetPoints()
{
    selectedPointIndex = -1;

    for(int i=0; i<points.count(); i++)
    {
        points[i].selected=false;
    }
}

void ChartWidget::selectPoint(int index)
{
    resetPoints();
    selectedPointIndex = index;
    points[index].selected = true;
    repaint();
}

void ChartWidget::moveToNewPos(int index, int x, int y)
{
    points[index].point.x = convertCoordFromWidget(x, X);
    points[index].point.y = convertCoordFromWidget(y, Y);
    resetPoints();
    repaint();
    emit pointMoved(index, x, y);
}

int ChartWidget::convertCoordFromChart(int coord, AXIS a) const
{
    if( a == X ) {
        return ((coord)*xScale ) + edgeOffset + axisEdgeOffset;
    }
    else if ( a == Y ) {
        return height() - ( coord )*yScale - edgeOffset - axisEdgeOffset;
    }
}

int ChartWidget::convertCoordFromWidget(int coord, AXIS a) const
{
    if( a == X ) {
        return (coord - (edgeOffset + axisEdgeOffset) ) /xScale;
    }
    else if ( a == Y ) {
        return  (height() -coord - edgeOffset - axisEdgeOffset)/yScale;
    }
}

ChartLine &ChartWidget::chartLines()
{
    return lines;
}

void ChartWidget::removeAllPoints()
{
    points.clear();
}

void ChartWidget::cleanAll()
{
    points.clear();
    standarts.clear();
    lines.removeAll();
    repaint();
}

