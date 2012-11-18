#include <QtCore/QDebug>

#include "program.h"
#include "mainwindow.h"
#include "chartwidget.h"
#include "methods.h"
#include "aboutdialog.h"


/*
 *Сделать умною линию
 *Сделать точки
 *Сделать меню
 *Подгрузку из файла выборки
 *Очистка линии
 *Очистка точек
 *� уссифицировать
*/

Program::Program(): fullScreen(false)
{
    if( QApplication::arguments().contains("--fullscreen") )
        fullScreen = true;

    init();
}

void Program::init()
{
    window = new MainWindow(0x0);
    connect( window, SIGNAL(calculateButtonClicked()), this, SLOT(processCalculating()) );
    connect( window, SIGNAL(drawButtonClicked()), this, SLOT(processLineDraw()) );
    connect( window, SIGNAL(aboutActionClicked()), this, SLOT(showAboutDialog()));
}


void Program::showGUI()
{
    int classCount = 2;

    (window->getChartWidget())->setMaxX(200);
    (window->getChartWidget())->setMaxY(200);

    for(int i=0; i<10; i++)
    {
        Point point;
        point.x = qrand()%200;
        point.y = qrand()%200;
        point.clas = qrand()%classCount;

        (window->getChartWidget())->addPoint(point);
    }

    if( fullScreen )
        window->showFullScreen();
    else
        window->show();
}

void Program::processCalculating()
{
    window->setCursor(Qt::WaitCursor);
    qDebug() << "Finding undefined class point class.";
    Methods alg;

    QVector<Point> points = alg.calculateClass(window->getChartWidget()->getPoints(),
                                              window->getCalculateMethod(),
                                              window->getCalculateMetrix() );
    //seting up points
    window->getChartWidget()->setPoints(points);

    if( window->getCalculateMethod() == CALCULATE_METHOD_STANTARD )
        window->getChartWidget()->setStandard( alg.GetStandarts() );
    else
        window->getChartWidget()->setStandard( QVector<Point>() );

    window->setCursor(Qt::ArrowCursor);
}

void Program::processLineDraw()
{
    qDebug() << "Finding all spit lines...";
    Methods alg;
    ChartLine lines = alg.findSplitLines(window->getChartWidget()->getPoints());
    window->getChartWidget()->chartLines() = lines;
    window->getChartWidget()->repaint();
}

void Program::showAboutDialog()
{
    AboutDialog dialog;
    dialog.exec();
}

