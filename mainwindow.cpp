#include "mainwindow.h"

#include <QtCore/QDebug>
#include <QtGui/QFileDialog>
#include <QtCore/QDir>

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
    connect(ui->actionSave_as, SIGNAL(triggered()), this, SLOT(saveFileSlot()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFileSlot()));
    connect(ui->boxMethod, SIGNAL(currentIndexChanged(int)), this, SIGNAL(paramChangedSignal()));
    connect(ui->boxMetrix, SIGNAL(currentIndexChanged(int)), this, SIGNAL(paramChangedSignal()));
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
            return CALCULATE_METHOD_STANTARD;
            break;
        case 1:
            return CALCULATE_METHOD_K_NEIGBORDS;
            break;
        case 2:
            return CALCULATE_METHOD_LINES;
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

void MainWindow::openFileSlot()
{
   QString file =  QFileDialog::getOpenFileName(this,
                                                tr("Open sample file"), QDir::home().absolutePath(),
                                                tr("Sample file (*.sample)"));

   if( ! file.isEmpty() )
       emit openFileSignal(file);
}

void MainWindow::saveFileSlot()
{
    QString file =  QFileDialog::getSaveFileName(this,
                                  tr("Save sample file"), QDir::home().absolutePath(),
                                                 tr("Sample file (*.sample)"));

    if( ! file.isEmpty() )
        emit saveFileSignal(file);
}
