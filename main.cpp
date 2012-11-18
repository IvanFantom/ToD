#include <QApplication>
#include "mainwindow.h"
#include "program.h"

#include "methods.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Program program;
    program.init();
    program.showGUI();

    return a.exec();
}
