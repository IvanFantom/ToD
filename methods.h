/*!
 * @author Ivan Zaharchuk
*/

#ifndef METHODS_H
#define METHODS_H

#include "point.h"
#include "chartline.h"
#include "enums.h"

#include <math.h>
#include <QVector>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <iostream>
using namespace std;

enum direction{MIN,MAX};

class Methods
{
    QVector<Point> standart; //эталоны классов
public:
    Methods();

    QVector<Point> GetStandarts();                             //возвращает эталоны класслов
    void StandartsCalculation(QVector<Point> sample);          //вычисление эталонов

    static int DirectionCosines(int x1,int y1,int x2,int y2);  //направляющие косинусы
    static int EuclideanDistance(int x1,int y1,int x2,int y2); //Евклидово расстояние
    static int TanimotoDistance(int x1,int y1,int x2,int y2);  //расстояние Танимото

    int Standarts(Point X,direction dir,int (*metric)(int x1,int y1,int x2,int y2));                               //метод эталонов
    int K_Neighbors(Point X,int K,QVector<Point> sample,direction dir,int (*metric)(int x1,int y1,int x2,int y2)); //метод k-ближайших соседей

    QVector<Point> calculateClass(QVector<Point> points,CalculateMethod method,CalculateMetrix metrix);

    QVector<Point> findSplitLine(QVector<Point> srcpoints,int dx,int clas0, int clas1);
    ChartLine findSplitLines(QVector<Point> srcpoints,int dx);

    void WriteFile(QVector<Point> points,QString filename);
    QVector<Point> ReadFile(QString filename);
};

#endif // METHODS_H