#include "fileoperations.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QStringList>

FileOperations::FileOperations()
{
}


void FileOperations::WriteFile(QVector<Point> points,QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << (file.errorString()) << endl;
        return;
    }

    QTextStream out(&file);
    foreach(Point P,points)
    {
        out<<P.x<<" "<<P.y<<" "<<P.clas<<" ";
        if(P.classType == DEFINED)
            out<<"DEFINED";
        else if(P.classType == UNDEFINED)
            out<<"UNDEFINED";
        if(P != points.last())
            out<<"\r\n";
    }
}

QVector<Point> FileOperations::ReadFile(QString filename)
{
    QVector<Point> points;
    Point P;

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Cannot open file for reading: "
             << qPrintable(file.errorString()) << endl;
        return points;
    }

    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList fields = line.split(' ');

        P.x = fields.takeFirst().toInt();
        P.y = fields.takeFirst().toInt();
        P.clas = fields.takeFirst().toInt();

        QString str = fields.takeFirst();
        if(str == "DEFINED")
            P.classType=DEFINED;
        else if(str == "UNDEFINED")
            P.classType=UNDEFINED;
        points.append(P);
    }

    return points;
}
