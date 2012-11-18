#include <QtCore/QDebug>

#include "program.h"
#include "mainwindow.h"
#include "chartwidget.h"
#include "methods.h"
#include "aboutdialog.h"
#include "fileoperations.h"

/*
 *Сделать умною линию
 *Сделать точки
 *Сделать меню
 *Подгрузку из файла выборки
 *Очистка линии
 *Очистка точек
 *Руссифицировать
*/

Program::Program(): fullScreen(false)
{
    init();

    (window->getChartWidget())->setMaxX(200);
    (window->getChartWidget())->setMaxY(200);

    foreach( QString arg, QApplication::arguments()) {
        if( arg.indexOf("--file=")!=-1 ) {
            int index = arg.indexOf("=")+1;
            QString uri = arg.mid(index, arg.length()-index);
            qDebug() << "Loading file: " << uri;
            QVector<Point> points =  FileOperations::ReadFile(uri);
            window->getChartWidget()->setPoints(points);

        }
        else if( arg.startsWith("--fullscreen")) {
            fullScreen = true;
        }
    }

}

void Program::init()
{
    window = new MainWindow(0x0);
    connect( window, SIGNAL(calculateButtonClicked()), this, SLOT(processCalculating()) );
    connect( window, SIGNAL(drawButtonClicked()), this, SLOT(processLineDraw()) );
    connect( window, SIGNAL(aboutActionClicked()), this, SLOT(showAboutDialog()));
    connect( window, SIGNAL(openFileSignal(QString)), this, SLOT(openFileSlot(QString)));
    connect( window, SIGNAL(saveFileSignal(QString)), this, SLOT(saveFileSlot(QString)));
    connect( window->getChartWidget(), SIGNAL(pointAdded(int)), this, SLOT(processCalculating()) );
    connect( window->getChartWidget(), SIGNAL(pointMoved(int,int,int)), this, SLOT(processCalculating()) );
    connect( window, SIGNAL(paramChangedSignal()), this, SLOT(processCalculating()) );
    connect( window, SIGNAL(cleanButtonClicked()), this, SLOT(cleanSlot()));
}


void Program::showGUI()
{
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
    CalculateMethod method = window->getCalculateMethod();



    QVector<Point> points = alg.calculateClass(window->getChartWidget()->getPoints(),
                                              method,
                                              window->getCalculateMetrix() );
    //seting up points
    window->getChartWidget()->setPoints(points);

    if( method ==  CALCULATE_METHOD_STANTARD )
        window->getChartWidget()->setStandard( alg.GetStandarts() );
    else
        window->getChartWidget()->setStandard( QVector<Point>() );

    if( method == CALCULATE_METHOD_LINES )
        processLineDraw();
    else
        window->getChartWidget()->chartLines().removeAll();


    window->setCursor(Qt::ArrowCursor);
}

void Program::processLineDraw()
{
    qDebug() << "Finding all spit lines...";
    Methods alg;
    ChartLine lines = alg.findSplitLines(window->getChartWidget()->getPoints(), 20);
    window->getChartWidget()->chartLines() = lines;
    window->getChartWidget()->repaint();
}

void Program::showAboutDialog()
{
    AboutDialog dialog;
    dialog.exec();
}

void Program::openFileSlot(QString uri)
{
   QVector<Point> points = FileOperations::ReadFile(uri);
    window->getChartWidget()->setPoints(points);
    qDebug() << "Loading file: " << uri;
}

void Program::saveFileSlot(QString uri)
{
    FileOperations::WriteFile(window->getChartWidget()->getPoints(), uri);
    qDebug() << "Saving file: " << uri;
}

void Program::cleanSlot()
{
    window->getChartWidget()->cleanAll();
}

