#include "mainwindow.h"

#include <QtCore/QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    chartWidget = new ChartWidget(0x0);
    chartWidget->setSizeIncrement(3, 1);
    ui->layoutData->addWidget(chartWidget);


    connect(ui->buttonCalculate, SIGNAL(clicked()), this, SIGNAL(calculateButtonClicked()));
    connect(ui->buttonDraw, SIGNAL(clicked()), this, SIGNAL(drawButtonClicked()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SIGNAL(aboutActionClicked()));
    connect(ui->cbuttonClear, SIGNAL(clicked()), this, SIGNAL(cleanButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete chartWidget;
    delete ui;
}

CalculateMethod MainWindow::getCalculateMethod()
{
    int index = ui->boxMethod->currentIndex();

    switch(index) {
        case 0:
            return CALCULATE_METHOD_K_NEIGBORDS;
            break;
        case 1:
            return CALCULATE_METHOD_STANTARD;
            break;
    }
}

CalculateMetrix MainWindow::getCalculateMetrix()
{
    int index = ui->boxMetrix->currentIndex();

     switch(index) {
         case 0:
             return CALCULATE_METRIX_EUCLIDEAN;
             break;
         case 1:
             return CALCULATE_METRIX_DIRCOS;
             break;
         case 2:
             return CALCULATE_METRIX_TINIMOTO;
             break;
     }

}

ChartWidget* MainWindow::getChartWidget() const
{
    return chartWidget;
}
