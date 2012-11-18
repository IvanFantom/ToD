/*!
 * @author Denis Vashchuk
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include "ui_mainwindow.h"
#include "chartwidget.h"
#include "enums.h"

namespace Ui {
class MainWindow;
}

class ChartWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    CalculateMethod getCalculateMethod();
    CalculateMetrix getCalculateMetrix();

    ChartWidget* getChartWidget() const;

private:
    ChartWidget* chartWidget;
    Ui::MainWindow *ui;
signals:
    void calculateButtonClicked();
    void drawButtonClicked();
    void aboutActionClicked();
    void cleanButtonClicked();
};

#endif // MAINWINDOW_H
